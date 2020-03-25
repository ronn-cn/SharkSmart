#include "eth_user.h"
#include "udp_bcast.h"
#include "m2_interface.h"
#include "rtos.h"
#include "lwip_comm.h"
#include "malloc.h"
#include "log.h" 
#include "lwip/dhcp.h"
#include "eth_upgrade.h"
#include "stdio.h"
#include "string.h"
//网络状态
uint8_t g_net_user_sta = 0;
DeviceInfoStruct Net_Local_Info;

uint8_t ( *user_eth_normal_recv)(uint8_t *user_recv,int user_len,uint32_t user_sin_addr);
//ETH用户接收函数指针初始化
uint8_t EV_ETH_User_Recv_Init(uint8_t (*Recv_Callback)(uint8_t *Data,int Len,uint32_t S_Addr))
{
    if(Recv_Callback == NULL)
    {
        return 1;
    }

    user_eth_normal_recv = Recv_Callback;
    return 0;
}

//ETH用户发送函数
int EV_ETH_User_Send(uint8_t *Data,int Len,uint32_t S_Addr)
{
    if(Data == NULL)
    {
        return -1;
    }
    int res = 1;
    char user_ip[20] = {0};
    sprintf(user_ip,"%s",(char *)inet_ntoa(S_Addr));
    switch(g_net_user_sta)
    {
        case WIFI_NETWORK:
            res = (int)wifi_send_user(Data, (uint16_t)Len,1,user_ip);
            break;
        case LWIP_NETWORK:
        case LWIP_AND_WIFI:
            res = lwip_udp_normal_send(Data,Len,S_Addr);
            break;
        default:
            break;
    }
    return res;
}


//网络扫描切换任务
#define ETH_USER_TASK_PRIO ETH_USER_TASK_PRIO_NUM
#define ETH_USER_STK_SIZE 1024 
uint32_t *ETH_USER_TASK_STK; 
void eth_user_task(void *pdata)
{
    uint8_t network_before_sta=g_net_user_sta;

    while(1)
    {
        if(wifi_Init_sta==2&&dhcpstatus==2)
        {
            g_net_user_sta = LWIP_AND_WIFI;
        }else if(wifi_Init_sta==2){
            g_net_user_sta = WIFI_NETWORK;
        }else if(dhcpstatus==2){
            g_net_user_sta = LWIP_NETWORK;
        }else{
            g_net_user_sta = NO_NETWORK;
        }
        
        if(network_before_sta != g_net_user_sta)
        {
            switch(g_net_user_sta)
            {
                case NO_NETWORK:
                    LOG(LEVEL_INFO,"NO_NETWORK\n");
                    wifi_Init_sta=0;            //wifi标志位清空
                    memset(&Net_Local_Info,0,sizeof(Net_Local_Info));
                    OSTaskSuspend(UDP_BCAST_PRIO_NUM);
                    OSTaskSuspend(UDP_NORMAL_PRIO_NUM);
                    OSTaskSuspend(M2_TASK_BCAST_PRIO_NUM);
                    OSTaskSuspend(LWIP_UPGRADE_PRIO_NUM);
                    OSTaskSuspend(WIFI_TASK_UPDATE_PRIO_NUM);
                    break;
                case WIFI_NETWORK:
                    LOG(LEVEL_INFO,"WIFI_NETWORK\n");
                    memset(&Net_Local_Info,0,sizeof(Net_Local_Info));
                    memcpy(&Net_Local_Info,&WiFi_Info,sizeof(Net_Local_Info));
                    OSTaskSuspend(UDP_BCAST_PRIO_NUM);
                    OSTaskSuspend(UDP_NORMAL_PRIO_NUM);
                    OSTaskResume(M2_TASK_BCAST_PRIO_NUM);
                    OSTaskSuspend(LWIP_UPGRADE_PRIO_NUM);
                    OSTaskResume(WIFI_TASK_UPDATE_PRIO_NUM);
                    break;
                case LWIP_NETWORK:
                    LOG(LEVEL_INFO,"LWIP_NETWORK\n");
                case LWIP_AND_WIFI:
                    LOG(LEVEL_INFO,"LWIP_AND_WIFI\n");
                    if(network_before_sta == NO_NETWORK
                        ||network_before_sta == WIFI_NETWORK)
                        {
                            wifi_Init_sta=0;            //wifi标志位清空
                            udp_LwipInfo_init();		//UDP信息初始化
                            memset(&Net_Local_Info,0,sizeof(Net_Local_Info));
                            memcpy(&Net_Local_Info,&Lwip_Info,sizeof(Net_Local_Info));   
                            OSTaskResume(UDP_BCAST_PRIO_NUM);
                            OSTaskResume(UDP_NORMAL_PRIO_NUM);
                            OSTaskSuspend(M2_TASK_BCAST_PRIO_NUM);
                            OSTaskResume(LWIP_UPGRADE_PRIO_NUM);
                            OSTaskSuspend(WIFI_TASK_UPDATE_PRIO_NUM);
                        }
                    break;
                default:
                    break;
            }
            network_before_sta = g_net_user_sta;
        }
        OSTimeDly(10000);
    }
}



//网络初始化
void ETH_Init(void)
{
    uint32_t cpu_sr;

    lwip_user_common_init();
    EV_WIFI_SET_SSID("Jump2G4", "18515089819");  
    EV_Add_Updata_Info_Manage(1,"172.16.7.11:8000/upgrade/stm32f429/beta/public","filelist",8000);
    OS_ENTER_CRITICAL();	//关中断
    //任务创建
    
#if LWIP_DHCP
    lwip_comm_dhcp_creat(); //以太网扫描任务创建
#endif
    lwip_udp_creat();       //以太网数据传输任务创建
    eth_upgrade_creat();

    ETH_USER_TASK_STK = mymalloc(SRAMEX, ETH_USER_STK_SIZE* 4);
    OSTaskCreate(eth_user_task, "eth_user_task", 0, (uint32_t *)&ETH_USER_TASK_STK[ETH_USER_STK_SIZE - 1], ETH_USER_TASK_PRIO);
    OS_EXIT_CRITICAL();
    
}



