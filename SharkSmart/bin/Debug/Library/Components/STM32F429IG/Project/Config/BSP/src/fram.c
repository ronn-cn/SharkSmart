#include "fram.h"
#include "delay.h"
#include "myiic.h"

#define MEM_BYTE_MAX	0x2000


uint8_t FRAM_ReadOneByte(uint16_t ReadAddr);
void FRAM_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);
void FRAM_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);
uint32_t FRAM_ReadLenByte(uint16_t ReadAddr,uint8_t Len);
void FRAM_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);
void FRAM_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);

//初始化IIC接口
uint8_t FRAM_Init(void)
{
	EV_I2C_Init(I2C3, PA8, PC9);
	return FRAM_Check();
}
uint8_t FRAM_ReadOneByte(uint16_t ReadAddr)
{		
	uint8_t read_data = 0;
	EV_I2C_ReadReg(I2C3, 0xA1, ReadAddr, I2C_MEMADD_SIZE_16BIT, &read_data, 1);
	return read_data;
}
void FRAM_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{	
	EV_I2C_WriteReg(I2C3, 0xA0, WriteAddr, I2C_MEMADD_SIZE_16BIT, &DataToWrite, 1);
}
void FRAM_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{  	
	uint8_t t;
	for(t=0;t<Len;t++)
	{
		FRAM_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}
uint32_t FRAM_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{  	
	uint8_t t;
	uint32_t temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=FRAM_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
uint8_t FRAM_Check(void)
{
	
	uint8_t temp;
	temp=FRAM_ReadOneByte(MEM_BYTE_MAX - 1);//避免每次开机都写FRAM			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		FRAM_WriteOneByte(MEM_BYTE_MAX - 1,0X55);
	    temp=FRAM_ReadOneByte(MEM_BYTE_MAX - 1);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}
void FRAM_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=FRAM_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
void FRAM_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		FRAM_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

void User_ParameterSave(uint16_t addr, uint8_t *data, uint8_t data_len)
{
	FRAM_Write(addr, data, data_len);
}
void User_ParameterRead(uint16_t addr, uint8_t *data, uint8_t data_len)
{
	FRAM_Read(addr, data, data_len);	
}
