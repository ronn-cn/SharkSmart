#ifndef __RTC_H
#define __RTC_H
#include "sys.h"

uint8_t EV_RTC_Init(RTC_TypeDef * Instance);
uint8_t EV_RTC_Set(RTC_TypeDef * Instance, uint8_t Sec, uint8_t Min, uint8_t Hour, uint8_t Weekday, uint8_t Date, uint8_t Month, uint16_t Year);
uint8_t EV_RTC_Get(RTC_TypeDef * Instance, uint8_t * Sec, uint8_t * Min, uint8_t * Hour, uint8_t * Weekday, uint8_t * Date, uint8_t * Month, uint16_t * Year);


#endif
