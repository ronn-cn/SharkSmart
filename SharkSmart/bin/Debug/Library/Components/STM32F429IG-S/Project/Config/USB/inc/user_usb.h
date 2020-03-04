#ifndef __USER_USB_H__
#define __USER_USB_H__

typedef enum {
    HID   = 0,
    HID_CDC,
    HID_MSC,
    HID_MSC_CDC,
}_usb_mode;

void USB_ModeConfig(void);
#endif