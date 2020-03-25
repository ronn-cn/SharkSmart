#ifndef __USER_PORTS_H
#define __USER_PORTS_H

#include "sys.h"
#include "log.h"
#include "bsp.h"
#include "ftl.h"  
#include "adc.h"
#include "disk.h"
#include "touch.h"
#include "sdram.h"
#include "malloc.h"
#include "user_usb.h"
#include "lwipopts.h"
#include "exfuns.h"
#include "port.h"
#include "config.h"
#include "timer.h"
#include "usb_communication.h"
#include "l2_usb_communication.h"
#include "eth_user.h"
#include "gui.h"

#include "can.h"
#include "myiic.h"
#include "usbd_usr.h"
#include "usbd_cdc_vcp.h" 
#include "usart.h"
#include "delay.h"
#include "lwip/api.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/dns.h"
#include "lwip/inet.h"
#include "lwip_comm.h"
#include "spi.h"
#include "rtos.h"
#include "ff.h"
#include "fram.h"
#include "dac.h"
#include "gpio.h"
#include "exti.h"
#include "rng.h"
#include "rtc.h"
#include "math.h"
#include "arm_math.h"

#include "os_task_prio.h"

#ifndef LOCAL_MAC_ADDR
#define LOCAL_MAC_ADDR      "00-00-00-00-00-00"
#endif



#ifndef SUPPORT_USER_CODE
#define SUPPORT_USER_CODE   1
#endif

#ifndef USE_ETH             
#define USE_ETH             1
#endif


#endif
