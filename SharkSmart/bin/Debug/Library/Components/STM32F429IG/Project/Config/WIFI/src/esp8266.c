#include "sys.h"
#include "usart.h"
#include "log.h"
#include "esp8266.h"
#include "stdlib.h"
#include "stdarg.h"
#include "timer.h"
#include "malloc.h"
#include "string.h"
#include "rtos.h"
#include "gpio.h"
#include "udp_bcast.h"
#include "lwip/inet.h"
#include "eth_user.h"
/*

    使用到的资源:
                    TIM7
                    UART7

*/

#define UART7_MAX_RECV_LEN 1500         //最大接收缓存字节数
#define UART7_MAX_SEND_LEN 500          //最大发送缓存字节数
#define UART7_RX_EN 1                   //0,不接收;1,接收.

extern TIM_HandleTypeDef TIM7_Handler;
uint8_t *UART7_RX_BUF;                  //接收缓冲,最大UART7_MAX_RECV_LEN个字节.
uint8_t *UART7_TX_BUF;                  //发送缓冲,最大UART7_MAX_SEND_LEN字节
uint16_t UART7_RX_STA = 0;
static uint8_t esp_time_cnt = 0;
char wifi_ssid[50];                     //无线网名字
char wifi_password[50];                 //无线网密码
uint8_t wifi_sending_cmd_sta = 0;           //wifi正在发送
uint8_t g_wifi_recv_data_sta=0;         //WIFI接收数据标志
uint8_t *WIFI_RX_BUF;                   //wifi接收数据缓冲区
DeviceInfoStruct WiFi_Info;             //wifi自身设备信息
uint8_t transparent_start=0;         	//透传开始接收标志位
uint8_t *transparent_rx_buf;            //透传缓冲区指针
uint8_t *transparent_rx_buf_last;       //透传缓冲区末尾指针
uint8_t *transparent_rx_buf_cnt;        //透传缓冲区数据当前指针


static void esp_delay_ms(uint32_t nms)
{
    if (OS_Running == FALSE)
    {
        uint32_t tickstart = 0;
        tickstart = HAL_GetTick();
        while ((HAL_GetTick() - tickstart) < nms)
        {
        }
    }
    else
    {
        OSTimeDly(nms);
    }
}
// static uint8_t find_strstr(char * str1, char * str2, uint16_t len)
// {
//     uint16_t str2_len = strlen(str2);
//     uint16_t max_comp_len = 0;
//     if(len < str2_len)
//         return 0;
//         max_comp_len = len - str2_len;
//     for(uint16_t i = 0; i < max_comp_len; i++)
//     {
//         if(memcmp(str1 + i, str2, str2_len) == 0)
//             return 1;
//     }
//     return 0;
// }

//确保一次发送数据不超过UART7_MAX_SEND_LEN字节
void u7_printf(char *fmt, ...)
{
    uint16_t i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)UART7_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((char *)UART7_TX_BUF); //此次发送数据的长度

    for (j = 0; j < i; j++) //循环发送数据
    {
        while ((UART7->SR & 0X40) == 0)
            ; //循环发送,直到发送完毕
        UART7->DR = UART7_TX_BUF[j];
    }
    //EV_UART_Send(UART7, UART7_TX_BUF, i);
}

//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
char *wifi_check_cmd(char *str)
{
    char *strx = 0;
    if (UART7_RX_STA & 0X8000) //接收到一次数据了
    {
        strx = strstr((const char *)UART7_RX_BUF, (const char *)str);
    }
    return (char *)strx;
}

//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:1ms)
//recv_cmd:获取应答的字符串，为空不获取应答字符串
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
uint8_t wifi_send_cmd(char *cmd, char *ack, uint16_t waittime,uint8_t *recv_cmd)
{
    uint8_t res = 1;
    UART7_RX_STA = 0;
    wifi_sending_cmd_sta = 1;
    u7_printf("%s\r\n", cmd); //发送命令
    if (ack && waittime)      //需要等待应答
    {
        while (waittime--) //等待倒计时
        {
            esp_delay_ms(1);
            if (UART7_RX_STA & 0X8000) //接收到期待的应答结果
            {
                if (wifi_check_cmd(ack))
                {
                    if(recv_cmd!=NULL)
                    {
                        strcpy((char *)recv_cmd,(char *)UART7_RX_BUF);  //获取应答数据
                    }
                    res=0;
                    UART7_RX_STA = 0;
                    break; //得到有效数据
                }
                UART7_RX_STA = 0;
            }
        }
    }
    wifi_sending_cmd_sta = 0;
    return res;
}

//向ATK-ESP8266发送指定数据
//data:发送的数据(不需要添加回车了)
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:1ms)
//返回值:0,发送成功(得到了期待的应答结果)
//      1，失败；
uint8_t wifi_send_data(char *data, uint16_t data_len,char *ack, uint16_t waittime)
{
    uint8_t res = 1;
    wifi_sending_cmd_sta = 1;
    UART7_RX_STA = 0;
    EV_UART_Send(UART7, (uint8_t *)data, data_len);  //发送数据
    if (ack && waittime)   //需要等待应答
    {
        while (waittime--) //等待倒计时
        {
            esp_delay_ms(1);
            if (UART7_RX_STA & 0X8000) //接收到期待的应答结果
            {
                if (wifi_check_cmd(ack))
                {
                    res=0;
                    UART7_RX_STA = 0;
                    break; //得到有效数据
                }  
                UART7_RX_STA = 0;
            }
        }
    }
    wifi_sending_cmd_sta = 0;
    return res;
}

//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
uint8_t esp8266_quit_trans(void)
{
    EV_UART_Send(UART7, (uint8_t *)"+", 1);
    esp_delay_ms(15); //大于串口组帧时间(10ms)
    EV_UART_Send(UART7, (uint8_t *)"+", 1);
    esp_delay_ms(15); //大于串口组帧时间(10ms)
    EV_UART_Send(UART7, (uint8_t *)"+", 1);
    esp_delay_ms(500);                           //等待500ms
    return wifi_send_cmd("AT", "OK", 200,NULL); //退出透传判断.
}

// //获取ATK-ESP8266模块的连接状态
// //返回值:0,未连接;1,连接成功.
// uint8_t esp8266_consta_check(void)
// {
//     char *p;
//     uint8_t res;
//     if (esp8266_quit_trans())
//         return 0;                              //退出透传
//     esp8266_send_cmd("AT+CIPSTATUS", ":", 50,NULL); //发送AT+CIPSTATUS指令,查询连接状态
//     p = esp8266_check_cmd("+CIPSTATUS:");
//     res = (uint8_t)*p; //得到连接状态
//     return res;
// }

//获取自身 ip地址 MAC地址
//ipbuf:ip地址输出缓存区
//macbuf：mac地址输出缓存区
//返回值：0：成功；1失败；
uint8_t wifi_get_wanip(char *ipbuf,char *macbuf)
{
    char recv_buf[100]={0};          //接收应答缓冲区
    char *p, *p1;
    if (wifi_send_cmd("AT+CIFSR", "OK", 100,(uint8_t *)recv_buf)) //获取WAN IP地址失败
    {
        return 1;
        ipbuf[0] = 0;
    }
    
    p = strstr(recv_buf,"MAC");
    p1 = strstr((const char *)(p + 5), "\"");
    *p1 = 0;
    sprintf((char *)macbuf, "%s", p + 5);
    p = strstr(recv_buf,"\"");
    p1 = strstr((const char *)(p + 1), "\"");
    *p1 = 0;
    sprintf((char *)ipbuf, "%s", p + 1);

    return 0;
}



// uint8_t wifi_tcp_client_set(char *tcp_ip, uint16_t port)
// {
//     uint8_t err_cnt = 0;
//     char ip_buf[20];
//     char p[50];
//     sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%d", tcp_ip, port); //配置目标TCP服务器
//     err_cnt = 0;
//     while (esp8266_send_cmd(p, "CONNECT", 200,NULL))
//     {
//         LOG(LEVEL_INFO, "link TCP Server fail\n"); //连接失败
//         esp_delay_ms(1000);
//         if (err_cnt++ > 10)
//             return 1;
//     }
//     esp8266_get_wanip(ip_buf); //服务器模式,获取WAN IP

//     sprintf((char *)p, "IPaddr:%s", ip_buf);
//     LOG(LEVEL_INFO, "%s\n", p); //显示IP地址和端口
//     err_cnt = 0;
//     while (esp8266_send_cmd("AT+CIPMODE=1", "OK", 200,NULL)) //传输模式为：透传
//     {
//         LOG(LEVEL_INFO, "Trans fail \n");
//         esp_delay_ms(100);
//         if (err_cnt++ > 10)
//             return 1;
//     }

//     esp8266_quit_trans();
//     esp8266_send_cmd("AT+CIPSEND", "OK", 20,NULL); //开始透传
//     // config_ok_flag = 1;
//     return 0;
// }

//添加UDP的IP连接
//UDP_mode:UDP模式
//返回值：0，成功；
//       1，参数错误；
//       2，添加连接失败；
uint8_t wifi_udp_set_ip(void)
{
    char p[50];

    //广播IP
    sprintf((char *)p, "AT+CIPSTART=0,\"UDP\",\"255.255.255.255\",%d,%d,%d",WIFI_UDP_BCAST_PORT,WIFI_UDP_BCAST_PORT,UDP_MODE);
    if(wifi_send_cmd(p, "OK", 200,NULL))
    {
        LOG(LEVEL_INFO, "add UDP bcast ip_addr fail\n"); //连接失败
        return 2;
    }

    //普通传输IP
    memset(p,0,sizeof(p));
    sprintf((char *)p, "AT+CIPSTART=1,\"UDP\",\"0.0.0.0\",%d,%d,%d",WIFI_UDP_NORMAL_PORT,WIFI_UDP_NORMAL_PORT,UDP_MODE);
    if(wifi_send_cmd(p, "OK", 200,NULL))
    {
        LOG(LEVEL_INFO, "add UDP normal ip_addr fail\n"); //连接失败
        return 2;
    }

    return 0;
}

uint8_t wifi_tcp_DNS(char *tcp_dns,char *tcp_ip)
{
    char p[50];
    uint8_t recv_buf[50] = {0};

    if(tcp_dns == NULL||tcp_ip == NULL)
    {
        return 1;
    }

    LOG(LEVEL_INFO,"%s\n",tcp_dns);
    sprintf((char *)p, "AT+CIPDOMAIN=\"%s\"",tcp_dns);
    wifi_send_cmd(p, "OK", 200,recv_buf);
    if(recv_buf!= NULL)
    {
        sscanf((char *)recv_buf,"%*[^:]:%s",tcp_ip);
        LOG(LEVEL_INFO,"recv_tcp_ip:%s\n",tcp_ip);
    }else{
        return 2;
    }
    
    return 0;
}

//TCP连接
uint8_t wifi_tcp_connect(char *tcp_ip,uint16_t tcp_port)
{
    char p[50];
    
    if(tcp_ip == NULL)
    {
        return 1;
    }

    memset(p,0,sizeof(p));
    sprintf((char *)p, "AT+CIPSTART=2,\"TCP\",\"%s\",%d",tcp_ip,tcp_port);
    if(wifi_send_cmd(p, "OK", 200,NULL))
    {
        return 2;
    }
    return 0;
}

//广播自身信息
uint8_t wifi_udp_bcast(void)
{
    char send_buf[100];
    sprintf(send_buf,"YQADV");
    memcpy(&send_buf[5],&Net_Local_Info,sizeof(Net_Local_Info));
    return wifi_send_user((uint8_t *)send_buf,sizeof(Net_Local_Info)+5,0,NULL);
}

//接收IP处理函数
uint8_t wifi_udp_check_IP(uint8_t *check_buf,uint16_t check_len,uint16_t check_port)
{
    uint8_t i;
    uint8_t check_ok=0;          //收到数据为设备信息标志
    char send_buff[100]={0};
    char ip_buf[20] = {0};
    DeviceInfoStruct recv_info;

    if(check_buf == NULL)
    {
        return 1;
    }

    if(strstr((char *)check_buf,"YQADV"))
    {
        if(check_port == 9999)
        {
            LOG(LEVEL_INFO,"recv_bcast\n");
            memcpy(&recv_info,check_buf+5,sizeof(recv_info));
            sprintf(ip_buf,"%s",inet_ntoa(recv_info.ipv4));
            sprintf(send_buff,"YQADV");
            memcpy(&send_buff[5],&Net_Local_Info,sizeof(Net_Local_Info));
            wifi_send_user((uint8_t *)send_buff,sizeof(Net_Local_Info)+5,1,ip_buf);
            check_ok=1;

        }else{
            memcpy(&recv_info,check_buf+5,sizeof(recv_info));
            check_ok=1;
        }

    }

    if(check_ok)
    {
        if(DeviceInfoCnt == 0)
        {
            memcpy(&DeviceInfoList[DeviceInfoCnt++], &recv_info, sizeof(recv_info));
        }
        else
        {
            for(i = 0; i < DeviceInfoCnt; i++)
            {
                if(memcmp(DeviceInfoList[i].mac, recv_info.mac, 6) == 0)
                {
                    break;
                }
            }
            if(i == DeviceInfoCnt)
            {
                memcpy(&DeviceInfoList[DeviceInfoCnt++], &recv_info, sizeof(recv_info));
            }
        }
        LOG(LEVEL_INFO, "recv len: %d\n", recv_info.name_len);
        LOG(LEVEL_INFO, "Recv Name: %s\n", recv_info.name_buf);
        LOG(LEVEL_INFO, "Recv IP:   %s\n", inet_ntoa(recv_info.ipv4));
        LOG(LEVEL_INFO, "Recv MAC:  %02X:%02X:%02X:%02X:%02X:%02X\n\n", recv_info.mac[0], 
                                                                        recv_info.mac[1], 
                                                                        recv_info.mac[2], 
                                                                        recv_info.mac[3], 
                                                                        recv_info.mac[4], 
                                                                        recv_info.mac[5]); 
        return 0;
    }

    return 2;
 
}

// uint8_t esp8266_get_time(__esp_time_t * time)
// {
//     uint8_t err_cnt = 0;
//     char p[50];
//     uint32_t second;
//     uint8_t send_ntp_buf[48] = {
//         0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd6, 0x6d, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00};
//     err_cnt = 0;
//     while(esp8266_quit_trans())   //推出透传
//     {
//         esp_delay_ms(1000);
//         if (err_cnt++ > 10)
//             return 1;
//     }

//     sprintf((char *)p, "AT+CIPSTART=\"UDP\",\"%s\",%d", "1.cn.pool.ntp.org", 123); //连接到时间服务器
//     err_cnt = 0;
//     while (esp8266_send_cmd(p, "CONNECT", 200))
//     {
//         LOG(LEVEL_INFO, "Connect to Timer server fail\n"); //连接失败
//         esp_delay_ms(1000);
//         if (err_cnt++ > 10)
//             return 1;
//     }

//     //开始透传
//     esp8266_send_cmd("AT+CIPMODE=1", "OK", 50);
//     esp8266_send_cmd("AT+CIPSEND", "OK", 50);
//     UART7_RX_STA = 0;
//     //UART7_RX_STA = 0;
//     esp8266_send(send_ntp_buf, 48);
//     esp_delay_ms(500);

//     if((UART7_RX_STA & 0x7FFF) != 48)
//     {
//         return 2;
//     }

//     second = 0;
//     second |= UART7_RX_BUF[40] << 24;
//     second |= UART7_RX_BUF[41] << 16;
//     second |= UART7_RX_BUF[42] << 8;
//     second |= UART7_RX_BUF[43];

//     second += 3600 * 8;  //东8区

//     time->sec = second % 60;
//     time->min = (second / 60) % 60;
//     time->hour = (second / 3600) % 24;

//     time->weekday = (second / 86400) % 7 + 1;

//     uint16_t y = 1900;
//     do{
//         unsigned long ys;
//         if(( y % 4 == 0 && y % 100 != 0) || y % 400 == 0){
//         ys = 31622400L; //31622400 = 366 * 24 * 3600;
//         }else{
//         ys = 31536000L;  // 31536000 = 365 * 24 * 3600;
//         }
//         if(second < ys){
//         break;
//         }else{
//         second -= ys;
//         y++;
//         }
//     }while(1);
//     time->year = y;

//     uint8_t m;
//     uint8_t mons[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
//     if((y % 4 == 0 && y % 100 != 0) || y % 400 == 0){
//         mons[1] = 29;
//     }
//     for(m=0; m < 12; m ++){
//         if(second < mons[m] * 86400L){
//         break;
//         }else{
//         second -= mons[m] * 86400L;
//         }
//     }
//     time->month = m + 1;
//     time->date = second / 86400L + 1;

//     UART7_RX_STA = 0;
//     esp8266_quit_trans();   //推出透传

//     //esp8266_send_cmd("AT+CIPCLOSE", "OK", 50);
//     esp_delay_ms(100);
//     return 0;
// }


//打开透传模式
uint8_t wifi_connect_transparent(char *tcp_ip,uint16_t tcp_port)
{
    char p[50];

    if(tcp_ip == NULL)
    {
        return 1;
    }
    if(transparent_start == 1)
    {
        return 0;
    }
    
    transparent_rx_buf = mymalloc(SRAMEX, 1024*200);
    transparent_rx_buf_last=&transparent_rx_buf[1024*200-1];
    transparent_rx_buf_cnt=transparent_rx_buf;

    wifi_send_cmd("AT+CIPCLOSE=0", "OK", 50,NULL);
    wifi_send_cmd("AT+CIPCLOSE=1", "OK", 50,NULL);
    wifi_send_cmd("AT+CIPCLOSE=2", "OK", 50,NULL);

    wifi_send_cmd("AT+CIPMUX=0", "OK", 200,NULL);    //设置单连接模式
    wifi_send_cmd("AT+CIPMODE=1", "OK", 200,NULL);   //设置透传模式
   
    memset(p,0,sizeof(p));
    sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%d",tcp_ip,tcp_port);
    if(wifi_send_cmd(p, "OK", 500,NULL))
    {
        LOG(LEVEL_INFO, "CIPSTART fail \n");
        return 2;
    }

    if(wifi_send_cmd("AT+CIPSEND", ">", 200,NULL))
    {
        LOG(LEVEL_INFO, "CIPSEND fail \n");
        return 3;
    }
    
    transparent_start=1;         //透传开始接收标志
    return 0;
}

//关闭透传模式
uint8_t wifi_disconnect_transparent(void)
{
    uint8_t err_cnt = 0;
    char p[50];

    if(transparent_start != 1)
    {
        return 0;
    }
    transparent_start=0;             //清空透传标志
    myfree(SRAMEX,transparent_rx_buf);

    esp8266_quit_trans();
    while(wifi_send_cmd("AT","OK",200,NULL))
    {
        esp_delay_ms(100);
        if(err_cnt++ >= 2)
        {
            LOG(LEVEL_INFO, "quit_trans fail \n");
            return 1;
        }
    }
    
    wifi_send_cmd("AT+CIPCLOSE", "OK", 50,NULL);
    wifi_send_cmd("AT+CIPMODE=0", "OK", 200,NULL);   //设置非透传模式
    wifi_send_cmd("AT+CIPMUX=1", "OK", 50,NULL);    //设置多连接模式
  
    sprintf((char *)p, "AT+CIPSTART=0,\"UDP\",\"255.255.255.255\",%d,%d,%d",WIFI_UDP_BCAST_PORT,WIFI_UDP_BCAST_PORT,UDP_MODE);
    if(wifi_send_cmd(p, "OK", 200,NULL))
    {
        LOG(LEVEL_INFO, "add UDP bcast ip_addr fail\n"); //连接失败
        return 3;
    }

    //普通传输IP
    memset(p,0,sizeof(p));
    sprintf((char *)p, "AT+CIPSTART=1,\"UDP\",\"0.0.0.0\",%d,%d,%d",WIFI_UDP_NORMAL_PORT,WIFI_UDP_NORMAL_PORT,UDP_MODE);
    if(wifi_send_cmd(p, "OK", 200,NULL))
    {
        LOG(LEVEL_INFO, "add UDP normal ip_addr fail\n"); //连接失败
        return 4;
    }


    LOG(LEVEL_INFO,"wifi_disconnect_trans ok\n");
    return 0;
}

//透传发送数据
uint8_t wifi_send_transparent(uint8_t *data, uint16_t length)
{
    return EV_UART_Send(UART7, data, length);  //发送数据
}


//UART7串口接收数据回调函数
uint8_t esp_rx_calll_back(uint8_t *data, uint8_t len)
{
    if(transparent_start)
    {
        *transparent_rx_buf_cnt=data[0];
        if(transparent_rx_buf_cnt==transparent_rx_buf_last)
        {
            transparent_rx_buf_cnt=transparent_rx_buf;
        }else{
            transparent_rx_buf_cnt++;
        }
        return 0;
    }

    if ((UART7_RX_STA & (1 << 15)) == 0) //接收完的一批数据,还没有被处理,则不再接收其他数据
    {
        if (UART7_RX_STA < UART7_MAX_RECV_LEN) //还可以接收数据
        {
            esp_time_cnt = 0;
            if (UART7_RX_STA == 0) //使能定时器7的中断
            {
                EV_TIM_Start(TIM7);
            }
            UART7_RX_BUF[UART7_RX_STA++] = data[0]; //记录接收到的值
        }
        else
        {
            UART7_RX_STA |= 1 << 15; //强制标记接收完成
            //判断接收到的数据是否为应答
            if(g_wifi_recv_data_sta==0&&strstr((const char *)UART7_RX_BUF,"+IPD")!=NULL)
            {
                uint8_t *str_ipd;
                uint16_t rx_len;
                str_ipd = (uint8_t *)strstr((const char *)UART7_RX_BUF,"+IPD");
                memset(WIFI_RX_BUF,0,UART7_MAX_RECV_LEN);
                rx_len = (UART7_RX_STA & 0X7FFF) - (str_ipd - UART7_RX_BUF);
                memcpy(WIFI_RX_BUF,str_ipd,rx_len);
                g_wifi_recv_data_sta=1; 
                UART7_RX_STA = 0;  
            }
        }
    }
    return 0;
}
//esp时钟回调函数
void esp_timer_callback(void)
{
    esp_time_cnt ++;
    
    if (esp_time_cnt >= 10)
    {
        UART7_RX_BUF[UART7_RX_STA & 0X7FFF] = 0;
        UART7_RX_STA |= 1 << 15; //标记接收完成
        //判断接收到的数据是否为应答
        if(g_wifi_recv_data_sta==0&&strstr((const char *)UART7_RX_BUF,"+IPD")!=NULL)
        {
            char *str_ipd;
            uint16_t rx_len;
            str_ipd = strstr((const char *)UART7_RX_BUF,"+IPD");
            memset(WIFI_RX_BUF,0,UART7_MAX_RECV_LEN);
            rx_len = (UART7_RX_STA & 0X7FFF) - (str_ipd - (char *)UART7_RX_BUF);    //去掉接收数据标志之前的数据
            memcpy(WIFI_RX_BUF,str_ipd,rx_len);
            g_wifi_recv_data_sta=1;
            UART7_RX_STA = 0;  
        }
        EV_TIM_Stop(TIM7);
    }
}

//wifi初始化
uint8_t wifi_init(void)
{
    UART7_RX_BUF = mymalloc(SRAMEX, UART7_MAX_RECV_LEN);
    UART7_TX_BUF = mymalloc(SRAMEX, UART7_MAX_SEND_LEN);
    WIFI_RX_BUF  = mymalloc(SRAMEX, UART7_MAX_RECV_LEN);


    EV_UART_Init(UART7, PF6, PF7, 115200, esp_rx_calll_back);

    EV_TIM_Init(TIM7, 1, esp_timer_callback);

    UART7_RX_STA = 0;

    //reset
    EV_GPIO_Init(PA5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
    //en
    EV_GPIO_Init(PH4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
    EV_GPIO_Set(PH4, SET);
        

    EV_GPIO_Set(PA5, SET);
    esp_delay_ms(100);
    EV_GPIO_Set(PA5, RESET);
    esp_delay_ms(200);
    EV_GPIO_Set(PA5, SET);

    return 0;
}


//配置ESP8266位sta模式，并连接到路由器
//wifi_ssid:ssid字符串；
//wifi_password：密钥；
//返回值：0：成功；
//      其他：失败；
uint8_t wifi_udp_sta_config(void)
{
    uint8_t err_cnt = 0;
    char mac_buf[20];
    char ip_addr[20];
    char p[50];

    transparent_start=0;             //清空透传标志
    myfree(SRAMEX,transparent_rx_buf);

    esp8266_quit_trans();
    while(wifi_send_cmd("AT","OK",200,NULL))
    {
        esp_delay_ms(100);
        if(err_cnt++ >= 2)
        {
            LOG(LEVEL_INFO, "config quit_trans fail \n");
            return 1;
        }
    }
    wifi_send_cmd("AT+RST", "OK", 100,NULL);
    esp_delay_ms(10000);
    
    err_cnt = 0;
    while (wifi_send_cmd("ATE0", "OK", 100,NULL))
    {
        if (err_cnt++ >= 2)
            return 1;
    }
    
    err_cnt = 0;
    while (wifi_send_cmd("AT+CWMODE=1", "OK", 200,NULL))    //设置WIFI STA模式
    {
        esp_delay_ms(100);
        if (err_cnt++ >= 2)
        {
            LOG(LEVEL_INFO, "CWMODE fail \n");
            return 1;
        }   
    }

    //设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!!
    sprintf((char *)p, "AT+CWJAP=\"%s\",\"%s\"", wifi_ssid, wifi_password); //设置无线参数:ssid,密码
    if(wifi_send_cmd(p, "OK", 10000,NULL))
    {
        LOG(LEVEL_INFO, "Can't get ip\n");
        return 2;       
    }; //连接目标路由器,并且获得IP
    esp_delay_ms(300);
    //读取ip地址
    err_cnt = 0;
    while (wifi_get_wanip(ip_addr,mac_buf))
    {
        if (err_cnt++ >= 2)
            return 3;       
    };

    int mac_temp[6];
    sscanf(mac_buf,"%x:%x:%x:%x:%x:%x", &mac_temp[0],
                                        &mac_temp[1],
                                        &mac_temp[2],
                                        &mac_temp[3],
                                        &mac_temp[4],
                                        &mac_temp[5]);
    WiFi_Info.mac[0] = mac_temp[0] & 0xFF;
    WiFi_Info.mac[1] = mac_temp[1] & 0xFF;
    WiFi_Info.mac[2] = mac_temp[2] & 0xFF;
    WiFi_Info.mac[3] = mac_temp[3] & 0xFF;
    WiFi_Info.mac[4] = mac_temp[4] & 0xFF;
    WiFi_Info.mac[5] = mac_temp[5] & 0xFF;

    WiFi_Info.ipv4 = inet_addr(ip_addr);
    LOG(LEVEL_INFO, "%x\n", WiFi_Info.ipv4); //显示IP地址和端口
    LOG(LEVEL_INFO,"MAC:%x,%x,%x,%x,%x,%x\n",   WiFi_Info.mac[0],
                                                WiFi_Info.mac[1],
                                                WiFi_Info.mac[2],
                                                WiFi_Info.mac[3],
                                                WiFi_Info.mac[4],
                                                WiFi_Info.mac[5]);


    return 0;
}


//设置UDP连接
//返回值：0：成功；
//       1：模式配置错误；
//       2：广播IP添加错误；
uint8_t wifi_udp_set(void)
{
    uint8_t err_cnt = 0;
    
    wifi_send_cmd("AT+CIPMODE=0", "OK", 200,NULL);

    err_cnt = 0;
    while (wifi_send_cmd("AT+CIPMUX=1", "OK", 50,NULL))   //设置多连接模式
    {
        LOG(LEVEL_INFO, "CIPMUX fail \n");
        esp_delay_ms(100);
        if (err_cnt++ >= 2)
            return 1;
    }
    
    err_cnt = 0;
    while (wifi_send_cmd("AT+CIPDINFO=1", "OK", 50,NULL))   //设置显示接收数据ip
    {
        LOG(LEVEL_INFO, "CIPDINFO fail \n");
        esp_delay_ms(100);
        if (err_cnt++ >= 2)
            return 1;
    }

    if(wifi_udp_set_ip())             //添加连接
    {
        LOG(LEVEL_INFO, "UDP link fail\n");     //连接失败
        return 2;
    }
    
    //添加自身信息
    LOG(LEVEL_INFO,"INFO_LEN:%d\n",sizeof(WiFi_Info));
    strcpy((char *)WiFi_Info.name_buf, "STM32F429 V3.3");
    WiFi_Info.name_len = 14;
    WiFi_Info.net_sta = WIFI_STA;
    LOG(LEVEL_INFO, "wifi len: %d\n", WiFi_Info.name_len);
    LOG(LEVEL_INFO, "wifi Name: %s\n", WiFi_Info.name_buf);
    LOG(LEVEL_INFO, "wifi IP:   %s\n", inet_ntoa(WiFi_Info.ipv4));
    LOG(LEVEL_INFO, "wifi MAC:  %02X:%02X:%02X:%02X:%02X:%02X\n\n", WiFi_Info.mac[0], 
                                                                    WiFi_Info.mac[1], 
                                                                    WiFi_Info.mac[2], 
                                                                    WiFi_Info.mac[3], 
                                                                    WiFi_Info.mac[4], 
                                                                    WiFi_Info.mac[5]);
    
    return 0;
}


//---------------------------------USER-----------------------------//

//WiFi发送数据
//data：要发送的数据指针；
//length：要发送数据长度；
//link_id：要发送数据连接id（一般情况为“1”，广播时为“0”）；
//send_ip：数据要发送到设备的ip地址；
//返回值：0，成功；1，错误；
uint8_t wifi_send_user(uint8_t *data, uint16_t length,uint8_t link_id,char *send_ip)
{
    uint8_t p[50];
    if(data==NULL)
    {
        return 3;
    }

    if(transparent_start == 1)
    {
        return 2;
    }

    if(send_ip!=NULL)                           //判断有无ip
    {
        sprintf((char *)p, "AT+CIPSEND=%d,%d,\"%s\",%d", link_id,length,send_ip,WIFI_UDP_NORMAL_PORT);         
    }else{
        sprintf((char *)p, "AT+CIPSEND=%d,%d", link_id,length);           
    }
    wifi_send_cmd((char *)p, ">", 200,NULL);
    return wifi_send_data((char *)data,length, "OK", 200);
   
}

//WiFi接收数据（检测数据是否是要的数据）
//data:返回找到的结果；
//data_len:数组长度；
//recv_ip:接收数据IP;
//返回值: -1: 接收数组错误；
//        0：没接收到数据；
//        其他：成功，数值为接收数据长度；
int wifi_recv_user(char *data,uint16_t data_len,uint32_t *recv_ip,uint16_t *recv_port)
{
    int wifi_recv_len=0;
    char *wifi_p;
    char recv_data_ip[20];

    if(data==NULL)
    {
        return -1;
    }  

    if (g_wifi_recv_data_sta) //接收到一次数据了
    {
        if(recv_port == NULL)
        {
            sscanf((char *)WIFI_RX_BUF,"%*[^,],%*d,%d,%[^,],%*d:%*s",&wifi_recv_len,recv_data_ip);
        }else{
            sscanf((char *)WIFI_RX_BUF,"%*[^,],%*d,%d,%[^,],%d:%*s",&wifi_recv_len,recv_data_ip,(int *)recv_port);
        }
      
        wifi_p = strstr((char *)WIFI_RX_BUF,":");
        if(recv_ip!=NULL)
        {
            *recv_ip = inet_addr(recv_data_ip);
        }
        if(data_len<=wifi_recv_len)
        {
            memcpy(data,wifi_p+1,data_len); 
        }else{
            memcpy(data,wifi_p+1,wifi_recv_len);
        }
        
        g_wifi_recv_data_sta=0;     //接收数据标志位置0
    }else if(wifi_sending_cmd_sta==0&&(UART7_RX_STA & (1 << 15))==1){     //命令或数据是否在发送中
        UART7_RX_STA=0;         //清除标志位（防止意外接收）
    }
    return wifi_recv_len;
}

//
extern uint8_t wifi_Init_sta;
uint8_t EV_WIFI_SET_SSID(char *WiFi_Name,char *Password)
{
    if(WiFi_Name == NULL || Password == NULL)
    {
        return 1;
    }

    memset(wifi_ssid,0,sizeof(wifi_ssid));
    memset(wifi_password,0,sizeof(wifi_password));
    if(strlen(WiFi_Name)>50)
    {
        memcpy(wifi_ssid,WiFi_Name,50);
    }else{
        memcpy(wifi_ssid,WiFi_Name,strlen(WiFi_Name));
    }
    
    if(strlen(Password)>50)
    {
        memcpy(wifi_password,Password,50);
    }else{
        memcpy(wifi_password,Password,strlen(Password));
    }
    wifi_Init_sta = 0;
    return 0;
}

