#include "gpio.h"
#include "stm32f4xx_hal_gpio.h"

#define GPIO_DEFAULT_CONFIG     \
{                                   \
    .Mode=GPIO_MODE_OUTPUT_PP,      \
    .Pull=GPIO_PULLUP,              \
    .Speed=GPIO_SPEED_HIGH,         \
}

uint8_t EV_GPIO_Init(uint8_t Pin, uint8_t Mode, uint8_t Pull)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();    
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

    GPIO_TypeDef * gpiox;
    if(Pin > PI15)
    {
        return 1;
    }
    uint32_t pin = find_gpio_pin(Pin);
    gpiox = find_gpio_group(Pin);    

    GPIO_InitTypeDef GPIO_Initure = GPIO_DEFAULT_CONFIG;

    GPIO_Initure.Pin = pin;
    GPIO_Initure.Mode = Mode;
    GPIO_Initure.Pull = Pull;
    HAL_GPIO_Init(gpiox,&GPIO_Initure);

    return 0;
}

uint8_t EV_GPIO_Set(uint8_t Pin, uint8_t State)
{
    GPIO_TypeDef * gpiox;
    uint32_t pin = find_gpio_pin(Pin);
    gpiox = find_gpio_group(Pin);  

    HAL_GPIO_WritePin(gpiox, pin, State);

    return 0;
}

uint8_t EV_GPIO_Get(uint8_t Pin)
{
    GPIO_TypeDef * gpiox;
    uint32_t pin = find_gpio_pin(Pin);
    gpiox = find_gpio_group(Pin);  

    return HAL_GPIO_ReadPin(gpiox, pin);
}
uint8_t EV_GPIO_Toggle(uint8_t Pin)
{
    GPIO_TypeDef * gpiox;
    uint32_t pin = find_gpio_pin(Pin);
    gpiox = find_gpio_group(Pin);  

    HAL_GPIO_TogglePin(gpiox, pin);

    return 0;
}