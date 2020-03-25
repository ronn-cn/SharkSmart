#ifndef __USB_HID_DATA_H
#define __USB_HID_DATA_H

void USB_HID_Recv(uint8_t *recv_data, uint8_t recv_data_len);
void USB_HID_Send(uint8_t * send_data, uint8_t send_data_len);
#endif