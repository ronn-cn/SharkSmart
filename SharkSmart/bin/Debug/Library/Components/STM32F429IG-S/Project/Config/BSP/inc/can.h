#ifndef __CAN_H
#define __CAN_H
#include "sys.h"

uint8_t EV_CAN_Init(CAN_TypeDef * Instance, uint8_t TxPin, uint8_t RxPin, uint32_t Id, uint32_t Speed, uint8_t (* Recv_Callback)(uint8_t *Data, uint8_t Len));
uint8_t EV_CAN_Send(CAN_TypeDef * Instance, uint32_t Id, uint8_t *Data, uint8_t Len);
#endif
