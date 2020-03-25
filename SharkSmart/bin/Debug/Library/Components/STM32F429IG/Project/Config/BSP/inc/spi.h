#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

uint8_t EV_SPI_Init(SPI_TypeDef * Instance, uint8_t ClkPin, uint8_t MosiPin, uint8_t MisoPin, uint32_t BaudRatePrescaler, uint32_t CLKPolarity, uint32_t CLKPhase);
uint8_t EV_SPI_Send(SPI_TypeDef * Instance, uint8_t *Data, uint16_t Len);
uint8_t EV_SPI_Recv(SPI_TypeDef * Instance, uint8_t *Data, uint16_t Len);

#endif
