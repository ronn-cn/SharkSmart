#ifndef _GPIO_H
#define _GPIO_H
#include "sys.h"

uint8_t EV_GPIO_Init(uint8_t Pin, uint8_t Mode, uint8_t Pull);
uint8_t EV_GPIO_Set(uint8_t Pin, uint8_t State);
uint8_t EV_GPIO_Get(uint8_t Pin);
uint8_t EV_GPIO_Toggle(uint8_t Pin);
#endif
