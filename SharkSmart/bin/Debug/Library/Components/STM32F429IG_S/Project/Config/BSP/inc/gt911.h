#ifndef __GT911_H_
#define __GT911_H_

#include "sys.h"

// #define WR_GT911_CONFIG             //向TP里写入配置表

//I2C读写命令    
#define GT_CMD_WR       0X28         //写命令
#define GT_CMD_RD       0X29         //读命令
   
//GT911 部分寄存器定义 
#define GT_CTRL_REG     0X8040       //GT911控制寄存器
#define GT_CFGS_REG     0X8047       //GT911配置起始地址寄存器
#define GT_CHECK_REG    0X80FF       //GT911校验和寄存器
#define GT_PID_REG      0X8140       //GT911产品ID寄存器
#define GT_GSTID_REG    0X814E       //GT911当前检测到的触摸情况

#define GT911_XY1_REG   0X8150       //GT911触摸点1
#define GT911_XY2_REG   0X8158       //GT911触摸点2
#define GT911_XY3_REG   0X8160       //GT911触摸点3
#define GT911_XY4_REG   0X8168       //GT911触摸点4
#define GT911_XY5_REG   0X8170       //GT911触摸点5

#define GT_INT          PEout(3)    //GT911中断引脚
#define GT_RST          PEout(2)    //GT911复位引脚


uint8_t GT911_Init(void);
uint8_t GT911_Scan(uint8_t mode);
void GT911_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
uint8_t GT911_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);


#endif 

