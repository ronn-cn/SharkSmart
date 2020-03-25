#include "sys.h"
#include "nrf52832.h"
#include "usart.h"
#include "rtos.h"

void (*nrf52832_default_transferCallback)(uint8_t *data, uint16_t len);
static void ble_delay_ms(uint32_t nms)
{
    if (OS_Running == FALSE)
    {
        uint32_t tickstart = 0;
        tickstart = HAL_GetTick();
        while ((HAL_GetTick() - tickstart) < nms)
        {
        }
    }
    else
    {
        OSTimeDly(nms);
    }
}

uint8_t nrf_rx_calll_back(uint8_t *data, uint8_t len)
{
    if(nrf52832_default_transferCallback != NULL)
	{
		nrf52832_default_transferCallback(data, len);
	}
	return 0;
}

uint8_t EV_BLE_Send(uint8_t *Data, uint16_t Len)
{
	return EV_UART_Send(UART5, Data, Len);
}

uint8_t EV_BLE_Recv_Init(void (*Recv_Callback)(uint8_t *data, uint16_t len))
{
    if(Recv_Callback == NULL)
    {
        return 1;
    }else{
        nrf52832_default_transferCallback = Recv_Callback;
    }
    return 0;
}

#include "log.h"
uint8_t EV_BLE_Init(void)
{
	EV_UART_Init(UART5, PC12, PD2, 115200, nrf_rx_calll_back);
	EV_GPIO_Init(PH7, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);

	EV_GPIO_Set(PH7, 0);
	ble_delay_ms(100);
	EV_GPIO_Set(PH7, 1);
	ble_delay_ms(100);
    return 0;    
}