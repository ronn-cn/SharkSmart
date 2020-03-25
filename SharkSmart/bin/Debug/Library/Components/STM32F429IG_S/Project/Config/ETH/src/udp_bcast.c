#include "udp_bcast.h"
#include "lwip_comm.h"
#include "log.h"
#include "rtos.h"
#include "lwip/api.h"
#include "lwip/lwip_sys.h"
#include "lwip/ip.h"
#include "string.h"
#include "malloc.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "exfuns.h"
#include "ff.h"
#include "eth_user.h"
#include "eth_upgrade.h"
struct sockaddr_in udp_ret_addr;

static int udp_normal_sock_fd = -1;
uint8_t udp_bcast_send_flag = 0;
uint8_t udp_back_send_info_flag = 0;

DeviceInfoStruct Lwip_Info;

DeviceInfoStruct DeviceInfoList[10];
uint8_t DeviceInfoCnt = 0;

//lwip接收函数指针
extern uint8_t ( *user_eth_normal_recv)(uint8_t *user_recv,int user_len,uint32_t user_sin_addr);

//lwip发送函数
int lwip_udp_normal_send(uint8_t *udp_send,int udp_len,uint32_t udp_sin_addr)
{
	if(udp_send == NULL)
	{
		return -1;
	}

	struct sockaddr_in UDP_normal_addr;
	UDP_normal_addr.sin_family = AF_INET;
	UDP_normal_addr.sin_port = htons(10000);
	UDP_normal_addr.sin_addr.s_addr = udp_sin_addr;
	return sendto(udp_normal_sock_fd, udp_send, udp_len, 0,(struct sockaddr*) &UDP_normal_addr, sizeof(UDP_normal_addr));
}

//UDP自身信息初始化
void udp_LwipInfo_init(void)
{
	memset(&Lwip_Info, 0, sizeof(Lwip_Info));
	Lwip_Info.ipv4 = lwip_netif.ip_addr.addr;
	memcpy(Lwip_Info.mac, g_local_mac, 6);
	memcpy(Lwip_Info.name_buf, "STM32F429 V3.3",14);
	Lwip_Info.name_len = 14;
	Lwip_Info.net_sta = LWIP_STA;
	
	LOG(LEVEL_INFO, "lwip len: %d\n", Lwip_Info.name_len);
    LOG(LEVEL_INFO, "lwip Name: %s\n", Lwip_Info.name_buf);
    LOG(LEVEL_INFO, "lwip IP:   %s\n", inet_ntoa(Lwip_Info.ipv4));
    LOG(LEVEL_INFO, "lwip MAC:  %02X:%02X:%02X:%02X:%02X:%02X\n\n", Lwip_Info.mac[0], 
                                                                    Lwip_Info.mac[1], 
                                                                    Lwip_Info.mac[2], 
                                                                    Lwip_Info.mac[3], 
                                                                    Lwip_Info.mac[4], 
                                                                    Lwip_Info.mac[5]);
	
}

//检查数据是否是设备信息
//check_buf：要检查的数据
//check_len：要检查的长度、
//返回值：0：成功；
//		1：参数错误；
//		2：数据不是设备信息；
static uint8_t udp_check_Info(uint8_t *check_buf,uint16_t check_len)
{
	DeviceInfoStruct Recv_Info;

	if(check_buf==NULL)
	{
		return 1;
	}
	
	if(memcmp(check_buf, "YQADV", 5) == 0)
	{
		memcpy(&Recv_Info, check_buf + 5, check_len - 5);

		LOG(LEVEL_INFO, "Recv Name: %s\n", Recv_Info.name_buf);
		LOG(LEVEL_INFO, "Recv IP:   %s\n", inet_ntoa(Recv_Info.ipv4));
		LOG(LEVEL_INFO, "Recv MAC:  %02X:%02X:%02X:%02X:%02X:%02X\n\n", Recv_Info.mac[0], 
																		Recv_Info.mac[1], 
																		Recv_Info.mac[2], 
																		Recv_Info.mac[3], 
																		Recv_Info.mac[4], 
																		Recv_Info.mac[5]);

		if(DeviceInfoCnt == 0)
		{
			memcpy(&DeviceInfoList[DeviceInfoCnt++], &Recv_Info, sizeof(Recv_Info));
		}
		else
		{
			uint8_t i = 0;
			for(i = 0; i < DeviceInfoCnt; i++)
			{
				if(memcmp(DeviceInfoList[i].mac, Recv_Info.mac, 6) == 0)
				{
					break;
				}
			}
			if(i == DeviceInfoCnt)
			{
				memcpy(&DeviceInfoList[DeviceInfoCnt++], &Recv_Info, sizeof(Recv_Info));
			}
		}
		return 0;
	}else{
		return 2;
	}
    return 2;
}

#define UDP_BCAST_PRIO		UDP_BCAST_PRIO_NUM
#define UDP_BCAST_STK_SIZE	2024
uint32_t UDP_BCAST_TASK_STK[UDP_BCAST_STK_SIZE];

//udp广播任务函数
static void udp_bcast_thread(void *arg)
{
	while(dhcpstatus != 2)
	{
		OSTimeDly(1000);
	}
	struct sockaddr_in local_addr;
	struct sockaddr_in from_addr;
	struct sockaddr_in adv_addr;
	int from_addr_len = sizeof(from_addr);
	int sock_fd;
	int timeout = 20;

	char send_buf[100];
	char recv_buf[100];
	int recv_len = 0;
	int send_len = 0;

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(9999);

	adv_addr.sin_family = AF_INET;
	adv_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	adv_addr.sin_port = htons(9999);

	setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout,sizeof(timeout));
	bind(sock_fd, (struct sockaddr*) &local_addr, sizeof(local_addr));

	while(1)
	{
		memset(recv_buf, 0, sizeof(recv_buf));
		recv_len = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0,(struct sockaddr*)&from_addr, (socklen_t *)&from_addr_len);
		if(recv_len > 0)
		{
			if(udp_check_Info((uint8_t *)recv_buf,recv_len)==0)
			{
				if(udp_back_send_info_flag==0)
				{
					memcpy(&udp_ret_addr, &from_addr, sizeof(from_addr));
					udp_back_send_info_flag=1;
				}
				
			}
			
			LOG(LEVEL_INFO, "broat_Recv %s:%d >(%d) %s\n",inet_ntoa(from_addr.sin_addr),htons(from_addr.sin_port), recv_len, recv_buf);
		}
		if(udp_bcast_send_flag != 0)
		{
			send_len = 0;
			memset(send_buf, 0, sizeof(send_buf));
			sprintf(send_buf,"YQADV");
			send_len += 5;
			memcpy(send_buf + send_len, &Net_Local_Info, sizeof(Net_Local_Info));
			send_len += sizeof(Net_Local_Info);

			sendto(sock_fd, send_buf, send_len, 0,(struct sockaddr*) &adv_addr, sizeof(adv_addr));
			udp_bcast_send_flag--;
			OSTimeDly(1000);
		}
		OSTimeDly(10);
	}
	closesocket(sock_fd);
}

#define UDP_NORMAL_PRIO		UDP_NORMAL_PRIO_NUM
#define UDP_NORMAL_STK_SIZE	2024
uint32_t UDP_NORMAL_TASK_STK[UDP_NORMAL_STK_SIZE];
//UDP普通数据处理任务
static void udp_normal_thread(void *arg)
{
	while(dhcpstatus != 2)
	{
		OSTimeDly(1000);
	}
	struct sockaddr_in local_addr;
	struct sockaddr_in from_addr;
	int from_addr_len = sizeof(from_addr);
	int timeout = 100;

	uint8_t send_buf[100];
	uint8_t recv_buf[100];
	int recv_len = 0;
	int send_len = 0;

	udp_normal_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = lwip_netif.ip_addr.addr;
	local_addr.sin_port = htons(10000);

	setsockopt(udp_normal_sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout,sizeof(timeout));
	bind(udp_normal_sock_fd, (struct sockaddr*) &local_addr, sizeof(local_addr));
	
	while(1)
	{
		memset(recv_buf, 0, sizeof(recv_buf));
		recv_len = recvfrom(udp_normal_sock_fd, recv_buf, sizeof(recv_buf), 0,(struct sockaddr*)&from_addr, (socklen_t *)&from_addr_len);
		if(recv_len > 0)
		{
			LOG(LEVEL_INFO, "normal_Recv %s:%d >(%d) %s\n",inet_ntoa(from_addr.sin_addr),htons(from_addr.sin_port), recv_len, recv_buf);
			if(udp_check_Info(recv_buf,recv_len))
			{
				//接收到普通数据 处理
				user_eth_normal_recv(recv_buf,recv_len,from_addr.sin_addr.s_addr);
			}
		}

		if(udp_back_send_info_flag != 0)
		{
			udp_ret_addr.sin_port = htons(10000);

			send_len = 0;
			memset(send_buf, 0, sizeof(send_buf));
			send_buf[send_len++] = 'Y';
			send_buf[send_len++] = 'Q';
			send_buf[send_len++] = 'A';
			send_buf[send_len++] = 'D';
			send_buf[send_len++] = 'V';

			memcpy(send_buf + send_len, &Net_Local_Info, sizeof(Net_Local_Info));
			send_len += sizeof(Net_Local_Info);

			sendto(udp_normal_sock_fd, send_buf, send_len, 0,(struct sockaddr*) &udp_ret_addr, sizeof(udp_ret_addr));
			udp_back_send_info_flag--;
			OSTimeDly(1000);
		}
		OSTimeDly(10);
	}
	closesocket(udp_normal_sock_fd);
}


//创建LWIP线程
void lwip_udp_creat(void)
{
	uint32_t cpu_sr;
	
	OS_ENTER_CRITICAL();	//关中断

	OSTaskCreate(udp_bcast_thread, \
				"udp_bcast", \
				NULL, \
				(uint32_t*)&UDP_BCAST_TASK_STK[UDP_BCAST_STK_SIZE-1], \
				UDP_BCAST_PRIO);
	OSTaskCreate(udp_normal_thread, \
				"udp_normal", \
				NULL, \
				(uint32_t*)&UDP_NORMAL_TASK_STK[UDP_NORMAL_STK_SIZE-1], \
				UDP_NORMAL_PRIO);
	
				
	OSTaskSuspend(UDP_BCAST_PRIO);
	OSTaskSuspend(UDP_NORMAL_PRIO);

	
	OS_EXIT_CRITICAL();
}
