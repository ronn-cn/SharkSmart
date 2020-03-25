#include "usart.h"
#include "delay.h"
#include "log.h"

#define UART_GPIO_DEFAULT_CONFIG        \
{                                       \
    .Mode = GPIO_MODE_AF_PP,            \
    .Pull = GPIO_PULLUP,                \
    .Speed = GPIO_SPEED_FREQ_VERY_HIGH, \
}

#define UART_DEFAULT_CONFIG             \
{                                       \
	.Init.WordLength=UART_WORDLENGTH_8B,\
	.Init.StopBits=UART_STOPBITS_1,     \
	.Init.Parity=UART_PARITY_NONE,      \
	.Init.HwFlowCtl=UART_HWCONTROL_NONE,\
	.Init.Mode=UART_MODE_TX_RX,         \
}

UART_HandleTypeDef UART1_Handler = UART_DEFAULT_CONFIG;
UART_HandleTypeDef UART2_Handler = UART_DEFAULT_CONFIG;
UART_HandleTypeDef UART3_Handler = UART_DEFAULT_CONFIG;
UART_HandleTypeDef UART4_Handler = UART_DEFAULT_CONFIG;
UART_HandleTypeDef UART5_Handler = UART_DEFAULT_CONFIG;
UART_HandleTypeDef UART6_Handler = UART_DEFAULT_CONFIG;
UART_HandleTypeDef UART7_Handler = UART_DEFAULT_CONFIG;
UART_HandleTypeDef UART8_Handler = UART_DEFAULT_CONFIG;


uint8_t system_uart_recv_callback_null(uint8_t *Data, uint8_t Len)
{
    return 0;
}

uint8_t (*system_uart1_recv_callback)(uint8_t *Data, uint8_t Len) = system_uart_recv_callback_null;
uint8_t (*system_uart2_recv_callback)(uint8_t *Data, uint8_t Len) = system_uart_recv_callback_null;
uint8_t (*system_uart3_recv_callback)(uint8_t *Data, uint8_t Len) = system_uart_recv_callback_null;
uint8_t (*system_uart4_recv_callback)(uint8_t *Data, uint8_t Len) = system_uart_recv_callback_null;
uint8_t (*system_uart5_recv_callback)(uint8_t *Data, uint8_t Len) = system_uart_recv_callback_null;
uint8_t (*system_uart6_recv_callback)(uint8_t *Data, uint8_t Len) = system_uart_recv_callback_null;
uint8_t (*system_uart7_recv_callback)(uint8_t *Data, uint8_t Len) = system_uart_recv_callback_null;
uint8_t (*system_uart8_recv_callback)(uint8_t *Data, uint8_t Len) = system_uart_recv_callback_null;


#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    EV_UART_Send(USART1, (uint8_t *)&ch, 1);
    return ch;
}

void UART1_Init(void)
{

    EV_UART_Init(USART1, PA9, PA10, 115200, NULL);
}

//串口1中断服务程序
void USART1_IRQHandler(void) {
	uint8_t res;	      
	if(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET){	 
		res=USART1->DR; 
        system_uart1_recv_callback(&res, 1);			 
	}  				 											 
}   
 
void USART2_IRQHandler(void) {
	uint8_t res;	      
	if(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET){	 
		res=USART2->DR; 
        system_uart2_recv_callback(&res, 1);			 
	} 
}
void USART3_IRQHandler(void) {
	uint8_t res;	      
	if(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET){	 
		res=USART3->DR; 
        system_uart3_recv_callback(&res, 1);			 
	} 
}
void UART4_IRQHandler(void) {
	uint8_t res;	      
	if(__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_RXNE)!=RESET){	 
		res=UART4->DR; 
        system_uart4_recv_callback(&res, 1);			 
	} 
}
void UART5_IRQHandler(void) {
	uint8_t res;	      
	if(__HAL_UART_GET_FLAG(&UART5_Handler,UART_FLAG_RXNE)!=RESET){	 
		res=UART5->DR; 
        system_uart5_recv_callback(&res, 1);			 
	} 
}
void USART6_IRQHandler(void) {
	uint8_t res;	      
	if(__HAL_UART_GET_FLAG(&UART6_Handler,UART_FLAG_RXNE)!=RESET){	 
		res=USART6->DR; 
        system_uart6_recv_callback(&res, 1);			 
	} 
}
void UART7_IRQHandler(void) {
	uint8_t res;	      
	if(__HAL_UART_GET_FLAG(&UART7_Handler,UART_FLAG_RXNE)!=RESET){	 
		res=UART7->DR; 
        system_uart7_recv_callback(&res, 1);			 
	}  	
}
void UART8_IRQHandler(void) {
	uint8_t res;	      
	if(__HAL_UART_GET_FLAG(&UART8_Handler,UART_FLAG_RXNE)!=RESET){	 
		res=UART8->DR; 
        system_uart8_recv_callback(&res, 1);			 
	} 
}


uint8_t EV_UART_Init(USART_TypeDef *Instance, uint8_t TxPin, uint8_t RxPin, uint32_t BaudRate, uint8_t (*Recv_Callback)(uint8_t *Data, uint8_t Len))
{
    UART_HandleTypeDef * uart_handle;
    GPIO_InitTypeDef GPIO_Initure = UART_GPIO_DEFAULT_CONFIG;
    GPIO_TypeDef * tx_gpiox;
    GPIO_TypeDef * rx_gpiox;

    uint8_t alternate = 0;

    uint32_t tx_pin = find_gpio_pin(TxPin);
    uint32_t rx_pin = find_gpio_pin(RxPin);
    tx_gpiox = find_gpio_group(TxPin);
    rx_gpiox = find_gpio_group(RxPin);

    if(Instance == USART1){
        __HAL_RCC_USART1_CLK_ENABLE();
        HAL_NVIC_SetPriority(USART1_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        uart_handle = &UART1_Handler;
        alternate = GPIO_AF7_USART1;
        if(Recv_Callback != NULL )
            system_uart1_recv_callback = Recv_Callback;
    }
    else if(Instance == USART2){
        __HAL_RCC_USART2_CLK_ENABLE();
        HAL_NVIC_SetPriority(USART2_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(USART2_IRQn);        
        uart_handle = &UART2_Handler;
        alternate = GPIO_AF7_USART2;
        if(Recv_Callback != NULL )
            system_uart2_recv_callback = Recv_Callback;     
    }
    else if(Instance == USART3){
        __HAL_RCC_USART3_CLK_ENABLE();
        HAL_NVIC_SetPriority(USART3_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(USART3_IRQn);            
        uart_handle = &UART3_Handler;
        alternate = GPIO_AF7_USART3;
        if(Recv_Callback != NULL )
            system_uart3_recv_callback = Recv_Callback;      
    }
    else if(Instance == UART4){
        __HAL_RCC_UART4_CLK_ENABLE();
        HAL_NVIC_SetPriority(UART4_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(UART4_IRQn);            
        uart_handle = &UART4_Handler;
        alternate = GPIO_AF8_UART4;
        if(Recv_Callback != NULL )
            system_uart4_recv_callback = Recv_Callback;       
    }
    else if(Instance == UART5){
        __HAL_RCC_UART5_CLK_ENABLE();
        HAL_NVIC_SetPriority(UART5_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(UART5_IRQn);  
        uart_handle = &UART5_Handler;
        alternate = GPIO_AF8_UART5;
        if(Recv_Callback != NULL )
            system_uart5_recv_callback = Recv_Callback;       
    }
    else if(Instance == USART6){
        __HAL_RCC_USART6_CLK_ENABLE();
        HAL_NVIC_SetPriority(USART6_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(USART6_IRQn);          
        uart_handle = &UART6_Handler;
        alternate = GPIO_AF8_USART6;
        if(Recv_Callback != NULL )
            system_uart6_recv_callback = Recv_Callback;      
    }
    else if(Instance == UART7){
        __HAL_RCC_UART7_CLK_ENABLE();
        HAL_NVIC_SetPriority(UART7_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(UART7_IRQn);          
        uart_handle = &UART7_Handler;
        alternate = GPIO_AF8_UART7;
        if(Recv_Callback != NULL )
            system_uart7_recv_callback = Recv_Callback;     
    }
    else if(Instance == UART8){
        __HAL_RCC_UART8_CLK_ENABLE();
        HAL_NVIC_SetPriority(UART8_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(UART8_IRQn);          
        uart_handle = &UART8_Handler;
        alternate = GPIO_AF8_UART8;
        if(Recv_Callback != NULL )
            system_uart8_recv_callback = Recv_Callback;      
    }
    else
    {
        return 1;
    }

    GPIO_Initure.Pin=tx_pin;			
    GPIO_Initure.Alternate=alternate;
    HAL_GPIO_Init(tx_gpiox,&GPIO_Initure);

    GPIO_Initure.Pin=rx_pin;			
    GPIO_Initure.Alternate=alternate;
    HAL_GPIO_Init(rx_gpiox,&GPIO_Initure);    

	//UART 初始化设置
	uart_handle->Instance=Instance;
	uart_handle->Init.BaudRate=BaudRate;

	HAL_UART_Init(uart_handle);

    __HAL_UART_DISABLE_IT(uart_handle,UART_IT_TC);
    __HAL_UART_ENABLE_IT(uart_handle, UART_IT_RXNE);

    return 0;
}


uint8_t EV_UART_Send(USART_TypeDef *Instance, uint8_t *Data, uint16_t Len)
{
    UART_HandleTypeDef * uart_handle = &UART1_Handler;
    if(Instance == USART1){
        uart_handle = &UART1_Handler;
    }else if(Instance == USART2){
        uart_handle = &UART2_Handler;
    }else if(Instance == USART3){
        uart_handle = &UART3_Handler;
    }else if(Instance == UART4){
        uart_handle = &UART4_Handler;
    }else if(Instance == UART5){
        uart_handle = &UART5_Handler;
    }else if(Instance == USART6){
        uart_handle = &UART6_Handler;
    }else if(Instance == UART7){
        uart_handle = &UART7_Handler;
    }else if(Instance == UART8){
        uart_handle = &UART8_Handler;
    }else{
        return 1;
    }

    return HAL_UART_Transmit(uart_handle , Data, Len, 1000);
}



