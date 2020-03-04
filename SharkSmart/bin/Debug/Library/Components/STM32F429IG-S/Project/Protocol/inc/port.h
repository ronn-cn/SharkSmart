#ifndef __PORT_H
#define __PORT_H
#include "sys.h"
#include "can.h"
#include "usbd_usr.h"
#include "spi.h"
#include "myiic.h"
//总线
#define     IIC1_DEV	0x01
#define		IIC2_DEV 	0x02
#define 	IIC3_DEV	0x03
#define		IIC4_DEV	0x04
#define     SPI1_DEV    0x05
#define 	SPI3_DEV	0x06

//返回值状态码
#define		ERR_MPU_TRUE		0		//成功
#define		ERR_MPU_INITFLASE	1		//MPU6050初始化失败:MPU6050_ON_OFF开关未开启
#define 	ERR_MPU_BUSFLASE	2		//MPU6050初始化失败:总线选择错误
#define		ERR_MPU_IDFLASE		3		//MPU6050初始化失败:读取器件ID错误
#define		ERR_MPU_GYROFLASE	4		//读取陀螺仪数据失败
#define		ERR_MPU_ACCFLASE	5		//读取加速计数据失败
#define 	ERR_MPU_TEMPFLASE 	6		//读取温度失败

#endif
