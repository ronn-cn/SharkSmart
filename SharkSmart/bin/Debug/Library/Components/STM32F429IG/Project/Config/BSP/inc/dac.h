#ifndef __DAC_H
#define __DAC_H
#include "sys.h"

uint8_t EV_DAC_Init(DAC_TypeDef *Instance, uint8_t Pin, uint8_t Channel);
uint8_t EV_DAC_Set(DAC_TypeDef *Instance, uint8_t Channel, uint16_t Value);
uint8_t EV_DAC_Start(DAC_TypeDef *Instance, uint8_t Channel);
uint8_t EV_DAC_Stop(DAC_TypeDef *Instance, uint8_t Channel);

#endif
