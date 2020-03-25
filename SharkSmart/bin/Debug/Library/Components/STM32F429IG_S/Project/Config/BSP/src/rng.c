#include "rng.h"

RNG_HandleTypeDef RNG_Handler; 

uint8_t EV_RNG_Init(RNG_TypeDef *Instance)
{
     __HAL_RCC_RNG_CLK_ENABLE();

    RNG_Handler.Instance=Instance;
    return HAL_RNG_Init(&RNG_Handler);
}

uint32_t EV_RNG_Get(RNG_TypeDef *Instance)
{
    return HAL_RNG_GetRandomNumber(&RNG_Handler);
}