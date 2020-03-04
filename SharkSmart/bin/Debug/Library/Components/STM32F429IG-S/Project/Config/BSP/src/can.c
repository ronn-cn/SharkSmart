#include "can.h"
#include "log.h"
#include "delay.h"
#include <string.h>
#include "timer.h"
#include "rtos.h"

#define CAN_DEFAULT_CONFIG      \
{                               \
    .Init.Mode=CAN_MODE_NORMAL, \
    .Init.SJW=CAN_SJW_1TQ,      \
    .Init.BS1=CAN_BS1_13TQ,     \
    .Init.BS2=CAN_BS2_2TQ,      \
    .Init.TTCM=DISABLE,         \
    .Init.ABOM=ENABLE,          \
    .Init.AWUM=ENABLE,          \
    .Init.NART=DISABLE,         \
    .Init.RFLM=ENABLE,          \
    .Init.TXFP=DISABLE,         \
}
#define CAN_FILER_DEFAULT_CONFIG        \
{                                       \
.FilterNumber=0,                        \
.FilterMode=CAN_FILTERMODE_IDMASK,      \
.FilterScale=CAN_FILTERSCALE_16BIT,     \
.FilterIdHigh=0x0000,                   \
.FilterIdLow=0x0000,                    \
.FilterMaskIdHigh=0x0000,               \
.FilterMaskIdLow=0x0000,                \
.FilterFIFOAssignment=CAN_FILTER_FIFO0, \
.FilterActivation=ENABLE,               \
.BankNumber=0,                          \
}

#define CAN_GPIO_DEFAULT_CONFIG \
{                               \
    .Mode=GPIO_MODE_AF_PP,      \
    .Pull=GPIO_PULLUP,          \
    .Speed=GPIO_SPEED_HIGH,     \
    .Alternate=GPIO_AF9_CAN1,   \
}

#define CAN_TX_MESSAGE_DEFAULT_CONFIG   \
{                                       \
    .ExtId=0x00,                        \
    .IDE=CAN_ID_STD,                    \
    .RTR=CAN_RTR_DATA,                  \
}

static  CAN_HandleTypeDef       CAN1_Handler    =   CAN_DEFAULT_CONFIG;
static  CAN_HandleTypeDef       CAN2_Handler    =   CAN_DEFAULT_CONFIG;
static  CanTxMsgTypeDef         CAN1_TxMessage  =   CAN_TX_MESSAGE_DEFAULT_CONFIG;
static  CanRxMsgTypeDef         CAN1_RxMessage;
static  CanTxMsgTypeDef         CAN2_TxMessage  =   CAN_TX_MESSAGE_DEFAULT_CONFIG;
static  CanRxMsgTypeDef         CAN2_RxMessage;
static  CAN_FilterConfTypeDef   CAN1_FilerConf  =   CAN_FILER_DEFAULT_CONFIG;
static  CAN_FilterConfTypeDef   CAN2_FilerConf  =   CAN_FILER_DEFAULT_CONFIG;

uint8_t system_can_recv_callback_null(uint8_t *data, uint8_t len){return 0;}
uint8_t (*system_can1_recv_callback)(uint8_t *data, uint8_t len) = system_can_recv_callback_null;
uint8_t (*system_can2_recv_callback)(uint8_t *data, uint8_t len) = system_can_recv_callback_null;

void CAN1_RX0_IRQHandler(void){HAL_CAN_IRQHandler(&CAN1_Handler);}
void CAN2_RX0_IRQHandler(void){HAL_CAN_IRQHandler(&CAN2_Handler);}
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
    if(hcan->Instance == CAN1)
    {
        system_can1_recv_callback(CAN1_Handler.pRxMsg->Data, CAN1_Handler.pRxMsg->DLC);
        __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);
    }
    else if(hcan->Instance == CAN2)
    {
        system_can2_recv_callback(CAN2_Handler.pRxMsg->Data, CAN2_Handler.pRxMsg->DLC);
        __HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP1);
    }
}

uint8_t EV_CAN_Init(CAN_TypeDef * Instance, uint8_t TxPin, uint8_t RxPin, uint32_t Id, uint32_t Speed, uint8_t (* Recv_Callback)(uint8_t *Data, uint8_t Len))
{
    CAN_HandleTypeDef * can_handle;
    CAN_FilterConfTypeDef  *can_filerconfig;
    GPIO_InitTypeDef GPIO_Initure = CAN_GPIO_DEFAULT_CONFIG;
    GPIO_TypeDef * tx_gpiox;
    GPIO_TypeDef * rx_gpiox;

    uint8_t alternate = 0;
    uint8_t can_recv_fifo;
    uint8_t can_recv_fifo_fmp;
    uint8_t can_iqr_num = 0;
    uint32_t can_id = 0;

    uint32_t tx_pin = find_gpio_pin(TxPin);
    uint32_t rx_pin = find_gpio_pin(RxPin);
    tx_gpiox = find_gpio_group(TxPin);
    rx_gpiox = find_gpio_group(RxPin); 

    can_id = Id;

    if(Instance == CAN1){
		__HAL_RCC_CAN1_CLK_ENABLE();
        can_iqr_num = CAN1_RX0_IRQn;
        can_recv_fifo = CAN_FIFO0;
        can_recv_fifo_fmp = CAN_IT_FMP0;
		can_handle = &CAN1_Handler;
        can_filerconfig = &CAN1_FilerConf;
        can_filerconfig->FilterNumber=0;
        can_filerconfig->BankNumber=0;
        can_filerconfig->FilterFIFOAssignment=CAN_FILTER_FIFO0;
        can_handle->pTxMsg=&CAN1_TxMessage;
        can_handle->pRxMsg=&CAN1_RxMessage;
		alternate = GPIO_AF9_CAN1;
        if(Recv_Callback != NULL )
           system_can1_recv_callback = Recv_Callback;
    }else if(Instance == CAN2){
		__HAL_RCC_CAN2_CLK_ENABLE();
        can_iqr_num = CAN2_RX0_IRQn;        //CAN1_RX0_IRQn
        can_recv_fifo = CAN_FIFO1;
        can_recv_fifo_fmp = CAN_IT_FMP1;
		can_handle = &CAN2_Handler;
        can_filerconfig = &CAN2_FilerConf;
        can_filerconfig->FilterNumber=14;
        can_filerconfig->BankNumber=14;
        can_filerconfig->FilterFIFOAssignment=CAN_FILTER_FIFO1;
        can_handle->pTxMsg=&CAN2_TxMessage;
        can_handle->pRxMsg=&CAN2_RxMessage;
		alternate = GPIO_AF9_CAN2;
        if(Recv_Callback != NULL )
            system_can2_recv_callback = Recv_Callback;
    }else{
        return 1;
    }        
    can_handle->Instance = Instance;
    can_handle->Init.Prescaler = 3000000/Speed;

    GPIO_Initure.Pin=tx_pin;			
    GPIO_Initure.Alternate=alternate;
    HAL_GPIO_Init(tx_gpiox,&GPIO_Initure);

    GPIO_Initure.Pin=rx_pin;			
    GPIO_Initure.Alternate=alternate;
    HAL_GPIO_Init(rx_gpiox,&GPIO_Initure);   

    if(HAL_CAN_Init(can_handle)!=HAL_OK) return 3;
    
    if (can_id==0xFFFF || can_id == 0)
    {
        //不过滤
        can_filerconfig->FilterMode=CAN_FILTERMODE_IDMASK; 
        can_filerconfig->FilterScale=CAN_FILTERSCALE_32BIT;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        can_filerconfig->FilterMaskIdHigh=0x0000;
        can_filerconfig->FilterMaskIdLow=0x0000;
    }
    else
    {
        can_filerconfig->FilterMode=CAN_FILTERMODE_IDMASK; 
        can_filerconfig->FilterScale=CAN_FILTERSCALE_32BIT;
        can_filerconfig->FilterMaskIdHigh=0xFFFF - (can_id << 5);
        can_filerconfig->FilterMaskIdLow=0xFFFF;
    }
    

    if(HAL_CAN_ConfigFilter(can_handle,can_filerconfig)!=HAL_OK) return 2;//滤波器初始化            


	HAL_CAN_Receive_IT(can_handle,can_recv_fifo);
    __HAL_CAN_ENABLE_IT(can_handle,can_recv_fifo_fmp);
    HAL_NVIC_SetPriority(can_iqr_num,2,2);
    HAL_NVIC_EnableIRQ(can_iqr_num); 

    return 0;
}
uint8_t EV_CAN_Send(CAN_TypeDef * Instance, uint32_t Id, uint8_t *Data, uint8_t Len)
{
    CanTxMsgTypeDef * TxMessage;

    if(Instance == CAN1)
    {
        TxMessage = &CAN1_TxMessage;
    }
    else
    {
        TxMessage = &CAN2_TxMessage;
    }
    TxMessage->StdId=Id;
    TxMessage->ExtId=0x00;                   // 设置扩展标示符 
    TxMessage->IDE=CAN_ID_STD;               // 标准帧
    TxMessage->RTR=CAN_RTR_DATA;             // 数据帧
    TxMessage->DLC=Len;                      // 要发送的数据长度
    memcpy(TxMessage->Data,Data,Len);

    if(Instance == CAN1)
    {
        if(HAL_CAN_Transmit(&CAN1_Handler,1000)!=HAL_OK)
        {
            __HAL_CAN_ENABLE_IT(&CAN1_Handler, CAN_IT_FMP0);
            return 1;
        }
        __HAL_CAN_ENABLE_IT(&CAN1_Handler, CAN_IT_FMP0);    
        return 0;  
    }
    else
    {
        if(HAL_CAN_Transmit(&CAN2_Handler,1000)!=HAL_OK)
        {
            __HAL_CAN_ENABLE_IT(&CAN2_Handler, CAN_IT_FMP1);
            return 1;
        }
        __HAL_CAN_ENABLE_IT(&CAN2_Handler, CAN_IT_FMP1);    
        return 0;  
    }
}



