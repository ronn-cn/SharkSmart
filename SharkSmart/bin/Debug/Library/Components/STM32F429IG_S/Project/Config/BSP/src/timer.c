#include "timer.h"
#include "sys.h"
#include "log.h"
#include "stm32f4xx_hal.h"


#define TIM_TIMER_DEFAULT_CONFIG                \
{                                               \
    .Init.CounterMode=TIM_COUNTERMODE_UP,       \
    .Init.ClockDivision=TIM_CLOCKDIVISION_DIV1, \
    .Init.Prescaler=47999,                      \
}



#define PWM_GPIO_DEFAULT_CONFIG     \
{                                   \
    .Mode=GPIO_MODE_AF_PP,          \
    .Pull=GPIO_PULLUP,              \
    .Speed=GPIO_SPEED_HIGH,         \
}

#define PWM_CH_DEFAULT_CONFIG           \
{                                       \
    .OCMode=TIM_OCMODE_PWM1,            \
    .Pulse=500,                         \
    .OCPolarity=TIM_OCPOLARITY_HIGH,    \
}

TIM_HandleTypeDef TIM1_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM2_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM3_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM4_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM5_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM6_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM7_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM8_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM9_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM10_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM11_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM12_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM13_Handler = TIM_TIMER_DEFAULT_CONFIG;
TIM_HandleTypeDef TIM14_Handler = TIM_TIMER_DEFAULT_CONFIG;

TIM_OC_InitTypeDef PUB_TIM_CHHandler = PWM_CH_DEFAULT_CONFIG;

void system_tim_call_back_null(void)
{
    
}

void (* system_tim1_call_back)(void) = system_tim_call_back_null;
void (* system_tim2_call_back)(void) = system_tim_call_back_null;
void (* system_tim3_call_back)(void) = system_tim_call_back_null;
void (* system_tim4_call_back)(void) = system_tim_call_back_null;
void (* system_tim5_call_back)(void) = system_tim_call_back_null;
void (* system_tim6_call_back)(void) = system_tim_call_back_null;
void (* system_tim7_call_back)(void) = system_tim_call_back_null;
void (* system_tim8_call_back)(void) = system_tim_call_back_null;
void (* system_tim9_call_back)(void) = system_tim_call_back_null;
void (* system_tim10_call_back)(void) = system_tim_call_back_null;
void (* system_tim11_call_back)(void) = system_tim_call_back_null;
void (* system_tim12_call_back)(void) = system_tim_call_back_null;
void (* system_tim13_call_back)(void) = system_tim_call_back_null;
void (* system_tim14_call_back)(void) = system_tim_call_back_null;


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM1){
        HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    }else if(htim->Instance==TIM2){
        HAL_NVIC_SetPriority(TIM2_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }else if(htim->Instance==TIM3){
        HAL_NVIC_SetPriority(TIM3_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);        
    }else if(htim->Instance==TIM4){
        HAL_NVIC_SetPriority(TIM4_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);        
    }else if(htim->Instance==TIM5){
        HAL_NVIC_SetPriority(TIM5_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM5_IRQn);   
    }else if(htim->Instance==TIM6){
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn); 
    }else if(htim->Instance==TIM7){
        HAL_NVIC_SetPriority(TIM7_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);         
    }else if(htim->Instance==TIM8){
        HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);         
    }else if(htim->Instance==TIM9){
        HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);         
    }else if(htim->Instance==TIM10){
        HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);         
    }else if(htim->Instance==TIM11){
        HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);         
    }else if(htim->Instance==TIM12){
        HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);         
    }else if(htim->Instance==TIM13){        
        HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn,2,2);
        HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
    }else if(htim->Instance==TIM14){
        HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn,1,2);    //设置中断优先级，抢占优先级1，子优先级3
        HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);          //开启ITM3中断  
    }

}


void TIM1_UP_TIM10_IRQHandler(void){HAL_TIM_IRQHandler(&TIM1_Handler);HAL_TIM_IRQHandler(&TIM10_Handler);}
void TIM2_IRQHandler(void){HAL_TIM_IRQHandler(&TIM2_Handler);}
void TIM3_IRQHandler(void){HAL_TIM_IRQHandler(&TIM3_Handler);}
void TIM4_IRQHandler(void){HAL_TIM_IRQHandler(&TIM4_Handler);}
void TIM5_IRQHandler(void){HAL_TIM_IRQHandler(&TIM5_Handler);}
void TIM6_DAC_IRQHandler(void){HAL_TIM_IRQHandler(&TIM6_Handler);}
void TIM7_IRQHandler(void){HAL_TIM_IRQHandler(&TIM7_Handler);}
void TIM8_UP_TIM13_IRQHandler(void){HAL_TIM_IRQHandler(&TIM8_Handler);HAL_TIM_IRQHandler(&TIM13_Handler);}
void TIM1_BRK_TIM9_IRQHandler(void){HAL_TIM_IRQHandler(&TIM9_Handler);}
void TIM1_TRG_COM_TIM11_IRQHandler(void){HAL_TIM_IRQHandler(&TIM11_Handler);}
void TIM8_BRK_TIM12_IRQHandler(void){HAL_TIM_IRQHandler(&TIM12_Handler);}
void TIM8_TRG_COM_TIM14_IRQHandler(void){HAL_TIM_IRQHandler(&TIM14_Handler);}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 
    if(htim->Instance==(TIM1)){
        system_tim1_call_back();
    }else if(htim->Instance==(TIM2)){
        system_tim2_call_back();
    }else if(htim->Instance==(TIM3)){
        system_tim3_call_back();
    }else if(htim->Instance==(TIM4)){
        system_tim4_call_back();   
    }else if(htim->Instance==(TIM5)){
        system_tim5_call_back();
    }else if(htim->Instance==(TIM6)){
        system_tim6_call_back();
    }else if(htim->Instance==(TIM7)){
        system_tim7_call_back();
    }else if(htim->Instance==(TIM8)){
        system_tim8_call_back();
    }else if(htim->Instance==(TIM9)){
        system_tim9_call_back();
    }else if(htim->Instance==(TIM10)){
        system_tim10_call_back();
    }else if(htim->Instance==(TIM11)){
        system_tim11_call_back();
    }else if(htim->Instance==(TIM12)){
        system_tim12_call_back();
    }else if(htim->Instance==(TIM13)){
        system_tim13_call_back();
    }else if(htim->Instance==(TIM14)){     
        system_tim14_call_back();
    }
}


uint8_t EV_TIM_Init(TIM_TypeDef * Instance, uint32_t Time, void (*Callback)(void))
{
    uint16_t arr;

    TIM_HandleTypeDef *TIM_Handler;
    if(Instance == TIM1){
        __HAL_RCC_TIM1_CLK_ENABLE();
        TIM_Handler = &TIM1_Handler;
        TIM_Handler->Instance = TIM1;
        system_tim1_call_back = Callback;  
    }else if(Instance == TIM2){
        __HAL_RCC_TIM2_CLK_ENABLE();
        TIM_Handler = &TIM2_Handler;
        TIM_Handler->Instance = TIM2;
        system_tim2_call_back = Callback;
    }else if(Instance == TIM3){
        __HAL_RCC_TIM3_CLK_ENABLE();
        TIM_Handler = &TIM3_Handler;
        TIM_Handler->Instance = TIM3;
        system_tim3_call_back = Callback;
    }else if(Instance == TIM4){
        __HAL_RCC_TIM4_CLK_ENABLE();
        TIM_Handler = &TIM4_Handler;
        TIM_Handler->Instance = TIM4;
        system_tim4_call_back = Callback;
    }else if(Instance == TIM5){
        __HAL_RCC_TIM5_CLK_ENABLE();
        TIM_Handler = &TIM5_Handler;
        TIM_Handler->Instance = TIM5;
        system_tim5_call_back = Callback; 
    }else if(Instance == TIM6){
        __HAL_RCC_TIM6_CLK_ENABLE();
        TIM_Handler = &TIM6_Handler;
        TIM_Handler->Instance = TIM6;
        system_tim6_call_back = Callback;
    }else if(Instance == TIM7){
        __HAL_RCC_TIM7_CLK_ENABLE();
        TIM_Handler = &TIM7_Handler;
        TIM_Handler->Instance = TIM7;
        system_tim7_call_back = Callback;
    }else if(Instance == TIM8){
        __HAL_RCC_TIM8_CLK_ENABLE();
        TIM_Handler = &TIM8_Handler;
        TIM_Handler->Instance = TIM8;
        system_tim8_call_back = Callback;
    }else if(Instance == TIM9){
        __HAL_RCC_TIM9_CLK_ENABLE();
        TIM_Handler = &TIM9_Handler;
        TIM_Handler->Instance = TIM9;
        system_tim9_call_back = Callback;
    }else if(Instance == TIM10){
        __HAL_RCC_TIM10_CLK_ENABLE();
        TIM_Handler = &TIM10_Handler;
        TIM_Handler->Instance = TIM10;
        system_tim10_call_back = Callback;
    }else if(Instance == TIM11){ 
        __HAL_RCC_TIM11_CLK_ENABLE();
        TIM_Handler = &TIM11_Handler;
        TIM_Handler->Instance = TIM11;
        system_tim11_call_back = Callback;
    }else if(Instance == TIM12){
        __HAL_RCC_TIM12_CLK_ENABLE();
        TIM_Handler = &TIM12_Handler;
        TIM_Handler->Instance = TIM12;
        system_tim12_call_back = Callback;
    }else if(Instance == TIM13){
        __HAL_RCC_TIM13_CLK_ENABLE();
        TIM_Handler = &TIM13_Handler;
        TIM_Handler->Instance = TIM13;
        system_tim13_call_back = Callback;
    }else if(Instance == TIM14){ 
        __HAL_RCC_TIM14_CLK_ENABLE();
        TIM_Handler = &TIM14_Handler;
        TIM_Handler->Instance = TIM14;
        system_tim14_call_back = Callback;
    }else{
        return 1;
    }
    if((Instance == TIM1)    \
    ||(Instance == TIM8) \
    ||(Instance == TIM9) \
    ||(Instance == TIM10) \
    ||(Instance == TIM11))
    {
        arr = (Time) * 4 - 1;  
    }
    else
    {
        arr = (Time) * 2 - 1;  
    }
    TIM_Handler->Init.Period = arr;

    HAL_TIM_Base_Init(TIM_Handler);
    __HAL_TIM_CLEAR_FLAG(TIM_Handler,TIM_IT_UPDATE);

    return 0;
}


uint8_t EV_TIM_Start(TIM_TypeDef * Instance)
{
    TIM_HandleTypeDef *TIM_Handler;
    if(Instance == TIM1){
        TIM_Handler = &TIM1_Handler;  
    }else if(Instance == TIM2){
        TIM_Handler = &TIM2_Handler;
    }else if(Instance == TIM3){
        TIM_Handler = &TIM3_Handler;
    }else if(Instance == TIM4){
        TIM_Handler = &TIM4_Handler;
    }else if(Instance == TIM5){
        TIM_Handler = &TIM5_Handler;
    }else if(Instance == TIM6){
        TIM_Handler = &TIM6_Handler;
    }else if(Instance == TIM7){
        TIM_Handler = &TIM7_Handler;
    }else if(Instance == TIM8){
        TIM_Handler = &TIM8_Handler;
    }else if(Instance == TIM9){
        TIM_Handler = &TIM9_Handler;
    }else if(Instance == TIM10){
        TIM_Handler = &TIM10_Handler;
    }else if(Instance == TIM11){ 
        TIM_Handler = &TIM11_Handler;
    }else if(Instance == TIM12){
        TIM_Handler = &TIM12_Handler;
    }else if(Instance == TIM13){
        TIM_Handler = &TIM13_Handler;
    }else if(Instance == TIM14){ 
        TIM_Handler = &TIM14_Handler;
    }else{
        return 1;
    }

    return HAL_TIM_Base_Start_IT(TIM_Handler);
}
uint8_t EV_TIM_Stop(TIM_TypeDef * Instance)
{
    TIM_HandleTypeDef *TIM_Handler;
    if(Instance == TIM1){
        TIM_Handler = &TIM1_Handler;  
    }else if(Instance == TIM2){
        TIM_Handler = &TIM2_Handler;
    }else if(Instance == TIM3){
        TIM_Handler = &TIM3_Handler;
    }else if(Instance == TIM4){
        TIM_Handler = &TIM4_Handler;
    }else if(Instance == TIM5){
        TIM_Handler = &TIM5_Handler;
    }else if(Instance == TIM6){
        TIM_Handler = &TIM6_Handler;
    }else if(Instance == TIM7){
        TIM_Handler = &TIM7_Handler;
    }else if(Instance == TIM8){
        TIM_Handler = &TIM8_Handler;
    }else if(Instance == TIM9){
        TIM_Handler = &TIM9_Handler;
    }else if(Instance == TIM10){
        TIM_Handler = &TIM10_Handler;
    }else if(Instance == TIM11){ 
        TIM_Handler = &TIM11_Handler;
    }else if(Instance == TIM12){
        TIM_Handler = &TIM12_Handler;
    }else if(Instance == TIM13){
        TIM_Handler = &TIM13_Handler;
    }else if(Instance == TIM14){ 
        TIM_Handler = &TIM14_Handler;
    }else{
        return 1;
    }

    return HAL_TIM_Base_Stop_IT(TIM_Handler);
}
uint8_t EV_PWM_Init(TIM_TypeDef * Instance, uint8_t Pin, uint8_t Channel, uint32_t Fre, uint32_t Dutymax)
{
    uint32_t prescaler, period;
    TIM_HandleTypeDef * TIM_Handler;
    TIM_OC_InitTypeDef *TIM_CHHandler;
    GPIO_InitTypeDef GPIO_Initure = PWM_GPIO_DEFAULT_CONFIG;
    GPIO_TypeDef * gpiox;
    uint32_t ch = Channel;

    uint32_t pin = find_gpio_pin(Pin);
    gpiox = find_gpio_group(Pin);


    TIM_CHHandler = &PUB_TIM_CHHandler;

    if(Instance == TIM1){
        __HAL_RCC_TIM1_CLK_ENABLE();
        TIM_Handler = &TIM1_Handler;
        TIM_Handler->Instance = TIM1;
        GPIO_Initure.Alternate = GPIO_AF1_TIM1;    
    }else if(Instance == TIM2){
        __HAL_RCC_TIM2_CLK_ENABLE();
        TIM_Handler = &TIM2_Handler;
        TIM_Handler->Instance = TIM2;
        GPIO_Initure.Alternate = GPIO_AF1_TIM2;         
    }else if(Instance == TIM3){
        __HAL_RCC_TIM3_CLK_ENABLE();
        TIM_Handler = &TIM3_Handler;
        TIM_Handler->Instance = TIM3;
        GPIO_Initure.Alternate = GPIO_AF2_TIM3;       
    }else if(Instance == TIM4){
        __HAL_RCC_TIM4_CLK_ENABLE();
        TIM_Handler = &TIM4_Handler;
        TIM_Handler->Instance = TIM4;
        GPIO_Initure.Alternate = GPIO_AF2_TIM4;      
    }else if(Instance == TIM5){
        __HAL_RCC_TIM5_CLK_ENABLE();
        TIM_Handler = &TIM5_Handler;
        TIM_Handler->Instance = TIM5;
        GPIO_Initure.Alternate = GPIO_AF2_TIM5;                
    }else if(Instance == TIM8){
        __HAL_RCC_TIM8_CLK_ENABLE();
        TIM_Handler = &TIM8_Handler;
        TIM_Handler->Instance = TIM8;
        GPIO_Initure.Alternate = GPIO_AF3_TIM8;        
    }else if(Instance == TIM9){
        __HAL_RCC_TIM9_CLK_ENABLE();
        TIM_Handler = &TIM9_Handler;
        TIM_Handler->Instance = TIM9;
        GPIO_Initure.Alternate = GPIO_AF3_TIM9;        
    }else if(Instance == TIM10){
        __HAL_RCC_TIM10_CLK_ENABLE();
        TIM_Handler = &TIM10_Handler;
        TIM_Handler->Instance = TIM10;
        GPIO_Initure.Alternate = GPIO_AF3_TIM10;        
    }else if(Instance == TIM11){ 
        __HAL_RCC_TIM11_CLK_ENABLE();
        TIM_Handler = &TIM11_Handler;
        TIM_Handler->Instance = TIM11;
        GPIO_Initure.Alternate = GPIO_AF3_TIM11;        
    }else if(Instance == TIM12){
        __HAL_RCC_TIM12_CLK_ENABLE();
        TIM_Handler = &TIM12_Handler;
        TIM_Handler->Instance = TIM12;
        GPIO_Initure.Alternate = GPIO_AF9_TIM12;      
    }else if(Instance == TIM13){
        __HAL_RCC_TIM13_CLK_ENABLE();
        TIM_Handler = &TIM13_Handler;
        TIM_Handler->Instance = TIM13;
        GPIO_Initure.Alternate = GPIO_AF9_TIM13;          
    }else if(Instance == TIM14){ 
        __HAL_RCC_TIM14_CLK_ENABLE();
        TIM_Handler = &TIM14_Handler;
        TIM_Handler->Instance = TIM14;
        GPIO_Initure.Alternate = GPIO_AF9_TIM14;
        
    }else{
        return 1;
    }

    if((Fre > 0xFFFF) || (Dutymax > 0xFFFF) || (Dutymax * Fre > 96000000))
        return 2;


    GPIO_Initure.Pin=pin;
    HAL_GPIO_Init(gpiox,&GPIO_Initure);

    period = Dutymax - 1;    
    
    if((Instance == TIM1)    \
    ||(Instance == TIM8) \
    ||(Instance == TIM9) \
    ||(Instance == TIM10) \
    ||(Instance == TIM11))
    {
        prescaler = (192000000 / period / Fre) - 1;
    }
    else
    {
        prescaler = (96000000 / period / Fre) - 1;
    }
    TIM_Handler->Init.Period = period;
    TIM_Handler->Init.Prescaler = prescaler;

    HAL_TIM_PWM_Init(TIM_Handler);
    
    HAL_TIM_PWM_ConfigChannel(TIM_Handler,TIM_CHHandler,ch);
	
    HAL_TIM_PWM_Start(TIM_Handler,ch);

    return 0;
}
uint8_t EV_PWM_Set(TIM_TypeDef * Instance, uint8_t Channel, uint16_t Duty)
{
    switch(Channel)
    {
        case TIM_CHANNEL_1:
            Instance->CCR1=Duty;
        break;
        case TIM_CHANNEL_2:
            Instance->CCR2=Duty;
        break;
        case TIM_CHANNEL_3:
            Instance->CCR3=Duty;
        break;
        case TIM_CHANNEL_4:
            Instance->CCR4=Duty;
        break;
        default:
            return 1;
        break;
    }
    return 0;
}
