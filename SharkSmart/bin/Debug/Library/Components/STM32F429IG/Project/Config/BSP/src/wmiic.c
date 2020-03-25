#include "wmiic.h"
#include "sys.h"
#include "myiic.h"
#include "delay.h"
//IIC初始化
void WM_IIC_Init(void)
{
	EV_I2C_Init(I2C1, PB6, PB7);
	return;
}

uint8_t WM_IIC_Write(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len)
{
	return EV_I2C_WriteReg(I2C1, addr, reg, I2C_MEMADD_SIZE_8BIT, data, len);
}

uint8_t WM_IIC_Read(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len)
{
	return EV_I2C_ReadReg(I2C1, addr, reg, I2C_MEMADD_SIZE_8BIT, data, len);
}
