#include "usbd_usr.h"
#include "usb_dcd_int.h"
#include <stdio.h> 
#include "log.h" 
#include "malloc.h"
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
#include "usbd_msc_bot.h"
#include "usbd_msc_hid_core.h"
#include "usbd_msc_hid_cdc_core.h"
#include "usbd_customhid_core.h"
#include "delay.h"
#include "usbd_cdc_core.h"
#include "usbd_hid_cdc_wrapper.h"

#include "user_usb.h"
//表示USB连接状态
//0,没有连接;
//1,已经连接;
vu8 bDeviceState=0;		//默认没有连接  


USB_OTG_CORE_HANDLE  USB_OTG_dev;
_usb_t g_usb_parameter;

/*          HID       */
uint8_t global_usb_new_head = 0;		//
uint8_t global_l1_receive_state = 0;	//L1层接收标志 当数据接收成功后 值为 TRUE(1)  没有数据时为 FALSE(0)
uint32_t global_usb_recv_data_len = 0;	//L1层接收的数据长度
uint8_t *global_usb_recv_data;			//L1层接收的数据

void USBD_User_Init(uint8_t mode){
    USBD_User_disconnected(); 
    global_usb_recv_data = mymalloc(SRAMEX, 2*1024);
	if(global_usb_recv_data == NULL)
	{
		LOG(LEVEL_ERROR,"USB L1 bufer malloc fail!\n");
		return;
	}
    g_usb_parameter.mode = mode;
    switch(mode)
    {
        case HID:
        {
            USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CUSTOMHID_cb,&USR_cb);
            break;
        }
        case HID_CDC:
        {
            USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_HID_CDC_cb,&USR_cb);
            break;
        }
        case HID_MSC:
        {
            MSC_BOT_Data=mymalloc(SRAMEX,MSC_MEDIA_PACKET);
            USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_HID_cb,&USR_cb);
            break;
        }
        case HID_MSC_CDC:
        {
            MSC_BOT_Data=mymalloc(SRAMEX,MSC_MEDIA_PACKET);
            USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_HID_CDC_cb,&USR_cb);
            break;
        }
        default:
        {
            MSC_BOT_Data=mymalloc(SRAMEX,MSC_MEDIA_PACKET);
            USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_HID_CDC_cb,&USR_cb);
            break;
        }
    }

}


//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void){
  	USBD_OTG_ISR_Handler(&USB_OTG_dev);
}  
//指向DEVICE_PROP结构体
//USB Device 用户回调函数. 
USBD_Usr_cb_TypeDef USR_cb ={
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,    
};
//USB Device 用户自定义初始化函数
void USBD_USR_Init(void){
} 
//USB Device 复位
//speed:USB速度,0,高速;1,全速;其他,错误.
void USBD_USR_DeviceReset (uint8_t speed){
}
//USB Device 配置成功
void USBD_USR_DeviceConfigured (void){
    bDeviceState=1;
} 
//USB Device挂起
void USBD_USR_DeviceSuspended(void){
    bDeviceState=0;
} 
//USB Device恢复
void USBD_USR_DeviceResumed(void){
}
//USB Device连接成功
void USBD_USR_DeviceConnected (void){
	bDeviceState=1;
}
//USB Device未连接
void USBD_USR_DeviceDisconnected (void){
	bDeviceState=0;
} 

//强制USB复位断开
void USBD_User_disconnected(void)
{
	__HAL_RCC_USB_OTG_FS_FORCE_RESET();
	delay_ms(100); 
	__HAL_RCC_USB_OTG_FS_RELEASE_RESET();

    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           
	
    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP; 
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);	
    delay_ms(100);
}









