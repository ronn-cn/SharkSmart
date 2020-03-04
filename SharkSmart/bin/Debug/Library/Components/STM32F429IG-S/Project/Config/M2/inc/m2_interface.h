#ifndef __M2_INTERFACE_H
#define __M2_INTERFACE_H
#include "sys.h"
#include "rtos.h"
#include "esp8266.h"


typedef enum{
    Interface1 = 0x01,
    Interface2 = 0x02,
    Interface3 = 0x03
}InterfaceNumEnum;

typedef enum{
    NoInsert = 0x00,
    IsInsert = 0x01
}InsertStaEnum;

typedef struct{
    InsertStaEnum (*check_insert_sta)(InterfaceNumEnum num);
    struct{
        uint8_t (*ReadBuf)(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
        uint8_t (*WriteBuf)(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
    }IIC;
}__M2InterfaceStruct;




void m2_insert_init(void);
void m2_task_creat(void);



//插入模块初始化标志
extern uint8_t ble_Init_sta;
extern uint8_t wifi_Init_sta;
extern uint8_t other_Init_sta;

extern uint8_t wifi_tcp_recving_sta;        //wifi TCP 数据正在接收中的标志




struct ANT_Device_Info_t
{
    uint8_t Type;
    uint16_t Id;
    int8_t Rssi;

    uint8_t Data[8];
};

#define ANT_DEVICES_MAX 5

extern uint8_t Ant_Devices_Cnt;
extern struct ANT_Device_Info_t ANT_Device_Info[ANT_DEVICES_MAX];



void EV_GET_MPU_ID(uint32_t MPU_ID_Out[3]);
uint16_t EV_Get_16Bit_uuid(uint32_t MPU_ID_In[3]);
uint8_t EV_Get_128Bit_uuid(uint32_t MPU_ID_In[3],uint32_t ID_128Bit[4]);

#endif