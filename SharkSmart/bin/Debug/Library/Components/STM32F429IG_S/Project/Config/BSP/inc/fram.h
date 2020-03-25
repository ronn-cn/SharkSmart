#ifndef _FRAM_H
#define _FRAM_H
#include "sys.h"
					  

uint8_t FRAM_Check(void);  //检查器件
uint8_t FRAM_Init(void); //初始化IIC

void User_ParameterSave(uint16_t addr, uint8_t *data, uint8_t data_len);
void User_ParameterRead(uint16_t addr, uint8_t *data, uint8_t data_len);

#endif
