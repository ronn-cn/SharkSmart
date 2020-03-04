#include "myiic.h"

#define I2C_DEFAULT_CONFIG                              \
{                                                       \
	.Init.ClockSpeed = 100000,                          \
	.Init.DutyCycle = I2C_DUTYCYCLE_2,                  \
	.Init.OwnAddress1 = 0,                              \
	.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,     \
	.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,    \
	.Init.OwnAddress2 = 0,                              \
	.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,    \
	.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE,        \
}

#define I2C_GPIO_DEFAULT_CONFIG				\
{											\
	.Mode = GPIO_MODE_AF_OD,				\
	.Pull = GPIO_PULLUP,					\
	.Speed = GPIO_SPEED_FREQ_VERY_HIGH,		\
}

I2C_HandleTypeDef I2C1_Handler = I2C_DEFAULT_CONFIG;
I2C_HandleTypeDef I2C2_Handler = I2C_DEFAULT_CONFIG;
I2C_HandleTypeDef I2C3_Handler = I2C_DEFAULT_CONFIG;

uint8_t EV_I2C_Init(I2C_TypeDef * Instance, uint8_t SclPin, uint8_t SdaPin)
{
    I2C_HandleTypeDef * i2c_handle;
    GPIO_InitTypeDef GPIO_Initure = I2C_GPIO_DEFAULT_CONFIG;
    GPIO_TypeDef * scl_gpiox;
    GPIO_TypeDef * sda_gpiox;

    uint8_t alternate = 0;

    uint32_t scl_pin = find_gpio_pin(SclPin);
    uint32_t sda_pin = find_gpio_pin(SdaPin);
    scl_gpiox = find_gpio_group(SclPin);
    sda_gpiox = find_gpio_group(SdaPin);

	if(Instance == I2C1){
		__HAL_RCC_I2C1_CLK_ENABLE();
		i2c_handle = &I2C1_Handler;
		alternate = GPIO_AF4_I2C1;	
	}else if(Instance == I2C2){
		__HAL_RCC_I2C2_CLK_ENABLE();
		i2c_handle = &I2C2_Handler;
		alternate = GPIO_AF4_I2C2;		
	}else if(Instance == I2C3){
		__HAL_RCC_I2C3_CLK_ENABLE();
		i2c_handle = &I2C3_Handler;
		alternate = GPIO_AF4_I2C3;		
	}else{
		return 1;
	}
	
    GPIO_Initure.Alternate = alternate;
    GPIO_Initure.Pin=scl_pin;
    HAL_GPIO_Init(scl_gpiox,&GPIO_Initure);
    GPIO_Initure.Pin=sda_pin;
    HAL_GPIO_Init(sda_gpiox,&GPIO_Initure);    

    i2c_handle->Instance = Instance;

    HAL_I2C_Init(i2c_handle);

	return 0;
}


uint8_t EV_I2C_Read(I2C_TypeDef * Instance, uint8_t I2CAddr, uint8_t *Data, uint16_t Len)
{
	I2C_HandleTypeDef * i2c_handle;
	
	if(Instance == I2C1){
		i2c_handle = &I2C1_Handler;
	}else if(Instance == I2C2){
		i2c_handle = &I2C2_Handler;
	}else if(Instance == I2C3){
		i2c_handle = &I2C3_Handler;
	}else{
		return 1;
	}
	return HAL_I2C_Master_Transmit(i2c_handle, I2CAddr, Data, Len, 1000);
}
uint8_t EV_I2C_Write(I2C_TypeDef * Instance, uint8_t I2CAddr, uint8_t *Data, uint16_t Len)
{
	I2C_HandleTypeDef * i2c_handle;

	if(Instance == I2C1){
		i2c_handle = &I2C1_Handler;
	}else if(Instance == I2C2){
		i2c_handle = &I2C2_Handler;
	}else if(Instance == I2C3){
		i2c_handle = &I2C3_Handler;
	}else{
		return 1;
	}
	return HAL_I2C_Master_Receive(i2c_handle, I2CAddr, Data, Len, 1000);
}
uint8_t EV_I2C_ReadReg(I2C_TypeDef * Instance, uint8_t I2CAddr, uint16_t RegAddr, uint32_t RegAddrSize, uint8_t *Data, uint16_t Len)
{
	I2C_HandleTypeDef * i2c_handle;

	if(Instance == I2C1){
		i2c_handle = &I2C1_Handler;
	}else if(Instance == I2C2){
		i2c_handle = &I2C2_Handler;
	}else if(Instance == I2C3){
		i2c_handle = &I2C3_Handler;
	}else{
		return 1;
	}
	

	return HAL_I2C_Mem_Read(i2c_handle, I2CAddr, RegAddr, RegAddrSize, (unsigned char *)Data, Len, 1000);
}
uint8_t EV_I2C_WriteReg(I2C_TypeDef * Instance, uint8_t I2CAddr, uint16_t RegAddr, uint32_t RegAddrSize, uint8_t *Data, uint16_t Len)
{
	I2C_HandleTypeDef * i2c_handle;

	if(Instance == I2C1){
		i2c_handle = &I2C1_Handler;
	}else if(Instance == I2C2){
		i2c_handle = &I2C2_Handler;
	}else if(Instance == I2C3){
		i2c_handle = &I2C3_Handler;
	}else{
		return 1;
	}
	

	return HAL_I2C_Mem_Write(i2c_handle, I2CAddr, RegAddr, RegAddrSize, (unsigned char *)Data, Len, 1000);
}
