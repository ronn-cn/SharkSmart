/**
  ******************************************************************************
  * File Name          : I2S.h
  * Description        : This file provides code for the configuration
  *                      of the I2S instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __i2s_H
#define __i2s_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2S_HandleTypeDef hi2s3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2S3_Init(void);
void I2S3_Init(uint32_t I2S_Standard,uint32_t I2S_Mode,uint32_t I2S_Clock_Polarity,uint32_t I2S_DataFormat);
/* USER CODE BEGIN Prototypes */
uint8_t I2S3_SampleRate_Set(uint32_t samplerate);
void I2S3_TX_DMA_Init(uint8_t* buf0,uint8_t *buf1,uint16_t num); 
void I2S_Play_Start(void); 
void I2S_Play_Stop(void);  
extern void (*i2s_tx_callback)(void);		//IIS TX回调函数指针  
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ i2s_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
