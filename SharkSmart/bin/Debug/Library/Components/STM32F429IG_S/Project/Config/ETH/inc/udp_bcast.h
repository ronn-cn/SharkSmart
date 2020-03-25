#ifndef __UDP_BCAST_H
#define __UDP_BCAST_H
#include "sys.h"


typedef struct
{
	uint8_t	net_sta;
	uint32_t ipv4;
	uint8_t mac[6];
	uint8_t name_len;
	uint8_t name_buf[30];
	
}DeviceInfoStruct;


extern uint8_t udp_bcast_send_flag;
extern DeviceInfoStruct DeviceInfoList[10];
extern uint8_t DeviceInfoCnt;
extern DeviceInfoStruct Lwip_Info;

void udp_LwipInfo_init(void);
void lwip_udp_creat(void);
int lwip_udp_normal_send(uint8_t *udp_send,int udp_len,uint32_t udp_sin_addr);

#endif

