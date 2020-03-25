#ifndef __ETH_USER_H
#define __ETH_USER_H
#include "sys.h"
#include "udp_bcast.h"

typedef enum {
	LWIP_STA 	= 0x01,
	WIFI_STA 	= 0x02
}NETWORK_STATE;

typedef enum{
    NO_NETWORK      = 0,
    LWIP_NETWORK    = 1,
    WIFI_NETWORK    = 2,
    LWIP_AND_WIFI   = 3
}ETH_USER_STATUS;




extern uint8_t g_net_user_sta;      //网络状态
extern DeviceInfoStruct Net_Local_Info;

void ETH_Init(void);
int EV_ETH_User_Send(uint8_t *Data,int Len,uint32_t S_Addr);
uint8_t EV_ETH_User_Recv_Init(uint8_t (*Recv_Callback)(uint8_t *Data,int Len,uint32_t S_Addr));
#endif