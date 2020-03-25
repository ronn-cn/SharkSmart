#include "usbd_cdc_vcp.h" 
#include "string.h"	
#include "stdarg.h"		 
#include "stdio.h"	 
#include "log.h"
#include "usbd_usr.h"
extern vu8 bDeviceState;

//USB���⴮��������ò���
LINE_CODING linecoding =
{
	115200,		//������
	0x00,   	//ֹͣλ,Ĭ��1λ
	0x00,   	//У��λ,Ĭ����
	0x08    	//����λ,Ĭ��8λ
}; 

uint8_t  USART_PRINTF_Buffer[USB_USART_REC_LEN];	//usb_printf���ͻ�����

//�����ƴ���1�������ݵķ���,������USB���⴮�ڽ��յ�������.
uint8_t USB_USART_RX_BUF[USB_USART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USB_USART_RX_STA=0;       				//����״̬���	 

 
extern uint8_t  APP_Rx_Buffer [];			//���⴮�ڷ��ͻ�����(��������) 
extern uint32_t APP_Rx_ptr_in;   			//���⴮�ڽ��ջ�����(�������Ե��Ե�����)

//���⴮�����ú���(��USB�ں˵���)
CDC_IF_Prop_TypeDef VCP_fops = 
{
	VCP_Init,
	VCP_DeInit,
	VCP_Ctrl,
	VCP_DataTx,
	VCP_DataRx
}; 

//��ʼ��VCP
//����ֵ:USBD_OK
uint16_t VCP_Init(void)
{ 
	return USBD_OK;
} 
//��λVCP
//����ֵ:USBD_OK
uint16_t VCP_DeInit(void)
{ 
	return USBD_OK;
} 
//����VCP������
//buf:�������ݻ�����/�������滺����
//len:���ݳ���
//����ֵ:USBD_OK
uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{ 
	switch (Cmd)
	{
		case SEND_ENCAPSULATED_COMMAND:break;   
		case GET_ENCAPSULATED_RESPONSE:break;  
		case SET_COMM_FEATURE:break;  
		case GET_COMM_FEATURE:break;  
 		case CLEAR_COMM_FEATURE:break;  
		case SET_LINE_CODING:
			linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
			linecoding.format = Buf[4];
			linecoding.paritytype = Buf[5];
			linecoding.datatype = Buf[6]; 

			APP_Rx_ptr_in = 0;
			break; 
		case GET_LINE_CODING:
			Buf[0] = (uint8_t)(linecoding.bitrate);
			Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
			Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
			Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
			Buf[4] = linecoding.format;
			Buf[5] = linecoding.paritytype;
			Buf[6] = linecoding.datatype; 
			break; 
		case SET_CONTROL_LINE_STATE:break;   
		case SEND_BREAK:break;   
		default:break;  
	} 
	return USBD_OK;
}
//����һ���ֽڸ����⴮��(��������)
//data:Ҫ���͵�����
//����ֵ:USBD_OK
uint16_t VCP_DataTx (uint8_t data)
{  
	APP_Rx_Buffer[APP_Rx_ptr_in]=data;	//д�뷢��buf
	APP_Rx_ptr_in++;  					//дλ�ü�1
	if(APP_Rx_ptr_in==APP_RX_DATA_SIZE)	//����buf��С��,����.
	{
		APP_Rx_ptr_in = 0;
	}   
	return USBD_OK;
} 
//�����USB���⴮�ڽ��յ�������
//databuffer:���ݻ�����
//Nb_bytes:���յ����ֽ���.
//����ֵ:USBD_OK
uint16_t usb_vcp_len = 0;
uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
	for(uint16_t i = 0; i < Len; i++)
	{
		USB_USART_RX_BUF[usb_vcp_len++] = Buf[i];
	}
	return USBD_OK;
}
uint16_t usb_vcp_receive(uint8_t *buf)
{
	uint16_t len = 0;
	memcpy(buf, USB_USART_RX_BUF, usb_vcp_len);
	len = usb_vcp_len;
	usb_vcp_len = 0;
	return len;
}
//usb���⴮��,printf ����
//ȷ��һ�η������ݲ���USB_USART_REC_LEN�ֽ�
void usb_printf(const char* fmt,...)  
{  

	if((g_usb_parameter.mode != HID_CDC) || 
	   (g_usb_parameter.mode != HID_MSC_CDC))
	   return;

	if(bDeviceState == 0)
	return;

	uint16_t i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART_PRINTF_Buffer,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART_PRINTF_Buffer);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
		VCP_DataTx(USART_PRINTF_Buffer[j]); 
	}
} 



























