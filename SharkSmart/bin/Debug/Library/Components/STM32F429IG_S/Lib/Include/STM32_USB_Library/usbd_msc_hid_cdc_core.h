/* Includes ------------------------------------------------------------------*/

#ifndef __USB_MSC_HID_CDC_CORE_H_
#define __USB_MSC_HID_CDC_CORE_H_

#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_HID
  * @brief This file is the Header file for USBD_msc.c
  * @{
  */ 





/** @defgroup USBD_HID_Exported_Defines
  * @{
  */ 



#define USB_MSC_HID_CDC_CONFIG_DESC_SIZ  (USB_CUSTOM_HID_CONFIG_DESC_SIZ - \
										(9 * 2)+ \
										USB_MSC_CONFIG_DESC_SIZ + \
										USB_CDC_CONFIG_DESC_SIZ + \
										USBD_IAD_DESC_SIZE * 3)
/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */


/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_Class_cb_TypeDef  USBD_MSC_HID_CDC_cb;
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */ 

/**
  * @}
  */ 

#endif  /* __USB_HID_CORE_H_ */
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
