#ifndef  __CONFIG_H__
#define  __CONFIG_H__
#include "stmflash.h"

#define CONFIG_BASEADDR		  0x08004000
#define CONFIG_PARTITION_SIZE 0x4000	//16KB ���� 2


void Config_Write(uint32_t AddrOffset,uint32_t *pBuffer,uint32_t NumToWrite);		
void Config_Read(uint32_t AddrOffset,uint32_t *pBuffer,uint32_t NumToRead);   		
void Config_Erase(void);


/*
config������128KB�ֽ�
ƫ�Ʒ�Χ0x0000-0x3fff
boot��ز�������
0x0000-0x03ff������ 1024 BYTE

APP�Լ�������������
0x0400-0xffff������ 1024 * 127 BYTE

0x0400-0x0420	��32�ֽ�		��ű�������
*/

//��ű���
#define 	ALIAS_ADDR		0x0400		//ƫ�Ƶ�ַ
#define		ALIAS_SIZE		8			//����  * 4 Byte

#define 	HARDWARE_VERSION_NUMBER			0x00000002	
#define		FIRMWARE_VERSION_ADDR			0x0440		//��Ź̼��汾��
#define		FIRMWARE_VERSION_SIZE			1			//4���ֽ�


#define     RECOVERY_FLAG_ADDR              0x0200
#define     RECOVERY_FLAG_AIZE              1

#endif 
