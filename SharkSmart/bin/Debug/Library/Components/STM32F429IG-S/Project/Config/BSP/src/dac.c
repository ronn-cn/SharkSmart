#include "dac.h"

#define DAC_DEFAULT_CONFIG      \
{                               \
    .Instance=DAC,              \
}

#define DAC_CH_DEFAULT_CONFIG                   \
{                                               \
    .DAC_Trigger=DAC_TRIGGER_NONE,              \
    .DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE, \
}


#define DAC_GPIO_DEFAULT_CONFIG \
{                               \
    .Pin=GPIO_PIN_4,            \
    .Mode=GPIO_MODE_ANALOG,     \
    .Pull=GPIO_NOPULL,          \
}


DAC_HandleTypeDef DAC_Handler = DAC_DEFAULT_CONFIG;
DAC_ChannelConfTypeDef DACCH1_Handler = DAC_CH_DEFAULT_CONFIG;
DAC_ChannelConfTypeDef DACCH2_Handler = DAC_CH_DEFAULT_CONFIG;

uint8_t EV_DAC_Init(DAC_TypeDef *Instance, uint8_t Pin, uint8_t Channel)
{
    DAC_HandleTypeDef * dac_handle;
    DAC_ChannelConfTypeDef *dac_ch_handle;
    GPIO_InitTypeDef GPIO_Initure = DAC_GPIO_DEFAULT_CONFIG;
    GPIO_TypeDef * gpiox;
    uint32_t ch = Channel;

    uint32_t pin = find_gpio_pin(Pin);
    gpiox = find_gpio_group(Pin);    
    
    if(Instance == DAC){
        __HAL_RCC_DAC_CLK_ENABLE();             //Ê¹ÄÜDACÊ±ÖÓ
        dac_handle = &DAC_Handler;

        if(ch == DAC_CHANNEL_1){
            dac_ch_handle = &DACCH1_Handler;
        }else if(ch == DAC_CHANNEL_2){
            dac_ch_handle = &DACCH2_Handler;
        }else{
            return 2;
        }
    }else{
        return 1;
    }

    GPIO_Initure.Pin = pin;
    HAL_GPIO_Init(gpiox, &GPIO_Initure);
    dac_handle->Instance=Instance;
    HAL_DAC_Init(dac_handle);

    return HAL_DAC_ConfigChannel(dac_handle,dac_ch_handle,ch);
}
uint8_t EV_DAC_Set(DAC_TypeDef *Instance, uint8_t Channel, uint16_t Value)
{
    return HAL_DAC_SetValue(&DAC_Handler,Channel,DAC_ALIGN_12B_R,Value);
}
uint8_t EV_DAC_Start(DAC_TypeDef *Instance, uint8_t Channel)
{
    return HAL_DAC_Start(&DAC_Handler,Channel);
}
uint8_t EV_DAC_Stop(DAC_TypeDef *Instance, uint8_t Channel)
{
    return HAL_DAC_Stop(&DAC_Handler,Channel);
}


