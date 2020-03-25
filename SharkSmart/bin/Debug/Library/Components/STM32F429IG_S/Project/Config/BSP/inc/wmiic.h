#ifndef _WMWM_IIC_H
#define _WMWM_IIC_H
#include "sys.h"

void WM_IIC_Init(void);
uint8_t WM_IIC_Write(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
uint8_t WM_IIC_Read(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
#endif

