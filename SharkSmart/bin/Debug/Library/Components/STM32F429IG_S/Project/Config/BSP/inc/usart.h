#ifndef _USART_H
#define _USART_H
#include "sys.h"

uint8_t EV_UART_Init(USART_TypeDef *Instance, uint8_t TxPin, uint8_t RxPin, uint32_t BaudRate, uint8_t (*Recv_Callback)(uint8_t *Data, uint8_t Len));
uint8_t EV_UART_Send(USART_TypeDef *Instance, uint8_t *Data, uint16_t Len);


void UART1_Init(void);

#endif
