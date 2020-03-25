#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "sys.h"	    

//IO��������
#define CT_SDA_IN()  {GPIOI->MODER&=~(3<<(8*2));GPIOI->MODER|=0<<8*2;}	//PI8����ģʽ
#define CT_SDA_OUT() {GPIOI->MODER&=~(3<<(8*2));GPIOI->MODER|=1<<8*2;} 	//PI8���ģʽ
//IO��������	 
#define CT_IIC_SCL  PCout(13)  //SCL
#define CT_IIC_SDA  PIout(8)  //SDA	 
#define CT_READ_SDA PIin(8)   //����SDA 
 

//IIC���в�������
void CT_IIC_Init(void);                	//��ʼ��IIC��IO��				 
void CT_IIC_Start(void);				//����IIC��ʼ�ź�
void CT_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CT_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t CT_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
uint8_t CT_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void CT_IIC_Ack(void);					//IIC����ACK�ź�
void CT_IIC_NAck(void);					//IIC������ACK�ź�

#endif







