#include "eth_upgrade.h"
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
#include "config.h"
#include "gpio.h"
#include "esp8266.h"
#include "m2_interface.h"
#include "stdlib.h"

struct http_message_app_info_t message_app_info;                    //APP所有信息变量
struct http_message_resources_info_t message_resources_info;        //资源文件信息变量
struct http_url_info_t URL_LIST[URL_CHOOSE_MAX];                    //URL服务器路径表格
uint32_t loacl_version = 0;                                         //自身APP版本号

//查找APP所有信息
//str       被查找的字符串指针
//message   APP信息句柄
//返回值    0：成功 ；1：失败     
uint8_t find_app_message(char * str, struct http_message_app_info_t *message)
{
    uint8_t res = 0;
    char * message_str = str;
    if((res = sscanf((char *)message_str, "{\"instance\":\"%[^\"]\",\"time\":\"%[^\"]\",\"hard_v\":\"%[^\"]\",\"soft_v\":\"%[^\"]\",\"path\":\"%[^\"]\",\"files\":\"[%[^\]]]\",\"force\":\"%[^\"]\"}", \
    message->instance_buf, \
    message->time_buf,\
    message->hard_v_buf,\
    message->soft_v_buf,\
    message->path_buf, \
    message->file_buf, \
    message->force_buf)) != 7)
    {
        LOG(LEVEL_INFO, "find_app_message res %d\n", res);
        return 1;
    }
    return 0;
}

//查找资源文件信息
//str       被查找的字符串指针
//message   APP信息句柄
//返回值    0：成功 ；1：失败    
uint8_t find_resources_message(char * str, struct http_message_resources_info_t *message)
{
    uint8_t res = 0;
    uint8_t find_pos = 0;

    char * message_str = str;
    if((res = sscanf((char *)message_str, "{\"instance\":\"%[^\"]\",\"time\":\"%[^\"]\",\"hard_v\":\"%[^\"]\",\"resource_v\":\"%[^\"]\",\"path\":\"%[^\"]\",\"files\":\"%[^\"]\",\"reboot\":\"%[^\"]\"}", \
    message->instance_buf, \
    message->time_buf,\
    message->hard_v_buf,\
    message->resource_v_buf,\
    message->path_buf, \
    message->files_buf, \
    message->reboot_buf)) != 7)
    {
        LOG(LEVEL_INFO, "find_app_message res %d\n", res);
        return 1;
    }

    //取出files_buf中各文件的名称   文件数量
    for(uint8_t i = 0; i < strlen(message->files_buf); i++)
    {
        if(message->files_buf[i] == '&')
        {
            memcpy(message->file_buf[message->files_cnt], message->files_buf + find_pos + 1, i - find_pos - 2);

            find_pos = i + 1;
            message->files_cnt++;
        }
    }
    memcpy(message->file_buf[message->files_cnt], message->files_buf + find_pos + 1, strlen(message->files_buf) - find_pos - 2);
    message->files_cnt++;

    return 0;
}

//获取需要升级资源文件的内容
//path      本机资源文件夹路径
//message   服务器资源文件信息
//返回值    返回FRESULT状态
FRESULT scan_files_need(char *path, struct http_message_resources_info_t *message)
{
	FRESULT res; //定义结果对象
	DIR dir; //定义目录对象
	static FILINFO fno; //定义静态文件信息结构对象
	res = f_opendir(&dir,path); //打开目录，返回状态 和 目录对象的指针
	if(res == FR_OK) //打开成功
	{
		while(1)
		{
			res = f_readdir(&dir, &fno); //读取目录，返回状态 和 文件信息的指针
			if(res != FR_OK || fno.fname[0] == 0) break; //若打开失败 或 到结尾，则退出
			if(!(fno.fattrib & AM_DIR))
            {
				//LOG(LEVEL_INFO, "File:%s/%s",path,fno.fname); //是文件
				//strcpy(pathBuff, fno.fname); //将文件目录存储起来
                for(uint8_t i = 0; i < message->files_cnt; i++)
                {
                    if(strcmp(message->file_buf[i], fno.fname) == 0)
                    {
                        for(uint8_t j = i; j < message->files_cnt - 1; j++)         //文件名相同，把file_buf数组中数据向前移
                        {
                            memset(message->file_buf[j], 0, sizeof(message->file_buf[j]));
                            strcpy(message->file_buf[j], message->file_buf[j + 1]);
                        }
                        message->files_cnt--;           //文件名相同，需要下载的资源文件数-1；
                        break;
                    }
                }

			}
		}
	}
	else
	{
		LOG(LEVEL_INFO, "Fail - %s",&res); //打开失败
	}
	f_closedir(&dir); //关闭目录
	return res; //返回状态
}

//改变获取升级文件信息服务器
//url_now       当前服务器号
//max           服务器表格最大值
//返回值        下一个获取的路径
uint8_t change_url_choose(uint8_t url_now, uint8_t max)
{
    uint8_t res;
    res = url_now + 1;
    if(res >= max)
    {
        res = 0;
    }
    return res;
}

//查找版本号
//返回值    0：成功     其他：失败
uint8_t find_version(void)
{
    uint32_t  cpu_sr;
    uint8_t res = 0;
    FIL file_fd;
    uint8_t file_res;
    char read_buf[100];
    UINT read_len = 0; 
    int resource_version = 0;

    if(message_resources_info.files_cnt > 0)
    {
        OS_ENTER_CRITICAL();

        file_res = f_open(&file_fd, (TCHAR*)"0:/System/version_info.txt", FA_READ);
        if(file_res == FR_OK)
        {
            f_read(&file_fd, read_buf, sizeof(read_buf), &read_len);
            if(read_len > 0)
            {
                if(sscanf(read_buf, "resource:%03d\r\n", &resource_version) != 1)       //获取资源文件版本号
                {
                    resource_version = 0;
                }
                else
                {
                    LOG(LEVEL_INFO, "Local Resource Version is %03d\n", resource_version);
                }
            }
            if(resource_version >= atoi(message_resources_info.resource_v_buf)) //????????
            {
                scan_files_need("0:/Resources", &message_resources_info);
            }
            //LOG(LEVEL_INFO, "\n\n%s\n%s\n%s\n%s\n%d\n", 
                // message_resources_info.path_buf, 
                // message_resources_info.file_buf[0], 
                // message_resources_info.file_buf[1], 
                // message_resources_info.file_buf[2], 
                // message_resources_info.files_cnt);
        }else{
            
            res = 1;
        }

        f_close(&file_fd);
        OS_EXIT_CRITICAL();
    }
    // if(strcmp(http_message_info.instance_buf, FILE_INSTANCE_NAME) != 0)
    // {
    //     LOG(LEVEL_INFO, "Instance fail\n");
    //     goto FILES_MESSAGE_FAIL;
    // }
    Config_Read(FIRMWARE_VERSION_ADDR,&loacl_version,FIRMWARE_VERSION_SIZE);        //读取APP版本号
    if(loacl_version == 0xFFFFFFFF)
        loacl_version = 0;
    LOG(LEVEL_INFO, "\nLocal app version %d\n", loacl_version);

    return res;
}

//添加APP升级文件
//file_app_upgrade_data         升级文件内容
void add_app_upgrade_file(char *file_app_upgrade_data)
{
    uint32_t  cpu_sr;
    FIL file_fd;
    int file_res;
    char file_url[30];
    UINT file_bw;
    
    if(file_app_upgrade_data == NULL)
    {
        return;
    }
    memset(file_url, 0, sizeof(file_url));
    
    strcpy(file_url, "0:/");
    strcpy(file_url + strlen(file_url), message_app_info.file_buf);

    
    OS_ENTER_CRITICAL();

    file_res = f_open(&file_fd, (TCHAR*)file_url, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);
    LOG(LEVEL_INFO, "File f_open res %d\n", file_res);
    LOG(LEVEL_INFO, "%s\n", file_url);
    if(file_res == 0)
    {
        file_res = f_write(&file_fd, file_app_upgrade_data, message_app_info.file_length, &file_bw);
    }
    LOG(LEVEL_INFO, "APP File write res %d, %d, \n", file_res, file_bw);
    
    message_app_info.recv_ok_flag = 1;

    f_close(&file_fd);
    OS_EXIT_CRITICAL();
    
}

//添加资源文件
//resources_file_cnt       资源文件表格位置
void add_resource_file(uint8_t resources_file_cnt)
{
    uint32_t  cpu_sr;
    FIL file_fd;
    int file_res;
    char file_url[30];
    UINT file_bw;
    
    memset(file_url, 0, sizeof(file_url));
    strcpy(file_url, "0:/Resources/");
    strcpy(file_url + strlen(file_url), message_resources_info.file_buf[resources_file_cnt - 1]);
    
    OS_ENTER_CRITICAL();
    f_mkdir("0:/Resources");
    file_res = f_open(&file_fd, (TCHAR*)file_url, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);
    LOG(LEVEL_INFO, "File f_open res %d\n", file_res);
    LOG(LEVEL_INFO, "%s\n", file_url);
    if(file_res == 0)
    {
        file_res = f_write(&file_fd, \
                            message_resources_info.save_buf_p[resources_file_cnt - 1], \
                            message_resources_info.file_length[resources_file_cnt - 1], \
                            &file_bw);

    }
    LOG(LEVEL_INFO, "APP File write res %d, %d, \n", file_res, file_bw);
    
    f_close(&file_fd);

    if(resources_file_cnt == 1)                 //最后一个时，更改资源文件版本号
    {
        message_resources_info.recv_ok_flag = 1;

        file_res = f_open(&file_fd, (TCHAR*)"0:/System/version_info.txt", FA_READ|FA_WRITE|FA_CREATE_ALWAYS);
        f_printf(&file_fd, "resource:%s\r\n", message_resources_info.resource_v_buf);
        f_close(&file_fd);
    }
    OS_EXIT_CRITICAL();
}

//添加服务器路径
//Path      服务器路径
//f_Name    要获取文件列表的名称
//Port      服务器端口号
//返回值    0   成功
//          1   参数错误
//          2   路径错误
//          3   路径重复
//          4   表格已满
uint8_t EV_Add_Updata_Info_Manage(uint8_t mode, char *web_buf, char *f_Name,uint16_t Port)
{
    if(web_buf == NULL &&  f_Name == NULL)
    {
        return 1;
    }
    uint8_t url_cnt;
    char f_host_name[20];
    char f_url1_1[50];
  
    if(mode == 0)
    {
        if(strstr(web_buf,"www") == NULL)
        {
            LOG(LEVEL_INFO," web_buf err!!!\n");
            return 2;
        }

    } 
    else
    {
        
    }
    sscanf(web_buf,"%[^/]%s",f_host_name,f_url1_1);
    LOG(LEVEL_INFO,"%s\n%s\n",f_host_name,f_url1_1);
    for(url_cnt=0;url_cnt<URL_CHOOSE_MAX;url_cnt++)     //查找已有路径
    {
        if(URL_LIST[url_cnt].sta == 0)
        {
            break;
        }else{
            if(strcmp(URL_LIST[url_cnt].host_name,f_host_name) == 0)
            {
                return 3;  
            }    
        }
    }

    if(url_cnt >=URL_CHOOSE_MAX)
    {
        LOG(LEVEL_INFO,"URL_LIST FULL\n");
        return 4;
    }
    memset(URL_LIST[url_cnt].host_name, 0, sizeof(URL_LIST[url_cnt].host_name));
    memset(URL_LIST[url_cnt].url_1, 0, sizeof(URL_LIST[url_cnt].url_1));
    memset(URL_LIST[url_cnt].url_2, 0, sizeof(URL_LIST[url_cnt].url_2));
    memset(URL_LIST[url_cnt].url_3, 0, sizeof(URL_LIST[url_cnt].url_3));
    memset(URL_LIST[url_cnt].url_4, 0, sizeof(URL_LIST[url_cnt].url_4));
    memset(URL_LIST[url_cnt].ip_buf, 0, sizeof(URL_LIST[url_cnt].ip_buf));

    if(mode == 1)
    {
        sscanf(f_host_name,"%[^:]",URL_LIST[url_cnt].ip_buf);
    }

    URL_LIST[url_cnt].mode = mode;
    URL_LIST[url_cnt].sta = 1;
    URL_LIST[url_cnt].port = Port;
    strcpy(URL_LIST[url_cnt].host_name,f_host_name);
    sprintf(URL_LIST[url_cnt].url_1,"GET %s",f_url1_1);
    sprintf(URL_LIST[url_cnt].url_2,"/%s",f_Name);
    sprintf(URL_LIST[url_cnt].url_3," HTTP/1.1\r\n");
    sprintf(URL_LIST[url_cnt].url_4,"Host: %s\r\n\r\n",f_host_name);

    return 0;
}

static uint8_t web_to_ip32(struct http_url_info_t * web, struct in_addr  * ip)
{
    struct hostent *link_host;
    struct in_addr link_ipaddr;
    if(web->mode == 0)
    {
        link_host = gethostbyname(web->host_name);
        if(link_host == NULL)
            return 1;
        else
        {
            link_ipaddr.s_addr = *(uint32_t *)link_host->h_addr_list[0];
        }
        
    }
    else
    {
        link_ipaddr.s_addr = inet_addr(web->ip_buf);
    }
    *ip = link_ipaddr;
    return 0;
}

/***************************************************************************************************/
/********************************************lwip_upgrade*******************************************/
/***************************************************************************************************/
#define LWIP_UPGRADE_PRIO		LWIP_UPGRADE_PRIO_NUM
#define LWIP_UPGRADE_STK_SIZE	3000
uint32_t *LWIP_UPGRADE_TASK_STK;

void lwip_upgarde_thread(void *arg)
{
    while(dhcpstatus != 2)
    {
        OSTimeDly(10);
    }

    OSTimeDly(100);
    
	struct hostent *link_host;
    struct in_addr link_ipaddr;
    struct sockaddr_in link_sockaddr;
	int sock_fd;
    int timeout = 500;
    int res;
    
    int recv_len = 0;
    char *send_buf_p;
    char *recv_buf_p;
	char *save_file_buf_p = NULL;
    
    send_buf_p = mymalloc(SRAMEX, HTTP_BUF_MAX);
    recv_buf_p = mymalloc(SRAMEX, HTTP_BUF_MAX);
    
    char *message_start_p;
    uint8_t url_choose = 0;
    while(1)
    {
        LOG(LEVEL_INFO,"\n******* URL %d *******\n",url_choose);
      
        if(web_to_ip32(&URL_LIST[url_choose], &link_ipaddr) != 0)
        {
            url_choose = change_url_choose(url_choose, URL_CHOOSE_MAX);
            continue;   
        }
        link_sockaddr.sin_family = AF_INET;
        link_sockaddr.sin_port = htons(URL_LIST[url_choose].port);
        link_sockaddr.sin_addr = link_ipaddr;

        memset(&(link_sockaddr.sin_zero), 0, sizeof(link_sockaddr.sin_zero));

        if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            goto LWIP_SOCKET_FAIL;
        }
        
        setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout,sizeof(timeout));
        
        if(connect(sock_fd, (struct sockaddr *)&link_sockaddr, sizeof(struct sockaddr)) == -1)
        {
            LOG(LEVEL_INFO, "Connect fail!\n");
            url_choose = change_url_choose(url_choose, URL_CHOOSE_MAX);
            goto LWIP_CONNECT_FAIL;
        }
        LOG(LEVEL_INFO, "Connect Success!\n");
       
        memset(send_buf_p, 0, HTTP_BUF_MAX);
        strcpy(send_buf_p, URL_LIST[url_choose].url_1);
        strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_2);
        strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_3);
        strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_4);
    
        //LOG(LEVEL_INFO, "Send %s\n", send_buf_p);
        if((res = send(sock_fd, send_buf_p, strlen((char *)send_buf_p), 0)) < 0)
        {
            LOG(LEVEL_INFO, "Res(%d)\n", res);
            LOG(LEVEL_INFO, "Send Fail\n");
            goto LWIP_FILES_MESSAGE_FAIL;
        }

        //LOG(LEVEL_INFO, "Res(%d)\n", res);
        OSTimeDly(1000);
        uint8_t message_recv_cnt = 0;
        while(1)
        {
            memset(recv_buf_p, 0, HTTP_BUF_MAX);
            recv_len = recv(sock_fd, recv_buf_p, HTTP_BUF_MAX, 0);
            if(recv_len > 0)
            {
                message_recv_cnt = 0;
                //LOG(LEVEL_INFO, "Recv(%d):%s\n", recv_len, recv_buf_p);
                message_start_p = strstr((char *)recv_buf_p, "{\"instance\":");
                
                if(message_start_p != NULL)
                {
                    //LOG(LEVEL_INFO, "\nMessage: %s\n", message_start_p);
                }
                else
                {
                    LOG(LEVEL_INFO, "Recv Message fail!\n");
                    goto LWIP_FILES_MESSAGE_FAIL;
                }
                
                memset(&message_app_info, 0, sizeof(message_app_info));
                
                if(find_app_message(message_start_p, &message_app_info) != 0)
                {
                    LOG(LEVEL_INFO, "FILES_MESSAGE_FAIL!\n");
                    goto LWIP_FILES_MESSAGE_FAIL;
                }
                //LOG(LEVEL_INFO, "\n\n%s\n%s\n%s\n%s\n", 
                // message_app_info.instance_buf, 
                // message_app_info.time_buf,
                // message_app_info.file_buf, 
                // message_app_info.force_buf);

                memset(&message_resources_info, 0, sizeof(message_resources_info));
                message_start_p = strstr(message_start_p, "\n");
                if(message_start_p != NULL)
                {
                    message_start_p += 1;
                    //LOG(LEVEL_INFO, "\nRes Message: %s\n", message_start_p);
                }
                else
                {
                    goto LWIP_FILES_MESSAGE_FAIL;
                }

                if(find_resources_message(message_start_p, &message_resources_info) != 0)
                {
                    LOG(LEVEL_INFO, "Resource FILES_MESSAGE_FAIL!\n");
                    //goto LWIP_FILES_MESSAGE_FAIL;
                }
                else
                {
                    //LOG(LEVEL_INFO, "\n\n%s\n%s\n%s\n%s\n%d\n", 
                        // message_resources_info.path_buf, 
                        // message_resources_info.file_buf[0], 
                        // message_resources_info.file_buf[1], 
                        // message_resources_info.file_buf[2], 
                        // message_resources_info.files_cnt);
                }
               
                find_version();
                break;
            }
            else
            {
                OSTimeDly(2000);
                if(message_recv_cnt ++ > 5)
                {
                    LOG(LEVEL_INFO, "Message no recv data.\n");
                    goto LWIP_FILES_MESSAGE_FAIL;
                }
            }
        }
        if(loacl_version < atoi(message_app_info.soft_v_buf))
        {
            memset(send_buf_p, 0, HTTP_BUF_MAX);
            
            strcpy(send_buf_p, URL_LIST[url_choose].url_1);
            strcpy(send_buf_p + strlen(send_buf_p), message_app_info.path_buf);
            strcpy(send_buf_p + strlen(send_buf_p), message_app_info.file_buf);
            strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_3);
            strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_4);
            //LOG(LEVEL_INFO, "URL:%s\n", send_buf_p);
            send(sock_fd, send_buf_p, strlen((char *)send_buf_p), 0);
            
            uint32_t recv_cnt = 0;
            char *buf_temp_p;
            uint8_t recv_err_cnt = 0;
            while(1)
            {
                memset(recv_buf_p, 0, HTTP_BUF_MAX);
                recv_len = recv(sock_fd, recv_buf_p, HTTP_BUF_MAX, 0);
                if(recv_len > 0)
                {
                    buf_temp_p = strstr(recv_buf_p, "\r\n\r\n");
                    recv_cnt += recv_len - (buf_temp_p - recv_buf_p + 4);
                    
                    buf_temp_p = strstr(recv_buf_p, "Content-Length: ");
                    if(buf_temp_p == NULL)
                    {
                        LOG(LEVEL_INFO,"File recv data err\n");
                        goto LWIP_APP_FILE_RECV_FAIL;
                    }
                    sscanf(buf_temp_p, "Content-Length: %d\r\n%*s", &(message_app_info.file_length));
                    LOG(LEVEL_INFO, "Need Recv Length:%d\n", message_app_info.file_length);
                    save_file_buf_p = mymalloc(SRAMEX, message_app_info.file_length);
                    memcpy(save_file_buf_p, recv_buf_p + recv_len - recv_cnt, recv_cnt);
                    break;
                }
                else
                {
                    OSTimeDly(1000);
                    if(recv_err_cnt++ > 5)
                    {
                        LOG(LEVEL_INFO, "File recv fail\n");
                        goto LWIP_APP_FILE_RECV_FAIL;
                    }
                }
            }
            
            recv_err_cnt = 0;
            while(1)
            {
                memset(recv_buf_p, 0, HTTP_BUF_MAX);
                recv_len = recv(sock_fd, recv_buf_p, HTTP_BUF_MAX, 0);
                if(recv_len >= 0)
                {
                    memcpy(save_file_buf_p + recv_cnt, recv_buf_p, recv_len);
                    recv_cnt += recv_len;
                    recv_err_cnt = 0;
                }
                else
                {
                    if(recv_cnt >= message_app_info.file_length)
                    {
                        //LOG(LEVEL_INFO, "File recv %d\n", recv_cnt);
                        LOG(LEVEL_INFO, "File recv success!\n");
                        break;
                    }
                    else
                    {
                        OSTimeDly(1000);
                        if(recv_err_cnt++ > 5)
                        {
                            //LOG(LEVEL_INFO, "File recv %d\n", recv_cnt);
                            LOG(LEVEL_INFO, "File recv fail!\n");
                            goto LWIP_APP_FILE_RECV_FAIL;
                        }
                        
                    }
                }
                OSTimeDly(5);
            }
            if(strcmp(message_app_info.force_buf, "1") == 0)
            {
                if(1)
                if(recv_cnt == message_app_info.file_length)
                {
                    LOG(LEVEL_INFO, "ALL recv OK!\n");
                    add_app_upgrade_file(save_file_buf_p);           //app文件添加
                } 
            }
            OSTimeDly(1000);
        }

LWIP_APP_FILE_RECV_FAIL:
        OSTimeDly(1000);

/*********************************************resource**********************************************/
        uint8_t resources_file_cnt = message_resources_info.files_cnt;
        while(resources_file_cnt > 0)
        {
            memset(send_buf_p, 0, HTTP_BUF_MAX);
            strcpy(send_buf_p, URL_LIST[url_choose].url_1);
            strcpy(send_buf_p + strlen(send_buf_p), message_resources_info.path_buf);
            strcpy(send_buf_p + strlen(send_buf_p), message_resources_info.file_buf[resources_file_cnt - 1]);
            strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_3);
            strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_4);
            LOG(LEVEL_INFO, "URL:%s\n", send_buf_p);
            send(sock_fd, send_buf_p, strlen((char *)send_buf_p), 0);
            
            uint32_t recv_cnt = 0;
            char *buf_temp_p;
            
            uint8_t recv_err_cnt = 0;
            while(1)
            {
                memset(recv_buf_p, 0, HTTP_BUF_MAX);
                recv_len = recv(sock_fd, recv_buf_p, HTTP_BUF_MAX, 0);
                if(recv_len > 0)
                {
                    buf_temp_p = strstr(recv_buf_p, "\r\n\r\n");
                    recv_cnt += recv_len - (buf_temp_p - recv_buf_p + 4);
                    
                    buf_temp_p = strstr(recv_buf_p, "Content-Length: ");
                    if(buf_temp_p == NULL)
                    {
                        LOG(LEVEL_INFO,"File recv data err\n");
                        goto LWIP_RES_FILE_RECV_FAIL;
                    }
                    sscanf(buf_temp_p, "Content-Length: %d\r\n%*s", &(message_resources_info.file_length[resources_file_cnt - 1]));
                    LOG(LEVEL_INFO, "Need Recv Length:%d\n", message_resources_info.file_length[resources_file_cnt - 1]);
                    message_resources_info.save_buf_p[resources_file_cnt - 1] = mymalloc(SRAMEX, message_resources_info.file_length[resources_file_cnt - 1]);
                    memcpy(message_resources_info.save_buf_p[resources_file_cnt - 1], recv_buf_p + recv_len - recv_cnt, recv_cnt);
                    break;
                }
                else
                {
                    OSTimeDly(1000);
                    if(recv_err_cnt++ > 5)
                    {
                        LOG(LEVEL_INFO, "File recv fail\n");
                        goto LWIP_RES_FILE_RECV_FAIL;
                    }
                }
            }
            
            recv_err_cnt = 0;
            while(1)
            {
                memset(recv_buf_p, 0, HTTP_BUF_MAX);
                recv_len = recv(sock_fd, recv_buf_p, HTTP_BUF_MAX, 0);
                if(recv_len >= 0)
                {
                    memcpy(message_resources_info.save_buf_p[resources_file_cnt - 1] + recv_cnt, recv_buf_p, recv_len);
                    recv_cnt += recv_len;
                }
                else
                {
                    if(recv_cnt >= message_resources_info.file_length[resources_file_cnt - 1])
                    {
                        LOG(LEVEL_INFO, "File recv success!\n");
                        break;
                    }
                    else
                    {
                        OSTimeDly(1000);
                        if(recv_err_cnt++ > 5)
                        {
                            LOG(LEVEL_INFO, "File recv fail!\n");
                            goto LWIP_RES_FILE_RECV_FAIL;
                        }
                    }
                }
                OSTimeDly(5);
            }
            if(1)
            {
                if(1)
                if(recv_cnt == message_resources_info.file_length[resources_file_cnt - 1])
                {
                    LOG(LEVEL_INFO, "ALL recv OK!\n");
                    add_resource_file(resources_file_cnt);        //添加缺少的资源文件
                }
                
            }
            resources_file_cnt--;
        }

LWIP_RES_FILE_RECV_FAIL:
        OSTimeDly(1000);
    
        myfree(SRAMEX, save_file_buf_p);

        myfree(SRAMEX, message_resources_info.save_buf_p[0]);
        myfree(SRAMEX, message_resources_info.save_buf_p[1]);
        myfree(SRAMEX, message_resources_info.save_buf_p[2]);
        myfree(SRAMEX, message_resources_info.save_buf_p[3]);
        myfree(SRAMEX, message_resources_info.save_buf_p[4]);
        myfree(SRAMEX, message_resources_info.save_buf_p[5]);
        myfree(SRAMEX, message_resources_info.save_buf_p[6]);
        myfree(SRAMEX, message_resources_info.save_buf_p[7]);
        myfree(SRAMEX, message_resources_info.save_buf_p[8]);
        myfree(SRAMEX, message_resources_info.save_buf_p[9]);


        if(strcmp(message_app_info.force_buf, "1") == 0  
           || strcmp(message_resources_info.reboot_buf, "1") == 0)
            {
                if(message_resources_info.recv_ok_flag == 1  
                || message_app_info.recv_ok_flag == 1)
                {
                    LOG(LEVEL_INFO, "System restart!\n");
                    HAL_NVIC_SystemReset();
                }

            }

        OSTimeDly(10000);
    
LWIP_FILES_MESSAGE_FAIL:
        OSTimeDly(2000);
      
LWIP_CONNECT_FAIL:
        OSTimeDly(100000);

LWIP_SOCKET_FAIL:
        LOG(LEVEL_INFO, "Close socket!\n");
        close(sock_fd);
        OSTimeDly(10000);
    }
}

/***************************************************************************************************/
/********************************************wifi_upgrade*******************************************/
/***************************************************************************************************/


//透传数据指针
extern uint8_t *transparent_rx_buf_cnt;             //当前指针
extern uint8_t *transparent_rx_buf_last;            //末尾指针
extern uint8_t *transparent_rx_buf;                 //接收缓冲区指针

#define WIFI_TASK_UPDATE_PRIO  WIFI_TASK_UPDATE_PRIO_NUM
#define WIFI_STK_UPDATE_SIZE   3000 
uint32_t *WIFI_TASK_UPDATE_STK; 
void wifi_update_task(void *pdata)
{
    while(g_net_user_sta != WIFI_NETWORK)
    {
        OSTimeDly(1000);
    }
    int recv_len = 0;
    char *send_buf_p;
    char *recv_buf_p;
	char *save_file_buf_p = NULL;
    uint8_t *transparent_recv_old;     //数据接收位置指针
    uint8_t *transparent_recv_new;
    
    send_buf_p = mymalloc(SRAMEX, HTTP_BUF_MAX);
    recv_buf_p = mymalloc(SRAMEX, HTTP_BUF_MAX);

    char *message_start_p;

    char TCP_IP[20] = {0};
    uint8_t url_choose = 0;
    while(1)
    {
        LOG(LEVEL_INFO,"\n******* URL %d *******\n",url_choose);

        if(wifi_tcp_DNS(URL_LIST[url_choose].host_name,TCP_IP) != 0)
        {
            url_choose = change_url_choose(url_choose, URL_CHOOSE_MAX);
            OSTimeDly(1000);
            continue;
        }
        
        wifi_tcp_connect(TCP_IP,URL_LIST[url_choose].port);             
        OSTimeDly(1000);

        memset(send_buf_p, 0, HTTP_BUF_MAX);
        strcpy(send_buf_p, URL_LIST[url_choose].url_1);
        strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_2);
        strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_3);
        strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_4);

        wifi_tcp_recving_sta = 1;           //tcp 接受中（防止普通任务接收到这次数据）
        if(wifi_send_user((uint8_t *)send_buf_p,strlen(send_buf_p),2,NULL))
        {
            LOG(LEVEL_INFO, "Send Fail\n");
            OSTimeDly(2000);
            continue;
        }
        
        OSTimeDly(1000);
        uint8_t message_recv_cnt = 0;
        uint32_t recv_tcp_ip = 0;
        while(1)
        {
            memset(recv_buf_p, 0, HTTP_BUF_MAX);
            recv_len=wifi_recv_user(recv_buf_p,HTTP_BUF_MAX,&recv_tcp_ip,NULL);
            if(inet_addr(TCP_IP) == recv_tcp_ip&& recv_len > 0)
            {
                //LOG(LEVEL_INFO, "wifi_Recv(%d):%s\n", recv_len, recv_buf_p);
                message_start_p = strstr((char *)recv_buf_p, "{\"instance\":\"");
                if(message_start_p!=NULL)
                {

                }else{
                    LOG(LEVEL_INFO,"wifi_recv FAIL\n");
                    goto WIFI_FILES_MESSAGE_FAIL;
                }

                memset(&message_app_info, 0, sizeof(message_app_info));
                    
                if(find_app_message(message_start_p, &message_app_info) != 0)
                {
                    LOG(LEVEL_INFO, "FILES_MESSAGE_FAIL!\n");
                    goto WIFI_FILES_MESSAGE_FAIL;
                }
                //LOG(LEVEL_INFO, "\n\n%s\n%s\n%s\n%s\n", 
                // message_app_info.instance_buf, 
                // message_app_info.time_buf,
                // message_app_info.file_buf, 
                // message_app_info.force_buf);

                memset(&message_resources_info, 0, sizeof(message_resources_info));
                message_start_p = strstr(message_start_p, "\n");
                if(message_start_p != NULL)
                {
                    message_start_p += 1;
                    //LOG(LEVEL_INFO, "\nRes Message: %s\n", message_start_p);
                }
                else
                {
                    goto WIFI_FILES_MESSAGE_FAIL;
                }

                if(find_resources_message(message_start_p, &message_resources_info) != 0)
                {
                    LOG(LEVEL_INFO, "Resource FILES_MESSAGE_FAIL!\n");
                    //goto WIFI_FILES_MESSAGE_FAIL;
                }
                else
                {
                    //LOG(LEVEL_INFO, "\n\n%s\n%s\n%s\n%s\n%d\n", 
                        // message_resources_info.path_buf, 
                        // message_resources_info.file_buf[0], 
                        // message_resources_info.file_buf[1], 
                        // message_resources_info.file_buf[2], 
                        // message_resources_info.files_cnt);
                }
               
                find_version();             //找到现在的版本号
                break;

            }else{
                OSTimeDly(1000);
                if(message_recv_cnt ++ > 5)
                {
                    LOG(LEVEL_INFO, "wifi Message no recv data.\n");
                    wifi_tcp_recving_sta = 0;       //TCP接收完成
                    goto WIFI_FILES_MESSAGE_FAIL;
                }

            }
        }
        wifi_tcp_recving_sta = 0;       //TCP接收完成
        if(loacl_version < atoi(message_app_info.soft_v_buf))
        {
            memset(send_buf_p, 0, HTTP_BUF_MAX); 
            strcpy(send_buf_p, URL_LIST[url_choose].url_1);
            strcpy(send_buf_p + strlen(send_buf_p), message_app_info.path_buf);
            strcpy(send_buf_p + strlen(send_buf_p), message_app_info.file_buf);
            strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_3);
            strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_4);
        
            uint32_t recv_cnt = 0;
            char *buf_temp_p;
            uint8_t recv_err_cnt = 0;

            wifi_connect_transparent(TCP_IP,URL_LIST[url_choose].port);          //设置透传模式 
            transparent_recv_old=transparent_rx_buf;
            transparent_recv_new=transparent_rx_buf;
            wifi_send_transparent((uint8_t *)send_buf_p, strlen(send_buf_p));

            while(1)
            {
                if(transparent_rx_buf_cnt-transparent_recv_old > 500)           //获取数据大于500时开始数据处理
                {
                    transparent_recv_new = transparent_recv_old+500;
                    recv_len = transparent_recv_new - transparent_recv_old;
                    memcpy(recv_buf_p,transparent_recv_old,recv_len);
                    transparent_recv_old = transparent_recv_new;

                    // LOG(LEVEL_INFO,"RX_APP:%s\n",recv_buf_p);
                    buf_temp_p = strstr(recv_buf_p, "\r\n\r\n");
                    recv_cnt += recv_len - (buf_temp_p - recv_buf_p + 4);			
                    buf_temp_p = strstr(recv_buf_p, "Content-Length: ");
                    if(buf_temp_p == NULL)
                    {
                        LOG(LEVEL_INFO, "File recv data err\n");
                        goto WIFI_APP_FILE_RECV_FAIL;
                    }
                    sscanf(buf_temp_p, "Content-Length: %d\r\n%*s", &(message_app_info.file_length));
                    LOG(LEVEL_INFO, "Need Recv Length:%d\n", message_app_info.file_length);  
                    save_file_buf_p = mymalloc(SRAMEX, message_app_info.file_length);
                    memcpy(save_file_buf_p, recv_buf_p + recv_len - recv_cnt, recv_cnt);
                    break;
                }else{
                    OSTimeDly(100);
                    if(recv_err_cnt++ > 50)
                    {
                        LOG(LEVEL_INFO, "File recv fail\n");
                        goto WIFI_APP_FILE_RECV_FAIL;
                    }
                }
            }

            recv_err_cnt = 0;
            while(1)
            {
                if(transparent_rx_buf_cnt<transparent_recv_old)             //透传数据指针重置到开头时
                {
                    transparent_recv_new = transparent_rx_buf_cnt;
                    recv_len = transparent_rx_buf_last - transparent_recv_old + 1;
                    memcpy(save_file_buf_p + recv_cnt, transparent_recv_old, recv_len);
                    recv_cnt += recv_len;

                    recv_len = transparent_recv_new-transparent_rx_buf;
                    memcpy(save_file_buf_p + recv_cnt, transparent_rx_buf, recv_len);
                    transparent_recv_old = transparent_recv_new;
                    recv_cnt += recv_len;
                    recv_err_cnt = 0;               //接收到数据，错误计数重置

                }else if(transparent_rx_buf_cnt>transparent_recv_old){      //透传指针一般情况
                    transparent_recv_new = transparent_rx_buf_cnt;
                    recv_len = transparent_recv_new - transparent_recv_old;
                    memcpy(save_file_buf_p + recv_cnt, transparent_recv_old, recv_len);
                    transparent_recv_old = transparent_recv_new;
                    recv_cnt += recv_len;
                    recv_err_cnt = 0;              //接收到数据，错误计数重置
                }
                if(recv_cnt >= message_app_info.file_length)
                {
                    LOG(LEVEL_INFO, "File recv success!\n");
                    break;
                }
                if(recv_err_cnt++ > 200)
                {
                    LOG(LEVEL_INFO, "File recv fail!\n");
                    goto WIFI_APP_FILE_RECV_FAIL;
                }
                OSTimeDly(10);
            }

            if(strcmp(message_app_info.force_buf, "1") == 0)
            {
                if(1)
                if(recv_cnt == message_app_info.file_length)
                {
                    LOG(LEVEL_INFO, "ALL recv OK!\n");
                    add_app_upgrade_file(save_file_buf_p);           //app文件添加
                }
                
            }
            OSTimeDly(1000);
        }    
WIFI_APP_FILE_RECV_FAIL:
        OSTimeDly(1000);


/*********************************************resource**********************************************/
        uint8_t resources_file_cnt = message_resources_info.files_cnt;        
        if(resources_file_cnt>0)
        {
            wifi_connect_transparent(TCP_IP,URL_LIST[url_choose].port);       //设置透传模式
        }
        
        while(resources_file_cnt > 0)
        {
            memset(send_buf_p, 0, HTTP_BUF_MAX);
            strcpy(send_buf_p, URL_LIST[url_choose].url_1);
            strcpy(send_buf_p + strlen(send_buf_p), message_resources_info.path_buf);
            strcpy(send_buf_p + strlen(send_buf_p), message_resources_info.file_buf[resources_file_cnt - 1]);
            strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_3);
            strcpy(send_buf_p + strlen(send_buf_p), URL_LIST[url_choose].url_4);
            LOG(LEVEL_INFO, "URL:%s\n", send_buf_p);

            transparent_rx_buf_cnt=transparent_rx_buf;
            transparent_recv_old=transparent_rx_buf;
            transparent_recv_new=transparent_rx_buf;
            wifi_send_transparent((uint8_t *)send_buf_p, strlen(send_buf_p));
            
            uint32_t recv_cnt = 0;
            char *buf_temp_p;
            uint8_t recv_err_cnt = 0;
            while(1)
            {
                if(transparent_rx_buf_cnt-transparent_recv_old > 500)
                {
                    transparent_recv_new = transparent_recv_old+500;
                    recv_len = transparent_recv_new - transparent_recv_old;
                    memcpy(recv_buf_p,transparent_recv_old,recv_len);
                    transparent_recv_old = transparent_recv_new;

                    // LOG(LEVEL_INFO,"rx_resource:%s\n",recv_buf_p);
                    buf_temp_p = strstr(recv_buf_p, "\r\n\r\n");
                    recv_cnt += recv_len - (buf_temp_p - recv_buf_p + 4);
                    buf_temp_p = strstr(recv_buf_p, "Content-Length: ");
                    if(buf_temp_p == NULL)
                    {
                        LOG(LEVEL_INFO, "File recv data err\n");
                        goto WIFI_RES_FILE_RECV_FAIL;
                    }
                    sscanf(buf_temp_p, "Content-Length: %d\r\n%*s", &(message_resources_info.file_length[resources_file_cnt - 1]));
                    LOG(LEVEL_INFO, "Need Recv Length:%d\n", message_resources_info.file_length[resources_file_cnt - 1]);
                    message_resources_info.save_buf_p[resources_file_cnt - 1] = mymalloc(SRAMEX, message_resources_info.file_length[resources_file_cnt - 1]);
                    memcpy(message_resources_info.save_buf_p[resources_file_cnt - 1], recv_buf_p + recv_len - recv_cnt, recv_cnt);
                    break;
                }else{
                    OSTimeDly(100);
                    if(recv_err_cnt++ > 50)
                    {
                        LOG(LEVEL_INFO, "File recv fail\n");
                        goto WIFI_RES_FILE_RECV_FAIL;
                    }
                }
            }
            
            recv_err_cnt = 0;
            while(1)
            {
                if(transparent_rx_buf_cnt<transparent_recv_old)
                {
                    transparent_recv_new = transparent_rx_buf_cnt;
                    recv_len = transparent_rx_buf_last - transparent_recv_old + 1;
                    memcpy(message_resources_info.save_buf_p[resources_file_cnt - 1] + recv_cnt, transparent_recv_old, recv_len);
                    recv_cnt += recv_len;

                    recv_len = transparent_recv_new-transparent_rx_buf;
                    memcpy(message_resources_info.save_buf_p[resources_file_cnt - 1] + recv_cnt, transparent_rx_buf, recv_len);
                    transparent_recv_old = transparent_recv_new;
                    recv_cnt += recv_len;
                    recv_err_cnt = 0;               //接收到数据重置

                }else if(transparent_rx_buf_cnt>transparent_recv_old){
                    transparent_recv_new = transparent_rx_buf_cnt;
                    recv_len = transparent_recv_new - transparent_recv_old;
                    memcpy(message_resources_info.save_buf_p[resources_file_cnt - 1] + recv_cnt, transparent_recv_old, recv_len);
                    transparent_recv_old = transparent_recv_new;
                    recv_cnt += recv_len;
                    recv_err_cnt = 0;               //接收到数据重置
                }
                if(recv_cnt >= message_resources_info.file_length[resources_file_cnt - 1])
                {
                    LOG(LEVEL_INFO, "File recv success!\n");
                    break;
                }
                if(recv_err_cnt++ > 200)
                {
                    //LOG(LEVEL_INFO, "File recv %d\n", recv_cnt);
                    LOG(LEVEL_INFO, "File recv fail!\n");
                    goto WIFI_RES_FILE_RECV_FAIL;
                }
                OSTimeDly(10);
            }
           
            if(1)
            {
                if(1)
                if(recv_cnt == message_resources_info.file_length[resources_file_cnt - 1])
                {
                    LOG(LEVEL_INFO, "resource_%d:ALL recv OK!\n",resources_file_cnt-1);
                    add_resource_file(resources_file_cnt);        //添加缺少的资源文件
                }
                
            }
            resources_file_cnt--;
        }
        
WIFI_RES_FILE_RECV_FAIL:
        OSTimeDly(1000);

        if(wifi_disconnect_transparent())    //关闭透传模式(必须成功)
        {
            goto  WIFI_RES_FILE_RECV_FAIL;
        }
           
        myfree(SRAMEX, save_file_buf_p);

        myfree(SRAMEX, message_resources_info.save_buf_p[0]);
        myfree(SRAMEX, message_resources_info.save_buf_p[1]);
        myfree(SRAMEX, message_resources_info.save_buf_p[2]);
        myfree(SRAMEX, message_resources_info.save_buf_p[3]);
        myfree(SRAMEX, message_resources_info.save_buf_p[4]);
        myfree(SRAMEX, message_resources_info.save_buf_p[5]);
        myfree(SRAMEX, message_resources_info.save_buf_p[6]);
        myfree(SRAMEX, message_resources_info.save_buf_p[7]);
        myfree(SRAMEX, message_resources_info.save_buf_p[8]);
        myfree(SRAMEX, message_resources_info.save_buf_p[9]);

        //APP或者资源文件下载成功，系统重启
        if(strcmp(message_app_info.force_buf, "1") == 0 
           ||strcmp(message_resources_info.reboot_buf, "1") == 0)
            {
                if(message_resources_info.recv_ok_flag == 1  
                    ||message_app_info.recv_ok_flag == 1)
                    {
                        LOG(LEVEL_INFO, "System restart!\n");
                        HAL_NVIC_SystemReset();
                    }

            }

        OSTimeDly(30000);
    
WIFI_FILES_MESSAGE_FAIL:
        OSTimeDly(100000);
    }

}


void eth_upgrade_creat(void)
{
    uint32_t  cpu_sr;
    OS_ENTER_CRITICAL();

    LWIP_UPGRADE_TASK_STK = mymalloc(SRAMEX, LWIP_UPGRADE_STK_SIZE* 4);
	OSTaskCreate(lwip_upgarde_thread, \
				"lwip_upgrade", \
				NULL, \
				(uint32_t*)&LWIP_UPGRADE_TASK_STK[LWIP_UPGRADE_STK_SIZE-1], \
				LWIP_UPGRADE_PRIO);

    WIFI_TASK_UPDATE_STK = mymalloc(SRAMEX, WIFI_STK_UPDATE_SIZE* 4);
    OSTaskCreate(wifi_update_task, \
                 "wifi_update_task", \
                  NULL, \
                  (uint32_t *)&WIFI_TASK_UPDATE_STK[WIFI_STK_UPDATE_SIZE - 1], \
                  WIFI_TASK_UPDATE_PRIO);

    OSTaskSuspend(LWIP_UPGRADE_PRIO);
    OSTaskSuspend(WIFI_TASK_UPDATE_PRIO);

    OS_EXIT_CRITICAL();
}
