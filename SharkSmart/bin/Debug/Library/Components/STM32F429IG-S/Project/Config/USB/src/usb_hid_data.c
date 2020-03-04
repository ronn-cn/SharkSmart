#include "sys.h"
#include <string.h>
#include "log.h"
#include "usb_hid_data.h"
#include "usbd_usr.h"
#include "usbd_customhid_core.h"
#include "delay.h"
#include "usb_communication.h"

//接收中断
void USB_HID_Recv(uint8_t *recv_data, uint8_t recv_data_len)
{
    usb_recv_L0data(recv_data, recv_data_len);
}
//发送函数
void USB_HID_Send(uint8_t * send_data, uint8_t send_data_len)
{
    uint8_t bBuffer[64]   = {0};
    uint8_t*pSend         = send_data;
    uint8_t nCount        = 0;
    
    while(send_data_len > 0)
    {
        nCount = (send_data_len > 64) ? 64 : send_data_len;
        memset(bBuffer, 0, 64);
        memcpy(bBuffer, pSend, nCount);
        
        
        USBD_CUSTOM_HID_SendReport(&USB_OTG_dev, bBuffer, 64);
        send_data_len -= nCount;
        pSend += nCount;
        delay_ms(80);
    }
}