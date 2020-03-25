#ifndef __ADC_H
#define __ADC_H
#include "sys.h"


uint8_t EV_ADC_Init(ADC_TypeDef * Instance, uint8_t Pin, uint8_t Channel);
uint16_t EV_ADC_Get(ADC_TypeDef * Instance, uint8_t Channel);




void BSP_ADC_Init(void);
short   Get_Temprate(void);
uint16_t Get_5V_mV(void);
uint16_t Get_5V_mA(void);
#endif 
