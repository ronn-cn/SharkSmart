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
struct netif lwip_netif;				//����һ��ȫ�ֵ�����ӿ�


extern uint32_t memp_get_memorysize(void);	//��memp.c���涨��
extern u8_t *memp_memory;				//��memp.c���涨��.
extern u8_t *ram_heap;					//��mem.c���涨��.
	vu8 dhcpstatus = 0;		//dhcp״̬ 		//0,δ��ȡDHCP��ַ;
										//1,����DHCP��ȡ״̬
										//2,�ɹ���ȡDHCP��ַ
										//0XFF,��ȡʧ��.
/////////////////////////////////////////////////////////////////////////////////
//lwip����������(�ں������DHCP����)

//lwip�ں������ջ(���ȼ��Ͷ�ջ��С��lwipopts.h������) 
uint32_t * TCPIP_THREAD_TASK_STK;	 

//lwip DHCP����
//�����������ȼ�
#define LWIP_DHCP_TASK_PRIO       		LWIP_DHCP_TASK_PRIO_NUM
//���������ջ��С
#define LWIP_DHCP_STK_SIZE  		    1024
//�����ջ�������ڴ����ķ�ʽ��������	
uint32_t * LWIP_DHCP_TASK_STK;	
//������
void lwip_dhcp_task(void *pdata); 


//������̫���жϵ���
void lwip_pkt_handle(void)
{
	ethernetif_input(&lwip_netif);
}

void lwip_user_common_init(void)
{
	LOG(LEVEL_INFO,"lwip ���ڳ�ʼ������ȷ����������......\n");
	if(lwip_comm_init() != 0) 	    //lwip��ʼ��
	{
		LOG(LEVEL_ERROR,"û�����ߣ������ߺ���Ҫ����������\n");
	}
	else
    {

    }	
}
//lwip��mem��memp���ڴ�����
//����ֵ:0,�ɹ�;
//    ����,ʧ��
uint8_t lwip_comm_mem_malloc(void)
{
	uint32_t mempsize;
	uint32_t ramheapsize; 
	mempsize=memp_get_memorysize();			//�õ�memp_memory�����С
	memp_memory=mymalloc(SRAMEX,mempsize);	//Ϊmemp_memory�����ڴ�
	ramheapsize=LWIP_MEM_ALIGN_SIZE(MEM_SIZE)+2*LWIP_MEM_ALIGN_SIZE(4*3)+MEM_ALIGNMENT;//�õ�ram heap��С
	ram_heap=mymalloc(SRAMEX,ramheapsize);	//Ϊram_heap�����ڴ�     
    TCPIP_THREAD_TASK_STK=mymalloc(SRAMEX,TCPIP_THREAD_STACKSIZE*4);//���ں����������ջ 
	LWIP_DHCP_TASK_STK=mymalloc(SRAMEX,LWIP_DHCP_STK_SIZE*4);		//��dhcp�����ջ�����ڴ�ռ�
	if(!memp_memory||!ram_heap||!TCPIP_THREAD_TASK_STK||!LWIP_DHCP_TASK_STK)//������ʧ�ܵ�
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}
//lwip��mem��memp�ڴ��ͷ�
void lwip_comm_mem_free(void)
{ 	
	myfree(SRAMEX,memp_memory);
	myfree(SRAMEX,ram_heap);
    myfree(SRAMEX,TCPIP_THREAD_TASK_STK);
	myfree(SRAMEX,LWIP_DHCP_TASK_STK);
}

//��ȡIP����
uint32_t ip_config(const char *ipaddress)
{
    int a, b, c, d;
    if (sscanf(ipaddress, "%d.%d.%d.%d", &a, &b, &c, &d) == 4)
    {
        return (a << 24) | (b << 16) | (c << 8) | d;
    }
    return 0;
}

//��ȡmac��ַ
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

//LWIP��ʼ��(LWIP������ʱ��ʹ��)
//����ֵ:0,�ɹ�
//      1,�ڴ����
//      2,LAN8720��ʼ��ʧ��
//      3,�������ʧ��.
uint8_t lwip_comm_init(void)
{
	struct netif *Netif_Init_Flag;		//����netif_add()����ʱ�ķ���ֵ,�����ж������ʼ���Ƿ�ɹ�
	struct ip_addr ipaddr;  			//ip��ַ
	struct ip_addr netmask; 			//��������
	struct ip_addr gw;      			//Ĭ������ 
	if(ETH_Mem_Malloc())return 1;		//�ڴ�����ʧ��
	if(lwip_comm_mem_malloc())return 2;	//�ڴ�����ʧ��
	dhcpstatus = 0;						//û��DHCP	
    if(LAN8720_Init())		       		//��ʼ��LAN8720,���ʧ�ܵĻ�������5��
    {
		lan8720_state = 0;
        return 3; //LAN8720��ʼ��ʧ��
    }
	lan8720_state = 1;
	tcpip_init(NULL,NULL);				//��ʼ��tcp ip�ں�,�ú�������ᴴ��tcpip_thread�ں�����

#if LWIP_DHCP		//ʹ�ö�̬IP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else				//ʹ�þ�̬IP
	unsigned int mac_addr[6];					//MAC��ַ
	ipaddr.addr = ip_config(LOCAL_STATIC_IP);
	netmask.addr = ip_config(LOCAL_STATIC_NETMASK);
	gw.addr = ip_config(LOCAL_STATIC_GATEWAY);
	mac_get(mac_addr);
	LOG(LEVEL_INFO,"����en��MAC��ַΪ:................%02X-%02X-%02X-%02X-%02X-%02X\r\n",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
	LOG(LEVEL_INFO,"��̬IP��ַ........................%s\r\n",LOCAL_STATIC_IP);
	LOG(LEVEL_INFO,"��������..........................%s\r\n",LOCAL_STATIC_NETMASK);
	LOG(LEVEL_INFO,"Ĭ������..........................%s\r\n",LOCAL_STATIC_GATEWAY);	

#endif
	Netif_Init_Flag=netif_add(&lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&tcpip_input);//�������б������һ������
	if(Netif_Init_Flag==NULL)return 4;//�������ʧ�� 
	else//������ӳɹ���,����netifΪĬ��ֵ,���Ҵ�netif����
	{
		netif_set_default(&lwip_netif); //����netifΪĬ������
		netif_set_up(&lwip_netif);		//��netif����
	}
	return 0;//����OK.
}   


//���ʹ����DHCP
#if LWIP_DHCP
//����DHCP����
void lwip_comm_dhcp_creat(void)
{
	//OS_CPU_SR cpu_sr;
	//OS_ENTER_CRITICAL();  //�����ٽ���
	OSTaskCreate(lwip_dhcp_task,"lwip_dhcp_task",(void*)0,(uint32_t*)&LWIP_DHCP_TASK_STK[LWIP_DHCP_STK_SIZE-1],LWIP_DHCP_TASK_PRIO);//����DHCP���� 
	//OS_EXIT_CRITICAL();  //�˳��ٽ���
}
//ɾ��DHCP����
void lwip_comm_dhcp_delete(void)
{
	dhcp_stop(&lwip_netif); 		//�ر�DHCP
	OSTaskSuspend(LWIP_DHCP_TASK_PRIO);	//ɾ��DHCP����
}


void my_found(const char *name, struct ip_addr *ipaddr, void *arg)
{
    uint8_t ip[4];
    ip[0] = ipaddr->addr>>24; 
    ip[1] = ipaddr->addr>>16;
    ip[2] = ipaddr->addr>>8; 
    ip[3] = ipaddr->addr; 


   LOG(LEVEL_INFO,"\r\n%s����������IP:%d.%d.%d.%d\n",name,ip[3], ip[2], ip[1], ip[0]); 
}
void DNS_Test(void)
{
    struct ip_addr DNS_IP;
    char hostname[] = "www.baidu.com";
    dns_gethostbyname(hostname,&DNS_IP,my_found,NULL);

}
//DHCP��������
void lwip_dhcp_task(void *pdata)
{
	struct netif *Netif_Init_Flag;		//����netif_add()����ʱ�ķ���ֵ,�����ж������ʼ���Ƿ�ɹ�
	struct ip_addr ipaddr0;  			//ip��ַ
	struct ip_addr netmask0; 			//��������
	struct ip_addr gw0;      			//Ĭ������ 
	uint32_t ip=0,netmask=0,gw=0;
	
	while(1)
	{
		if(lan8720_state == 1&&dhcpstatus == 0)
		{
			ip=0;
			netmask=0;
			gw=0;
			dhcp_start(&lwip_netif);//����DHCP 
			dhcpstatus=0;			//����DHCP
			unsigned int  mac[6];
			mac_get(mac);
			LOG(LEVEL_INFO,"\n���ڲ���DHCP������,���Ե�...........\r\n"); 
			LOG(LEVEL_INFO,"���ڻ�ȡ��ַ");    
			while(1)
			{ 
				LOG(LEVEL_INFO,".");
				ip=lwip_netif.ip_addr.addr;		//��ȡ��IP��ַ
				netmask=lwip_netif.netmask.addr;//��ȡ��������
				gw=lwip_netif.gw.addr;			//��ȡĬ������ 
				if(ip!=0)   					//����ȷ��ȡ��IP��ַ��ʱ��
				{
					dhcpstatus=2;	//DHCP�ɹ�
					LOG(LEVEL_INFO,"\n����en��MAC��ַΪ:................%02X-%02X-%02X-%02X-%02X-%02X\r\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
					//������ͨ��DHCP��ȡ����IP��ַ
					LOG(LEVEL_INFO,"ͨ��DHCP��ȡ��IP��ַ..............%d.%d.%d.%d\r\n",(uint8_t)(ip),(uint8_t)(ip>>8),(uint8_t)(ip>>16),(uint8_t)(ip>>24));
					//����ͨ��DHCP��ȡ�������������ַ
					LOG(LEVEL_INFO,"ͨ��DHCP��ȡ����������............%d.%d.%d.%d\r\n",(uint8_t)(netmask),(uint8_t)(netmask>>8),(uint8_t)(netmask>>16),(uint8_t)(netmask>>24));
					//������ͨ��DHCP��ȡ����Ĭ������
					LOG(LEVEL_INFO,"ͨ��DHCP��ȡ����Ĭ������..........%d.%d.%d.%d\r\n",(uint8_t)(gw),(uint8_t)(gw>>8),(uint8_t)(gw>>16),(uint8_t)(gw>>24));
				
					break;
				}else if(lwip_netif.dhcp->tries>LWIP_MAX_DHCP_TRIES) //ͨ��DHCP�����ȡIP��ַʧ��,�ҳ�������Դ���
				{  
					// dhcp_discover(&lwip_netif);
					//LOG(LEVEL_INFO, "dhcp_stop, %d\n", dhcp_stop(&lwip_netif));
					//LOG(LEVEL_INFO,"dhcp_start, %d\n\n", dhcp_start(&lwip_netif));
					dhcpstatus=0XFF;//DHCPʧ��
					//LOG(LEVEL_INFO,"��ȡIPʧ��\n");
					break;
				}  
				OSTimeDly(250); //��ʱ250ms
			}
		}


		while(1)
		{
			if(LAN8720_ReadPHY(PHY_BSR)&PHY_LINKED_STATUS)
			{
				if(lan8720_state == 0&&LAN8720_Init()==0)		       		
				{
					lan8720_state = 1;	
					tcpip_init(NULL,NULL);				//��ʼ��tcp ip�ں�,�ú�������ᴴ��tcpip_thread�ں�����
					ipaddr0.addr = 0;
					netmask0.addr = 0;
					gw0.addr = 0;

					Netif_Init_Flag=netif_add(&lwip_netif,&ipaddr0,&netmask0,&gw0,NULL,&ethernetif_init,&tcpip_input);//�������б������һ������
					if(Netif_Init_Flag==NULL)return ;//�������ʧ�� 
					else//������ӳɹ���,����netifΪĬ��ֵ,���Ҵ�netif����
					{
						netif_set_default(&lwip_netif); //����netifΪĬ������
						netif_set_up(&lwip_netif);		//��netif����
					}
					break;
				}else if(lan8720_state == 1&&dhcpstatus == 0){
					break;
				}
			}else{
				dhcpstatus = 0;
				LOG(LEVEL_INFO, "Please check the network connection!\n" );
				OSTimeDly(30000); //��ʱ250ms
			}
			OSTimeDly(1000); //��ʱ250ms
		}


		OSTimeDly(1000); 
		
	}
	// lwip_comm_dhcp_delete();//ɾ��DHCP���� 
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
