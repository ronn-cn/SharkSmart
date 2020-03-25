#ifndef __LAN8720_H
#define __LAN8720_H
#include "sys.h"
#include "stm32f4xx_hal_conf.h"
extern ETH_HandleTypeDef ETH_Handler;               //��̫�����
extern ETH_DMADescTypeDef *DMARxDscrTab;			//��̫��DMA�������������ݽṹ��ָ��
extern ETH_DMADescTypeDef *DMATxDscrTab;			//��̫��DMA�������������ݽṹ��ָ�� 
extern uint8_t *Rx_Buff; 							//��̫���ײ���������buffersָ�� 
extern uint8_t *Tx_Buff; 							//��̫���ײ���������buffersָ��
extern ETH_DMADescTypeDef  *DMATxDescToSet;			//DMA����������׷��ָ��
extern ETH_DMADescTypeDef  *DMARxDescToGet; 		//DMA����������׷��ָ�� 
 

uint8_t LAN8720_Init(void);
uint32_t LAN8720_ReadPHY(uint16_t reg);
void LAN8720_WritePHY(uint16_t reg,uint16_t value);
uint8_t LAN8720_Get_Speed(void);
uint8_t ETH_MACDMA_Config(void);
uint8_t ETH_Mem_Malloc(void);
void ETH_Mem_Free(void);
uint32_t  ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDesc);
#endif
