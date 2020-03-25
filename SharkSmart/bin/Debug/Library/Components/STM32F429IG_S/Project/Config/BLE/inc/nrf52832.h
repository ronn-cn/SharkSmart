#include "sys.h"
#include "gpio.h"
#include "usart.h"

uint8_t EV_BLE_Init(void);
uint8_t EV_BLE_Send(uint8_t *Data, uint16_t Len);
uint8_t EV_BLE_Recv_Init(void (*Recv_Callback)(uint8_t *data, uint16_t len));


void ble_reset(void);
uint8_t ble_check_connected(void);
