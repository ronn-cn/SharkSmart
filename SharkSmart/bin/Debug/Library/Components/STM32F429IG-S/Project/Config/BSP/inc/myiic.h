#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"

uint8_t EV_I2C_Init(I2C_TypeDef * Instance, uint8_t SclPin, uint8_t SdaPin);
uint8_t EV_I2C_Read(I2C_TypeDef * Instance, uint8_t I2CAddr, uint8_t *Data, uint16_t Len);
uint8_t EV_I2C_Write(I2C_TypeDef * Instance, uint8_t I2CAddr, uint8_t *Data, uint16_t Len);
uint8_t EV_I2C_ReadReg(I2C_TypeDef * Instance, uint8_t I2CAddr, uint16_t RegAddr, uint32_t RegAddrSize, uint8_t *Data, uint16_t Len);
uint8_t EV_I2C_WriteReg(I2C_TypeDef * Instance, uint8_t I2CAddr, uint16_t RegAddr, uint32_t RegAddrSize, uint8_t *Data, uint16_t Len);
#endif

