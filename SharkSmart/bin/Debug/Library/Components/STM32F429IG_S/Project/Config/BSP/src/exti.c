#include "exti.h"

void system_gpio_pin_callback_null(void)
{
	return;
}
void (*system_gpio_pin0_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin1_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin2_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin3_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin4_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin5_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin6_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin7_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin8_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin9_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin10_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin11_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin12_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin13_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin14_callback)(void) = system_gpio_pin_callback_null;
void (*system_gpio_pin15_callback)(void) = system_gpio_pin_callback_null;

void EXTI1_IRQHandler(void){HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);}
void EXTI2_IRQHandler(void){HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);}
void EXTI3_IRQHandler(void){HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);}
void EXTI4_IRQHandler(void){HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);}
void EXTI9_5_IRQHandler(void){HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);}
void EXTI15_10_IRQHandler(void){HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case GPIO_PIN_0: system_gpio_pin0_callback();	break;
		case GPIO_PIN_1: system_gpio_pin1_callback();	break;
		case GPIO_PIN_2: system_gpio_pin2_callback();	break;
		case GPIO_PIN_3: system_gpio_pin3_callback();	break;
		case GPIO_PIN_4: system_gpio_pin4_callback();	break;
		case GPIO_PIN_5: system_gpio_pin5_callback();	break;
		case GPIO_PIN_6: system_gpio_pin6_callback();	break;
		case GPIO_PIN_7: system_gpio_pin7_callback();	break;
		case GPIO_PIN_8: system_gpio_pin8_callback();	break;
		case GPIO_PIN_9: system_gpio_pin9_callback();	break;
		case GPIO_PIN_10: system_gpio_pin10_callback();	break;
		case GPIO_PIN_11: system_gpio_pin11_callback();	break;
		case GPIO_PIN_12: system_gpio_pin12_callback();	break;
		case GPIO_PIN_13: system_gpio_pin13_callback();	break;
		case GPIO_PIN_14: system_gpio_pin14_callback();	break;
		case GPIO_PIN_15: system_gpio_pin15_callback();	break;
		default:
			break;
	}
	
}
 

uint8_t EV_EXTI_Init(uint8_t Pin, uint8_t Mode,  void (*Callback)(void))
{
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_TypeDef * exit_gpiox;

    uint32_t exit_pin = find_gpio_pin(Pin);
    exit_gpiox = find_gpio_group(Pin);

	GPIO_Initure.Pin = exit_pin;
	GPIO_Initure.Mode=Mode; 
	if(Mode == GPIO_MODE_IT_FALLING){
		GPIO_Initure.Pull=GPIO_PULLUP;
	}else{
		GPIO_Initure.Pull=GPIO_PULLDOWN;
	}

	if(Callback == NULL) return 1;
	switch(exit_pin)
	{
		case GPIO_PIN_0: system_gpio_pin0_callback = Callback;	break;
		case GPIO_PIN_1: system_gpio_pin1_callback = Callback;	break;
		case GPIO_PIN_2: system_gpio_pin2_callback = Callback;	break;
		case GPIO_PIN_3: system_gpio_pin3_callback = Callback;	break;
		case GPIO_PIN_4: system_gpio_pin4_callback = Callback;	break;
		case GPIO_PIN_5: system_gpio_pin5_callback = Callback;	break;
		case GPIO_PIN_6: system_gpio_pin6_callback = Callback;	break;
		case GPIO_PIN_7: system_gpio_pin7_callback = Callback;	break;
		case GPIO_PIN_8: system_gpio_pin8_callback = Callback;	break;
		case GPIO_PIN_9: system_gpio_pin9_callback = Callback;	break;
		case GPIO_PIN_10: system_gpio_pin10_callback = Callback;	break;
		case GPIO_PIN_11: system_gpio_pin11_callback = Callback;	break;
		case GPIO_PIN_12: system_gpio_pin12_callback = Callback;	break;
		case GPIO_PIN_13: system_gpio_pin13_callback = Callback;	break;
		case GPIO_PIN_14: system_gpio_pin14_callback = Callback;	break;
		case GPIO_PIN_15: system_gpio_pin15_callback = Callback;	break;
		default:
			break;
	}


	HAL_GPIO_Init(exit_gpiox,&GPIO_Initure);
	
	switch (exit_pin)
	{
	case GPIO_PIN_0:
			HAL_NVIC_SetPriority(EXTI0_IRQn,3,3);       
			HAL_NVIC_EnableIRQ(EXTI0_IRQn);
		break;
	case GPIO_PIN_1:
			HAL_NVIC_SetPriority(EXTI1_IRQn,3,3);       
			HAL_NVIC_EnableIRQ(EXTI1_IRQn);
		break;
	case GPIO_PIN_2:
			HAL_NVIC_SetPriority(EXTI2_IRQn,3,3);       
			HAL_NVIC_EnableIRQ(EXTI2_IRQn);
		break;
	case GPIO_PIN_3:
			HAL_NVIC_SetPriority(EXTI3_IRQn,3,3);       
			HAL_NVIC_EnableIRQ(EXTI3_IRQn);
		break;
	case GPIO_PIN_4:
			HAL_NVIC_SetPriority(EXTI4_IRQn,3,3);       
			HAL_NVIC_EnableIRQ(EXTI4_IRQn);
		break;
	case GPIO_PIN_5:
	case GPIO_PIN_6:
	case GPIO_PIN_7:
	case GPIO_PIN_8:
	case GPIO_PIN_9:
			HAL_NVIC_SetPriority(EXTI9_5_IRQn,3,3);       
			HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	case GPIO_PIN_10:
	case GPIO_PIN_11:
	case GPIO_PIN_12:
	case GPIO_PIN_13:
	case GPIO_PIN_14:
	case GPIO_PIN_15:
			HAL_NVIC_SetPriority(EXTI15_10_IRQn,3,3);       
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;		
	default:
		break;
	}
	return 0;
}

