#include "adc.h"
#include "delay.h"
#include "log.h"

#include "stm32f429xx.h"


#define ADC_GPIO_DEFAULT_CONFIG     \
{                                   \
    .Mode=GPIO_MODE_ANALOG,         \
    .Pull=GPIO_NOPULL,              \
}

#define ADC_DEFAULT_CONFIG                          \
{                                                   \
    .Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4,  \
    .Init.Resolution=ADC_RESOLUTION_12B,            \
    .Init.DataAlign=ADC_DATAALIGN_RIGHT,            \
    .Init.ScanConvMode=DISABLE,                     \
    .Init.EOCSelection=DISABLE,                     \
    .Init.ContinuousConvMode=DISABLE,               \
    .Init.NbrOfConversion=1,                        \
    .Init.DiscontinuousConvMode=DISABLE,            \
    .Init.NbrOfDiscConversion=0,                    \
    .Init.ExternalTrigConv=ADC_SOFTWARE_START,      \
    .Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE,   \
    .Init.DMAContinuousRequests=DISABLE,            \
}

#define ADC_CHANNEL_DEFAULT_CONFIG          \
{                                           \
    .Rank=1,                                \
    .SamplingTime=ADC_SAMPLETIME_480CYCLES, \
    .Offset=0,                              \
}

ADC_HandleTypeDef ADC1_Handler = ADC_DEFAULT_CONFIG;
ADC_HandleTypeDef ADC2_Handler = ADC_DEFAULT_CONFIG;
ADC_HandleTypeDef ADC3_Handler = ADC_DEFAULT_CONFIG;

ADC_ChannelConfTypeDef ADC_ChanConf = ADC_CHANNEL_DEFAULT_CONFIG;

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();
    
}

void BSP_ADC_Init(void)
{

    EV_ADC_Init(ADC1, PA3, ADC_CHANNEL_3);
}
//得到温度值
//返回值:温度值(扩大了100倍,单位:℃.)
short Get_Temprate(void)
{
	return 4000;
}

uint16_t Get_5V_mV(void)
{
	uint32_t adcx;
    uint16_t result;
	adcx=EV_ADC_Get(ADC1, ADC_CHANNEL_3);
	result=(uint16_t)adcx*(3300.0/4096.0*2.0);		//电压值
	return result;
}
uint16_t Get_5V_mA(void)
{
	return 800;
}

uint8_t EV_ADC_Init(ADC_TypeDef * Instance, uint8_t Pin, uint8_t Channel)
{
    ADC_HandleTypeDef * adc_handle;
    GPIO_InitTypeDef GPIO_Initure = ADC_GPIO_DEFAULT_CONFIG;
    GPIO_TypeDef * gpiox;

    uint32_t pin = find_gpio_pin(Pin);
    gpiox = find_gpio_group(Pin);

    if(Instance == ADC1){
        adc_handle = &ADC1_Handler;
        __HAL_RCC_ADC1_CLK_ENABLE();
    }else if(Instance == ADC2){
        adc_handle = &ADC2_Handler;
        __HAL_RCC_ADC2_CLK_ENABLE();
    }else if(Instance == ADC3){
        adc_handle = &ADC3_Handler;
        __HAL_RCC_ADC3_CLK_ENABLE();
    }else{
        return 1;
    }
    GPIO_Initure.Pin=pin;
    HAL_GPIO_Init(gpiox,&GPIO_Initure);

    adc_handle->Instance=Instance;
    return HAL_ADC_Init(adc_handle);
}
uint16_t EV_ADC_Get(ADC_TypeDef * Instance, uint8_t Channel)
{
    ADC_HandleTypeDef * adc_handle;
    ADC_ChanConf.Channel=Channel;

    if(Instance == ADC1){
        adc_handle = &ADC1_Handler;
    }else if(Instance == ADC2){
        adc_handle = &ADC2_Handler;
    }else if(Instance == ADC3){
        adc_handle = &ADC3_Handler;
    }else{
        return 0xFFFF;
    }

    HAL_ADC_ConfigChannel(adc_handle,&ADC_ChanConf);
    HAL_ADC_Start(adc_handle);
    HAL_ADC_PollForConversion(adc_handle,10);
	return (uint16_t)HAL_ADC_GetValue(adc_handle);
}