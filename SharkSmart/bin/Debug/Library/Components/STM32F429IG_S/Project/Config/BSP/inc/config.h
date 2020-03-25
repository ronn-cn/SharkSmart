#ifndef  __CONFIG_H__
#define  __CONFIG_H__
#include "stmflash.h"

#define CONFIG_BASEADDR		  0x08004000
#define CONFIG_PARTITION_SIZE 0x4000	//16KB 扇区 2


void Config_Write(uint32_t AddrOffset,uint32_t *pBuffer,uint32_t NumToWrite);		
void Config_Read(uint32_t AddrOffset,uint32_t *pBuffer,uint32_t NumToRead);   		
void Config_Erase(void);


/*
config分区共128KB字节
偏移范围0x0000-0x3fff
boot相关参数放入
0x0000-0x03ff区域内 1024 BYTE

APP以及其他参数放入
0x0400-0xffff区域内 1024 * 127 BYTE

0x0400-0x0420	共32字节		存放本机别名
*/

//存放别名
#define 	ALIAS_ADDR		0x0400		//偏移地址
#define		ALIAS_SIZE		8			//长度  * 4 Byte

#define 	HARDWARE_VERSION_NUMBER			0x00000002	
#define		FIRMWARE_VERSION_ADDR			0x0440		//存放固件版本号
#define		FIRMWARE_VERSION_SIZE			1			//4个字节


#define     RECOVERY_FLAG_ADDR              0x0200
#define     RECOVERY_FLAG_AIZE              1

#endif 
