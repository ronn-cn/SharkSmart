#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "stm32f429xx.h"

uint8_t EV_TIM_Init(TIM_TypeDef * Instance, uint32_t Time, void (*Callback)(void));
uint8_t EV_TIM_Start(TIM_TypeDef * Instance);
uint8_t EV_TIM_Stop(TIM_TypeDef * Instance);

uint8_t EV_PWM_Init(TIM_TypeDef * Instance, uint8_t Pin, uint8_t Channel, uint32_t Fre, uint32_t Dutymax);
uint8_t EV_PWM_Set(TIM_TypeDef * Instance, uint8_t Channel, uint16_t Duty);
#endif

