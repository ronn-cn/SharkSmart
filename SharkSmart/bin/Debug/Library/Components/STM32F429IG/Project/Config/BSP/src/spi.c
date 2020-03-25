#include "spi.h"

#define SPI_DEFAULT_CONFIG                              \
{                                                       \
    .Init.Mode              =SPI_MODE_MASTER,           \
    .Init.Direction         =SPI_DIRECTION_2LINES,      \
    .Init.DataSize          =SPI_DATASIZE_8BIT,         \
    .Init.CLKPolarity       =SPI_POLARITY_HIGH,         \
    .Init.CLKPhase          =SPI_PHASE_2EDGE,           \
    .Init.NSS               =SPI_NSS_SOFT,              \
    .Init.BaudRatePrescaler =SPI_BAUDRATEPRESCALER_128, \
    .Init.FirstBit          =SPI_FIRSTBIT_MSB,          \
    .Init.TIMode            =SPI_TIMODE_DISABLE,        \
    .Init.CRCCalculation    =SPI_CRCCALCULATION_DISABLE,\
    .Init.CRCPolynomial     =7,                         \
}

#define SPI_GPIO_DEFAULT_CONFIG     \
{                                   \
        .Mode=GPIO_MODE_AF_PP,      \
        .Pull=GPIO_PULLUP,          \
        .Speed=GPIO_SPEED_FAST,     \
}
SPI_HandleTypeDef SPI1_Handler = SPI_DEFAULT_CONFIG;
SPI_HandleTypeDef SPI2_Handler = SPI_DEFAULT_CONFIG;
SPI_HandleTypeDef SPI3_Handler = SPI_DEFAULT_CONFIG;
SPI_HandleTypeDef SPI4_Handler = SPI_DEFAULT_CONFIG;
SPI_HandleTypeDef SPI5_Handler = SPI_DEFAULT_CONFIG;
SPI_HandleTypeDef SPI6_Handler = SPI_DEFAULT_CONFIG;
uint8_t SPI_Init(SPI_TypeDef * Instance, uint8_t ClkPin, uint8_t MosiPin, uint8_t MisoPin, uint32_t BaudRatePrescaler, uint32_t CLKPolarity, uint32_t CLKPhase)
{
    SPI_HandleTypeDef * spi_handle;
    GPIO_InitTypeDef GPIO_Initure = SPI_GPIO_DEFAULT_CONFIG;
    GPIO_TypeDef * clk_gpiox;
    GPIO_TypeDef * mosi_gpiox;
    GPIO_TypeDef * miso_gpiox;

    uint8_t alternate = 0;

    uint32_t clk_pin = find_gpio_pin(ClkPin);
    uint32_t mosi_pin = find_gpio_pin(MosiPin);
    uint32_t miso_pin = find_gpio_pin(MisoPin);
    clk_gpiox = find_gpio_group(ClkPin);
    mosi_gpiox = find_gpio_group(MosiPin);
    miso_gpiox = find_gpio_group(MisoPin);

    if(Instance == SPI1){
        __HAL_RCC_SPI1_CLK_ENABLE();
        spi_handle = &SPI1_Handler;
        alternate = GPIO_AF5_SPI1;
    }else if(Instance == SPI2){
        __HAL_RCC_SPI2_CLK_ENABLE();
        spi_handle = &SPI2_Handler;
        alternate = GPIO_AF5_SPI2;    
    }else if(Instance == SPI3){
        __HAL_RCC_SPI3_CLK_ENABLE();
        spi_handle = &SPI3_Handler;
        alternate = GPIO_AF5_SPI3;     
    }else if(Instance == SPI4){
        __HAL_RCC_SPI4_CLK_ENABLE();
        spi_handle = &SPI4_Handler;
        alternate = GPIO_AF5_SPI4;      
    }else if(Instance == SPI5){
        __HAL_RCC_SPI5_CLK_ENABLE();
        spi_handle = &SPI5_Handler;
        alternate = GPIO_AF5_SPI5;  
    }else if(Instance == SPI6){
        __HAL_RCC_SPI6_CLK_ENABLE();
        spi_handle = &SPI6_Handler;
        alternate = GPIO_AF5_SPI6;
    }else{
        return 1;
    }

    GPIO_Initure.Alternate = alternate;
    GPIO_Initure.Pin=clk_pin;
    HAL_GPIO_Init(clk_gpiox,&GPIO_Initure);
    GPIO_Initure.Pin=mosi_pin;
    HAL_GPIO_Init(mosi_gpiox,&GPIO_Initure);
    GPIO_Initure.Pin=miso_pin;
    HAL_GPIO_Init(miso_gpiox,&GPIO_Initure);        

    spi_handle->Instance = Instance;
    spi_handle->Init.CLKPolarity = CLKPolarity;
    spi_handle->Init.CLKPhase = CLKPhase;
    spi_handle->Init.BaudRatePrescaler = BaudRatePrescaler;

    HAL_SPI_Init(spi_handle);
    
    __HAL_SPI_ENABLE(spi_handle);

    return 0;
}

uint8_t EV_SPI_Send(SPI_TypeDef * Instance, uint8_t *Data, uint16_t Len)
{
    SPI_HandleTypeDef * spi_handle;

    if(Instance == SPI1){
        spi_handle = &SPI1_Handler;
    }else if(Instance == SPI2){
        spi_handle = &SPI2_Handler;
    }else if(Instance == SPI3){
        spi_handle = &SPI3_Handler;
    }else if(Instance == SPI4){
        spi_handle = &SPI4_Handler;
    }else if(Instance == SPI5){
        spi_handle = &SPI5_Handler;
    }else if(Instance == SPI6){
        spi_handle = &SPI6_Handler;
    }else{
        return 1;
    }
    return HAL_SPI_Transmit(spi_handle, Data, Len, 1000);
}
uint8_t EV_SPI_Recv(SPI_TypeDef * Instance, uint8_t *Data, uint16_t Len)
{
    SPI_HandleTypeDef * spi_handle;

    if(Instance == SPI1){
        spi_handle = &SPI1_Handler;
    }else if(Instance == SPI2){
        spi_handle = &SPI2_Handler;
    }else if(Instance == SPI3){
        spi_handle = &SPI3_Handler;
    }else if(Instance == SPI4){
        spi_handle = &SPI4_Handler;
    }else if(Instance == SPI5){
        spi_handle = &SPI5_Handler;
    }else if(Instance == SPI6){
        spi_handle = &SPI6_Handler;
    }else{
        return 1;
    }
    return HAL_SPI_Receive(spi_handle, Data, Len, 1000);
}
