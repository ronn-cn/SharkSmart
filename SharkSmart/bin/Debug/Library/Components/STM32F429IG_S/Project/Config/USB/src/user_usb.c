#include "sys.h"
#include "user_usb.h"
#include "usbd_usr.h"

void USB_ModeConfig(void)
{
    USBD_User_Init(HID_MSC);
}