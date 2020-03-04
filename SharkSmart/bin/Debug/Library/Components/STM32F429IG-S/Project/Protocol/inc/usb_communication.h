#ifndef __USB_COMMUNICATION_H
#define __USB_COMMUNICATION_H
#include "sys.h"

void usb_send_L1data(uint8_t * L2_Content_Buffer, uint16_t L2_Content_Len);
uint8_t usb_recv_L1data(uint8_t * L2_Content_Buffer, uint16_t * L2_Content_Len);
uint8_t usb_recv_L0data(uint8_t *usb_hid_data, uint8_t len);
#endif




