#ifndef __ESP8266_H_
#define __ESP8266_H_
#include "sys.h"
#include "usart.h"
#include "udp_bcast.h"

#define WIFI_UDP_BCAST_PORT     9999
#define WIFI_UDP_NORMAL_PORT    10000
#define UDP_MODE                2

#define ESP_RX_STA_RESET    UART7_RX_STA=0
#define ESP_RX_STA_GET      (UART7_RX_STA&0x8000)?1:0


uint8_t wifi_init(void);
uint8_t wifi_udp_sta_config(void);
uint8_t wifi_udp_set(void);
uint8_t wifi_udp_set_ip(void);
uint8_t wifi_send_user(uint8_t *data, uint16_t length,uint8_t link_id,char *send_ip);
int wifi_recv_user(char *data,uint16_t data_len,uint32_t *recv_ip,uint16_t *recv_port);
uint8_t wifi_udp_bcast(void);
uint8_t wifi_udp_check_IP(uint8_t *check_buf,uint16_t check_len,uint16_t check_port);
uint8_t wifi_tcp_DNS(char *tcp_dns,char *tcp_ip);
uint8_t wifi_tcp_connect(char *tcp_ip,uint16_t tcp_port);
uint8_t wifi_connect_transparent(char *tcp_ip,uint16_t tcp_port);
uint8_t wifi_disconnect_transparent(void);
uint8_t wifi_send_transparent(uint8_t *data, uint16_t length);

// uint8_t wifi_tcp_client_set(char *tcp_ip, uint16_t port);

extern uint16_t UART7_RX_STA;
extern uint8_t g_wifi_recv_data_sta;

extern DeviceInfoStruct WiFi_Info;
uint8_t EV_WIFI_SET_SSID(char *WiFi_Name,char *Password);

#endif