#ifndef __RNG_H
#define __RNG_H
#include "sys.h"

uint8_t EV_RNG_Init(RNG_TypeDef *Instance);
uint32_t EV_RNG_Get(RNG_TypeDef *Instance);
#endif

