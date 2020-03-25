#ifndef _EXTI_H
#define _EXTI_H
#include "sys.h"	

uint8_t EV_EXTI_Init(uint8_t Pin, uint8_t Mode,  void (*Callback)(void));

#endif
