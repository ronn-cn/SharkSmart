#ifndef __ETH_UPGRADE_H
#define __ETH_UPGRADE_H
#include "sys.h"


#define HTTP_BUF_MAX        1024
#define FILE_INSTANCE_NAME     "STM32F429"
#define URL_CHOOSE_MAX      5

struct http_url_info_t
{
    uint8_t sta;
    char host_name[50];
    uint16_t port;
    char url_1[50];
    char url_2[50];
    char url_3[50];
    char url_4[50];
    
};

struct http_message_app_info_t
{
    char instance_buf[20];
    char time_buf[30];
    char hard_v_buf[10];
    char soft_v_buf[10];
    char path_buf[50];
    char file_buf[50];
    char force_buf[10];
    
    int file_length;

    uint8_t recv_ok_flag;
};



struct http_message_resources_info_t
{
    char instance_buf[20];
    char time_buf[30];
    char hard_v_buf[10];
    char resource_v_buf[10];
    char path_buf[50];
    char files_buf[250];
    char reboot_buf[10];
    
    char file_buf[10][20];
    uint8_t files_cnt;

    char *save_buf_p[10];

    int file_length[10];
    uint8_t recv_ok_flag;
};


void eth_upgrade_creat(void);
uint8_t EV_Add_Updata_Info_Manage(char *Path,char *f_Name,uint16_t Port);

#endif

