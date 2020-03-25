#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H 
#include "lan8720.h" 
#include "sys.h"	   
 #include "lwip/api.h"
#include "lwip/lwip_sys.h"
#include "lwip/ip.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#define LWIP_MAX_DHCP_TRIES		4   //DHCP服务器最大重试次数

extern struct netif lwip_netif;				//定义一个全局的网络接口
extern uint8_t g_local_mac[6];
extern vu8 dhcpstatus;

void lwip_user_common_init(void);
void lwip_pkt_handle(void);
uint8_t lwip_comm_mem_malloc(void);
void lwip_comm_mem_free(void);
uint8_t lwip_comm_init(void);
void lwip_comm_dhcp_creat(void);
void lwip_comm_dhcp_delete(void);
void lwip_comm_destroy(void);
void lwip_comm_delete_next_timeout(void);

void mac_get(unsigned int * mac);

void socketaddr_set(struct sockaddr *addr, char *ip, uint16_t *port);
void socketaddr_get(struct sockaddr *addr, char *ip, uint16_t *port);
#endif













