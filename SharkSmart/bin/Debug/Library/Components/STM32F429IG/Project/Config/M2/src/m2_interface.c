#include "gpio.h"
#include "malloc.h"
#include "log.h"
#include "m2_interface.h"
#include "nrf52832.h"
#include "wmiic.h"
#include "string.h"
#include "udp_bcast.h"
#include "eth_user.h"
#include "ff.h"

#define BLE_RECV_DATA_MAX   100

//插入模块初始化标志
uint8_t ble_Init_sta    = 0;
uint8_t wifi_Init_sta   = 0;
uint8_t other_Init_sta  = 0;

//M2
__M2InterfaceStruct M2InterfaceStruct;
static InsertStaEnum check_insert_sta(InterfaceNumEnum num);
//BLE
uint8_t ble_recv_data[BLE_RECV_DATA_MAX];
uint8_t ble_recv_cnt = 0;
uint8_t Ant_Devices_Cnt = 0;
struct ANT_Device_Info_t ANT_Device_Info[ANT_DEVICES_MAX];

void ble_rx_callback(uint8_t *data, uint16_t len)
{
	//LOG(LEVEL_INFO, "%02X", data[0]);
    ble_recv_data[ble_recv_cnt++] = data[0];
    ble_recv_cnt = (ble_recv_cnt >= BLE_RECV_DATA_MAX) ? 0 : ble_recv_cnt;     
}
//WIFI
uint8_t wifi_tcp_recving_sta = 0;   
//extern char test_time_buf[50];          //获取时间测试     
extern uint8_t ( *user_eth_normal_recv)(uint8_t *user_recv,int user_len,uint32_t user_sin_addr);


//GET_ID
void EV_GET_MPU_ID(uint32_t MPU_ID_Out[3])
{
    if(MPU_ID_Out == NULL)
        return;
    
	MPU_ID_Out[0] = *(__IO u32*)(0x1FFF7A10);
	MPU_ID_Out[1] = *(__IO u32*)(0x1FFF7A14);
	MPU_ID_Out[2] = *(__IO u32*)(0x1FFF7A18);
	
}

//UUID 16BIT
uint16_t EV_Get_16Bit_uuid(uint32_t MPU_ID_In[3])
{
    uint16_t uuid = 0;
    uint32_t uuid_32bit;
    uint32_t id_buff[3];

    if(MPU_ID_In == NULL)
        return 0;

    memcpy(id_buff,MPU_ID_In,sizeof(id_buff));
    for(uint8_t i=0,tmp=0;i<3;i++)
    {
        tmp = (uint8_t)((id_buff[i]+i)&0x0000000f);
        for(uint8_t j=0;j<8;j++)
        {
            id_buff[i] += (uint32_t)((tmp+j)<<(j*4));
        }
    }
    uuid_32bit = id_buff[0]+id_buff[1]+id_buff[2];
    uuid = (uint16_t)uuid_32bit;
    uuid += (uint16_t)(uuid_32bit>>16);
    
    return uuid;
}

//UUID 128BIT
uint8_t EV_Get_128Bit_uuid(uint32_t MPU_ID_In[3],uint32_t ID_128Bit[4])
{
    uint32_t sum;
    uint8_t right;
    uint32_t id_buff[3];

    if(MPU_ID_In == NULL||ID_128Bit == NULL)
        return 1;

    memcpy(id_buff,MPU_ID_In,sizeof(id_buff));
    for(uint8_t i=0,tmp=0;i<3;i++)
    {
        tmp = (uint8_t)((id_buff[i]+i)&0x0000000f);
        for(uint8_t j=0;j<8;j++)
        {
            id_buff[i] += (uint32_t)((tmp+j)<<(j*4));
        }
    }
    sum = id_buff[0]+id_buff[1]+id_buff[2];
    right = (uint8_t)(sum&0x0000000f);
    for(uint8_t i=0;i<4;i++)
    {
        if(right == 0 || right > 0x0f)
        {
            right = 1;
        }
        ID_128Bit[i] = sum * right;
        right++;
        LOG(LEVEL_INFO,"ID_128Bit__%d:%08x\n",i,ID_128Bit[i]);
    }
    return 0;
}

void m2_insert_init(void)
{
    /*
    PF8     Interface1
    PB10    Interface2
    PI3     Interface3
    */
    EV_GPIO_Init(PF8, GPIO_MODE_INPUT, GPIO_PULLUP);
    EV_GPIO_Init(PB10, GPIO_MODE_INPUT, GPIO_PULLUP);
    EV_GPIO_Init(PI3, GPIO_MODE_INPUT, GPIO_PULLUP);

    M2InterfaceStruct.check_insert_sta = check_insert_sta;
    M2InterfaceStruct.IIC.ReadBuf = WM_IIC_Read;

    M2InterfaceStruct.IIC.WriteBuf = WM_IIC_Write;
}

static InsertStaEnum check_insert_sta(InterfaceNumEnum num)
{
    InsertStaEnum res = NoInsert;
    if(num == Interface1)
        res = (EV_GPIO_Get(PF8) == GPIO_PIN_RESET) ? IsInsert : NoInsert;
    else if(num == Interface2)
        res = (EV_GPIO_Get(PB10) == GPIO_PIN_RESET) ? IsInsert : NoInsert;
    else
        res = (EV_GPIO_Get(PI3) == GPIO_PIN_RESET) ? IsInsert : NoInsert;
    return res;
} 

#define M2_TASK_CHECK_INSERT_PRIO M2_TASK_CHECK_INSERT_PRIO_NUM
#define M2_STK_CHECK_INSERT_SIZE 1024 
uint32_t *M2_TASK_CHECK_INSERT_STK; 
void m2_interface_task(void *pdata)
{
    uint8_t res;
    uint8_t data[10];
    uint8_t err_cnt = 0;

    wifi_init();
    while(1)
    {
        //蓝牙检测模块插入
        if(M2InterfaceStruct.check_insert_sta(Interface1) == IsInsert
            &&ble_Init_sta==0)
        {
            EV_BLE_Init();
            EV_BLE_Recv_Init(ble_rx_callback);
            data[0] = 1;
            err_cnt = 0;
            do{
                res = M2InterfaceStruct.IIC.WriteBuf(0x27<<1, 80, data, 1);
                LOG(LEVEL_INFO, "ble init res %d\n", res);
                if(err_cnt++>10)
                    break;
                OSTimeDly(1000);
            }while(res);
			data[0] = 0;//0x78;
        	M2InterfaceStruct.IIC.WriteBuf(0x27<<1, 112, data, 1);
        	data[0] = 1;
        	M2InterfaceStruct.IIC.WriteBuf(0x27<<1, 113, data, 1); 
            data[0] = 1;    
        	M2InterfaceStruct.IIC.WriteBuf(0x27<<1, 114, data, 1); 
			if(res==0)
            {
                ble_Init_sta=1;
            }
        }else if(M2InterfaceStruct.check_insert_sta(Interface1) == NoInsert){
            ble_Init_sta=0;
        } 
        
        //WIFI检测模块插入
        if(M2InterfaceStruct.check_insert_sta(Interface2) == IsInsert
            &&wifi_Init_sta!=2)
        {
            if(wifi_Init_sta==0&&wifi_udp_sta_config()==0)
            {
                LOG(LEVEL_INFO, "wifi_udp_sta_config ok\n");
                wifi_Init_sta=1;
            }
            if(wifi_Init_sta==1)
            {
                if(wifi_udp_set()==0)
                {
                    LOG(LEVEL_INFO, "wifi_udp_set ok\n");
                    wifi_Init_sta=2;
                }else{
                    LOG(LEVEL_INFO, "wifi_udp_set fail\n");
                    wifi_Init_sta=0;
                }
            }
            //esp8266_tcp_client_set("172.16.8.242", 8086);
        }else if(M2InterfaceStruct.check_insert_sta(Interface2) == NoInsert){
            wifi_Init_sta=0;
        }

        //其他模块检测
        if(M2InterfaceStruct.check_insert_sta(Interface3) == IsInsert
            &&other_Init_sta==0)
        {
            //其他模块初始化
            other_Init_sta=1;
        }else if(M2InterfaceStruct.check_insert_sta(Interface3) == NoInsert){
            other_Init_sta=0;
        }

        OSTimeDly(10000);
    }
   
}

#define M2_TASK_NORMAL_PRIO M2_TASK_NORMAL_PRIO_NUM
#define M2_STK_NORMAL_SIZE 1024 
uint32_t *M2_TASK_NORMAL_STK; 
void m2_normal_task(void *pdata)
{
    uint8_t *start = 0;
    uint8_t wifi_recv_data[200];
    int wifi_recv_data_len=0;
	uint16_t wifi_recv_port = 0;
    uint32_t wifi_recv_ip = 0;
	
    uint16_t ant_id = 0;
    uint8_t ant_i = 0;

    while(1)
    {
        if(M2InterfaceStruct.check_insert_sta(Interface1) == IsInsert)
        {
            if(ble_Init_sta == 1)
            {
                for(uint8_t i = 0; i < ble_recv_cnt - 2; i++)
                {
                    if((ble_recv_data[i] == 'Y') && (ble_recv_data[i + 1] == 'Q'))
                    {
                        if(ble_recv_cnt - i < 14)
                            break;
                        start  = ble_recv_data + i;
                        // LOG(LEVEL_INFO, "Type : 0x%02X\n", start[2]);
                        // LOG(LEVEL_INFO, "Id   : 0x%04X\n", (start[3]) | (start[4] << 8));
                        // LOG(LEVEL_INFO, "Rssi : 0x%02X\n", start[5]);
                        // LOG(LEVEL_INFO, "Cnt %d\n", Ant_Devices_Cnt);
                        ant_id = (start[3]) | (start[4] << 8);
                        for(ant_i = 0; ant_i < Ant_Devices_Cnt; ant_i++)
                        {
                            if(ANT_Device_Info[ant_i].Id == ant_id)
                            {
                                ANT_Device_Info[ant_i].Rssi = start[5];
                                memcpy(ANT_Device_Info[ant_i].Data, start + 6, 8);
                                break;
                            }
                        }
                        if((Ant_Devices_Cnt == 0) || (ant_i == Ant_Devices_Cnt))
                        {
                            if(Ant_Devices_Cnt >= ANT_DEVICES_MAX)
                            {
                                memset(ble_recv_data, 0, BLE_RECV_DATA_MAX);
                                ble_recv_cnt = 0;   
                                break; 
                            }
                            ANT_Device_Info[Ant_Devices_Cnt].Type = start[2];
                            ANT_Device_Info[Ant_Devices_Cnt].Id = ant_id;
                            ANT_Device_Info[Ant_Devices_Cnt].Rssi = start[5];
                            memcpy(ANT_Device_Info[Ant_Devices_Cnt].Data, start + 6, 8);
                            Ant_Devices_Cnt++;
                        }

                        memset(ble_recv_data, 0, BLE_RECV_DATA_MAX);
                        ble_recv_cnt = 0;   
                        break; 
                    }
                }
            }
        }
        if(M2InterfaceStruct.check_insert_sta(Interface2) == IsInsert&&g_net_user_sta == WIFI_NETWORK)
        {
            // res = esp8266_get_time(&esp_time);
            // if(res == 0)
            // {
            //     memset(test_time_buf, 0, sizeof(test_time_buf));
            //     sprintf(test_time_buf, "%4d/%d/%d (%d)", esp_time.year, esp_time.month, esp_time.date, esp_time.weekday);
            //     sprintf(test_time_buf+ strlen(test_time_buf), " %02d:%02d:%02d\n", esp_time.hour, esp_time.min, esp_time.sec);
            //     LOG(LEVEL_INFO, "%s", test_time_buf);
            // }
            if(wifi_tcp_recving_sta==0)
            {
                memset(wifi_recv_data,0,sizeof(wifi_recv_data));
                wifi_recv_data_len = wifi_recv_user((char *)wifi_recv_data,sizeof(wifi_recv_data),&wifi_recv_ip,&wifi_recv_port);
                if(wifi_recv_data_len > 0)
                {
                    LOG(LEVEL_INFO,"normal:%s",wifi_recv_data);
                    LOG(LEVEL_INFO,"RECV_PORT:%d\n",wifi_recv_port);
                    if(wifi_udp_check_IP(wifi_recv_data,wifi_recv_data_len,wifi_recv_port))
                    {
                        user_eth_normal_recv(wifi_recv_data,wifi_recv_data_len,wifi_recv_ip);

                    }
                }
            }
            
            // wifi_send_user(test_send,2000,1,"172.16.8.61");
            //LOG(LEVEL_INFO,"WIFI_RUNING\n");
            
        }
        if(M2InterfaceStruct.check_insert_sta(Interface3) == IsInsert)
        {

        }
        OSTimeDly(100);
    }    
}

#define M2_TASK_BCAST_PRIO  M2_TASK_BCAST_PRIO_NUM
#define M2_STK_BCAST_SIZE   512 
uint32_t *M2_TASK_BCAST_STK; 
void m2_bcast_task(void *pdata)
{
    while(1)
    {
        //扫描
        if(udp_bcast_send_flag != 0)
        {
            wifi_udp_bcast();
            udp_bcast_send_flag--;
            OSTimeDly(1000);
        }
        OSTimeDly(10);
    }
}


void m2_task_creat(void)
{
    uint32_t cpu_sr;

    m2_insert_init();

	OS_ENTER_CRITICAL();	//关中断

    M2_TASK_CHECK_INSERT_STK = mymalloc(SRAMEX, M2_STK_CHECK_INSERT_SIZE* 4);
    OSTaskCreate(m2_interface_task, "m2_interface_task", 0, (uint32_t *)&M2_TASK_CHECK_INSERT_STK[M2_STK_CHECK_INSERT_SIZE - 1], M2_TASK_CHECK_INSERT_PRIO);

    M2_TASK_BCAST_STK = mymalloc(SRAMEX, M2_STK_BCAST_SIZE* 4);
    OSTaskCreate(m2_bcast_task, "m2_bcast_task", 0, (uint32_t *)&M2_TASK_BCAST_STK[M2_STK_BCAST_SIZE - 1], M2_TASK_BCAST_PRIO);
    
    M2_TASK_NORMAL_STK = mymalloc(SRAMEX, M2_STK_NORMAL_SIZE* 4);
    OSTaskCreate(m2_normal_task, "m2_normal_task", 0, (uint32_t *)&M2_TASK_NORMAL_STK[M2_STK_NORMAL_SIZE - 1], M2_TASK_NORMAL_PRIO);
    
    OSTaskSuspend(M2_TASK_BCAST_PRIO);

    OS_EXIT_CRITICAL();
}
