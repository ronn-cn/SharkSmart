#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H
#include "sys.h"
#include "usbd_cdc_core.h"
#include "usbd_conf.h"

#define USB_USART_REC_LEN	 	200				//USB���ڽ��ջ���������ֽ���

extern uint8_t  USB_USART_RX_BUF[USB_USART_REC_LEN]; //���ջ���,���USB_USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USB_USART_RX_STA;   					//����״̬���	


//USB���⴮��������ò���
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
}LINE_CODING;
 


uint16_t VCP_Init     (void);
uint16_t VCP_DeInit   (void);
uint16_t VCP_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
uint16_t VCP_DataTx   (uint8_t data);
uint16_t VCP_DataRx   (uint8_t* Buf, uint32_t Len);
uint16_t usb_vcp_receive(uint8_t *buf);
void usb_printf(const char* fmt,...); 
#endif 
















