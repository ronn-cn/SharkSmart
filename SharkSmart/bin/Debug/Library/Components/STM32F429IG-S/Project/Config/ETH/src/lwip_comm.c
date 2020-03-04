#include "lwip_comm.h" 
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "ethernetif.h" 
#include "lwip/timers.h"
#include "lwip/tcp_impl.h"
#include "lwip/ip_frag.h"
#include "lwip/tcpip.h" 
#include "lwip/timers.h"
#include "lwip/dns.h"
#include "malloc.h"
#include "delay.h"
#include "log.h" 
#include <stdio.h>
#include "user_ports.h"
#include "stdlib.h" 
#include "lwip/inet.h"
#include "string.h"
uint8_t lan8720_state = 0;

uint8_t g_local_mac[6];
//////////////////////////////////////////////////////////////////////////////////
struct netif lwip_netif;				//定义一个全局的网络接口


extern uint32_t memp_get_memorysize(void);	//在memp.c里面定义
extern u8_t *memp_memory;				//在memp.c里面定义.
extern u8_t *ram_heap;					//在mem.c里面定义.
	vu8 dhcpstatus = 0;		//dhcp状态 		//0,未获取DHCP地址;
										//1,进入DHCP获取状态
										//2,成功获取DHCP地址
										//0XFF,获取失败.
/////////////////////////////////////////////////////////////////////////////////
//lwip两个任务定义(内核任务和DHCP任务)

//lwip内核任务堆栈(优先级和堆栈大小在lwipopts.h定义了) 
uint32_t * TCPIP_THREAD_TASK_STK;	 

//lwip DHCP任务
//设置任务优先级
#define LWIP_DHCP_TASK_PRIO       		LWIP_DHCP_TASK_PRIO_NUM
//设置任务堆栈大小
#define LWIP_DHCP_STK_SIZE  		    1024
//任务堆栈，采用内存管理的方式控制申请	
uint32_t * LWIP_DHCP_TASK_STK;	
//任务函数
void lwip_dhcp_task(void *pdata); 


//用于以太网中断调用
void lwip_pkt_handle(void)
{
	ethernetif_input(&lwip_netif);
}

void lwip_user_common_init(void)
{
	LOG(LEVEL_INFO,"lwip 正在初始化，请确保插上网线......\n");
	if(lwip_comm_init() != 0) 	    //lwip初始化
	{
		LOG(LEVEL_ERROR,"没插网线，插网线后需要重启！！！\n");
	}
	else
    {

    }	
}
//lwip中mem和memp的内存申请
//返回值:0,成功;
//    其他,失败
uint8_t lwip_comm_mem_malloc(void)
{
	uint32_t mempsize;
	uint32_t ramheapsize; 
	mempsize=memp_get_memorysize();			//得到memp_memory数组大小
	memp_memory=mymalloc(SRAMEX,mempsize);	//为memp_memory申请内存
	ramheapsize=LWIP_MEM_ALIGN_SIZE(MEM_SIZE)+2*LWIP_MEM_ALIGN_SIZE(4*3)+MEM_ALIGNMENT;//得到ram heap大小
	ram_heap=mymalloc(SRAMEX,ramheapsize);	//为ram_heap申请内存     
    TCPIP_THREAD_TASK_STK=mymalloc(SRAMEX,TCPIP_THREAD_STACKSIZE*4);//给内核任务申请堆栈 
	LWIP_DHCP_TASK_STK=mymalloc(SRAMEX,LWIP_DHCP_STK_SIZE*4);		//给dhcp任务堆栈申请内存空间
	if(!memp_memory||!ram_heap||!TCPIP_THREAD_TASK_STK||!LWIP_DHCP_TASK_STK)//有申请失败的
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}
//lwip中mem和memp内存释放
void lwip_comm_mem_free(void)
{ 	
	myfree(SRAMEX,memp_memory);
	myfree(SRAMEX,ram_heap);
    myfree(SRAMEX,TCPIP_THREAD_TASK_STK);
	myfree(SRAMEX,LWIP_DHCP_TASK_STK);
}

//读取IP配置
uint32_t ip_config(const char *ipaddress)
{
    int a, b, c, d;
    if (sscanf(ipaddress, "%d.%d.%d.%d", &a, &b, &c, &d) == 4)
    {
        return (a << 24) | (b << 16) | (c << 8) | d;
    }
    return 0;
}

//获取mac地址
void mac_get(unsigned int * mac)
{
	uint8_t id_data[12];

	memcpy(id_data, (uint8_t *)0x1FFF7A10, 12);
	sscanf(LOCAL_MAC_ADDR, "%2X-%2X-%2X-%2X-%2X-%2X", 
							(unsigned int *)&mac[0], 
							(unsigned int *)&mac[1], 
							(unsigned int *)&mac[2], 
							(unsigned int *)&mac[3], 
							(unsigned int *)&mac[4], 
							(unsigned int *)&mac[5]);
	if((mac[0]==0)&&(mac[1]==0)&&(mac[2]==0)&&(mac[3]==0)&&(mac[4]==0)&&(mac[5]==0))
	{
		mac[0] = 0;
		mac[1] = 0;
		mac[2] = 0;
		mac[3] = id_data[0] + id_data[3] + id_data[6] + id_data[9];
		mac[4] = id_data[1] + id_data[4] + id_data[7] + id_data[10];
		mac[5] = id_data[2] + id_data[5] + id_data[8] + id_data[11];
	}

	//memcpy(g_local_mac, mac, 6);

	g_local_mac[0] = mac[0];
	g_local_mac[1] = mac[1];
	g_local_mac[2] = mac[2];
	g_local_mac[3] = mac[3];
	g_local_mac[4] = mac[4];
	g_local_mac[5] = mac[5];
}

//LWIP初始化(LWIP启动的时候使用)
//返回值:0,成功
//      1,内存错误
//      2,LAN8720初始化失败
//      3,网卡添加失败.
uint8_t lwip_comm_init(void)
{
	struct netif *Netif_Init_Flag;		//调用netif_add()函数时的返回值,用于判断网络初始化是否成功
	struct ip_addr ipaddr;  			//ip地址
	struct ip_addr netmask; 			//子网掩码
	struct ip_addr gw;      			//默认网关 
	if(ETH_Mem_Malloc())return 1;		//内存申请失败
	if(lwip_comm_mem_malloc())return 2;	//内存申请失败
	dhcpstatus = 0;						//没有DHCP	
    if(LAN8720_Init())		       		//初始化LAN8720,如果失败的话就重试5次
    {
		lan8720_state = 0;
        return 3; //LAN8720初始化失败
    }
	lan8720_state = 1;
	tcpip_init(NULL,NULL);				//初始化tcp ip内核,该函数里面会创建tcpip_thread内核任务

#if LWIP_DHCP		//使用动态IP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else				//使用静态IP
	unsigned int mac_addr[6];					//MAC地址
	ipaddr.addr = ip_config(LOCAL_STATIC_IP);
	netmask.addr = ip_config(LOCAL_STATIC_NETMASK);
	gw.addr = ip_config(LOCAL_STATIC_GATEWAY);
	mac_get(mac_addr);
	LOG(LEVEL_INFO,"网卡en的MAC地址为:................%02X-%02X-%02X-%02X-%02X-%02X\r\n",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
	LOG(LEVEL_INFO,"静态IP地址........................%s\r\n",LOCAL_STATIC_IP);
	LOG(LEVEL_INFO,"子网掩码..........................%s\r\n",LOCAL_STATIC_NETMASK);
	LOG(LEVEL_INFO,"默认网关..........................%s\r\n",LOCAL_STATIC_GATEWAY);	

#endif
	Netif_Init_Flag=netif_add(&lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&tcpip_input);//向网卡列表中添加一个网口
	if(Netif_Init_Flag==NULL)return 4;//网卡添加失败 
	else//网口添加成功后,设置netif为默认值,并且打开netif网口
	{
		netif_set_default(&lwip_netif); //设置netif为默认网口
		netif_set_up(&lwip_netif);		//打开netif网口
	}
	return 0;//操作OK.
}   


//如果使能了DHCP
#if LWIP_DHCP
//创建DHCP任务
void lwip_comm_dhcp_creat(void)
{
	//OS_CPU_SR cpu_sr;
	//OS_ENTER_CRITICAL();  //进入临界区
	OSTaskCreate(lwip_dhcp_task,"lwip_dhcp_task",(void*)0,(uint32_t*)&LWIP_DHCP_TASK_STK[LWIP_DHCP_STK_SIZE-1],LWIP_DHCP_TASK_PRIO);//创建DHCP任务 
	//OS_EXIT_CRITICAL();  //退出临界区
}
//删除DHCP任务
void lwip_comm_dhcp_delete(void)
{
	dhcp_stop(&lwip_netif); 		//关闭DHCP
	OSTaskSuspend(LWIP_DHCP_TASK_PRIO);	//删除DHCP任务
}


void my_found(const char *name, struct ip_addr *ipaddr, void *arg)
{
    uint8_t ip[4];
    ip[0] = ipaddr->addr>>24; 
    ip[1] = ipaddr->addr>>16;
    ip[2] = ipaddr->addr>>8; 
    ip[3] = ipaddr->addr; 


   LOG(LEVEL_INFO,"\r\n%s域名解析的IP:%d.%d.%d.%d\n",name,ip[3], ip[2], ip[1], ip[0]); 
}
void DNS_Test(void)
{
    struct ip_addr DNS_IP;
    char hostname[] = "www.baidu.com";
    dns_gethostbyname(hostname,&DNS_IP,my_found,NULL);

}
//DHCP处理任务
void lwip_dhcp_task(void *pdata)
{
	struct netif *Netif_Init_Flag;		//调用netif_add()函数时的返回值,用于判断网络初始化是否成功
	struct ip_addr ipaddr0;  			//ip地址
	struct ip_addr netmask0; 			//子网掩码
	struct ip_addr gw0;      			//默认网关 
	uint32_t ip=0,netmask=0,gw=0;
	
	while(1)
	{
		if(lan8720_state == 1&&dhcpstatus == 0)
		{
			ip=0;
			netmask=0;
			gw=0;
			dhcp_start(&lwip_netif);//开启DHCP 
			dhcpstatus=0;			//正在DHCP
			unsigned int  mac[6];
			mac_get(mac);
			LOG(LEVEL_INFO,"\n正在查找DHCP服务器,请稍等...........\r\n"); 
			LOG(LEVEL_INFO,"正在获取地址");    
			while(1)
			{ 
				LOG(LEVEL_INFO,".");
				ip=lwip_netif.ip_addr.addr;		//读取新IP地址
				netmask=lwip_netif.netmask.addr;//读取子网掩码
				gw=lwip_netif.gw.addr;			//读取默认网关 
				if(ip!=0)   					//当正确读取到IP地址的时候
				{
					dhcpstatus=2;	//DHCP成功
					LOG(LEVEL_INFO,"\n网卡en的MAC地址为:................%02X-%02X-%02X-%02X-%02X-%02X\r\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
					//解析出通过DHCP获取到的IP地址
					LOG(LEVEL_INFO,"通过DHCP获取到IP地址..............%d.%d.%d.%d\r\n",(uint8_t)(ip),(uint8_t)(ip>>8),(uint8_t)(ip>>16),(uint8_t)(ip>>24));
					//解析通过DHCP获取到的子网掩码地址
					LOG(LEVEL_INFO,"通过DHCP获取到子网掩码............%d.%d.%d.%d\r\n",(uint8_t)(netmask),(uint8_t)(netmask>>8),(uint8_t)(netmask>>16),(uint8_t)(netmask>>24));
					//解析出通过DHCP获取到的默认网关
					LOG(LEVEL_INFO,"通过DHCP获取到的默认网关..........%d.%d.%d.%d\r\n",(uint8_t)(gw),(uint8_t)(gw>>8),(uint8_t)(gw>>16),(uint8_t)(gw>>24));
				
					break;
				}else if(lwip_netif.dhcp->tries>LWIP_MAX_DHCP_TRIES) //通过DHCP服务获取IP地址失败,且超过最大尝试次数
				{  
					// dhcp_discover(&lwip_netif);
					//LOG(LEVEL_INFO, "dhcp_stop, %d\n", dhcp_stop(&lwip_netif));
					//LOG(LEVEL_INFO,"dhcp_start, %d\n\n", dhcp_start(&lwip_netif));
					dhcpstatus=0XFF;//DHCP失败
					//LOG(LEVEL_INFO,"获取IP失败\n");
					break;
				}  
				OSTimeDly(250); //延时250ms
			}
		}


		while(1)
		{
			if(LAN8720_ReadPHY(PHY_BSR)&PHY_LINKED_STATUS)
			{
				if(lan8720_state == 0&&LAN8720_Init()==0)		       		
				{
					lan8720_state = 1;	
					tcpip_init(NULL,NULL);				//初始化tcp ip内核,该函数里面会创建tcpip_thread内核任务
					ipaddr0.addr = 0;
					netmask0.addr = 0;
					gw0.addr = 0;

					Netif_Init_Flag=netif_add(&lwip_netif,&ipaddr0,&netmask0,&gw0,NULL,&ethernetif_init,&tcpip_input);//向网卡列表中添加一个网口
					if(Netif_Init_Flag==NULL)return ;//网卡添加失败 
					else//网口添加成功后,设置netif为默认值,并且打开netif网口
					{
						netif_set_default(&lwip_netif); //设置netif为默认网口
						netif_set_up(&lwip_netif);		//打开netif网口
					}
					break;
				}else if(lan8720_state == 1&&dhcpstatus == 0){
					break;
				}
			}else{
				dhcpstatus = 0;
				LOG(LEVEL_INFO, "Please check the network connection!\n" );
				OSTimeDly(30000); //延时250ms
			}
			OSTimeDly(1000); //延时250ms
		}


		OSTimeDly(1000); 
		
	}
	// lwip_comm_dhcp_delete();//删除DHCP任务 
}
void socketaddr_set(struct sockaddr *addr, char *ip, uint16_t *port)
{
	struct sockaddr_in *temp;
	temp = (struct sockaddr_in *)addr;
	temp->sin_addr.s_addr = inet_addr(ip);
	temp->sin_port = *port;
}

void socketaddr_get(struct sockaddr *addr, char *ip, uint16_t *port)
{
	struct sockaddr_in *temp;
	temp = (struct sockaddr_in *)addr;
	strcpy(ip, inet_ntoa(temp->sin_addr.s_addr));
	*port = temp->sin_port;
}



#endif 
