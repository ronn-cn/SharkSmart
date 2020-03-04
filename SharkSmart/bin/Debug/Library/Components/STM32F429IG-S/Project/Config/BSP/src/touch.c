#include "touch.h" 
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "lcd.h"
#include "log.h"
#include "config.h"
_m_tp_dev tp_dev=
{
	TP_Init,
	0,
	0,
	{0,0,0,0,0},
	{0,0,0,0,0},
	0,
	0,	  	 		
	0,
	0,	  	 		
};					

//´¥ÃþÆÁ³õÊ¼»¯  		    
uint8_t TP_Init(void)
{	
	uint8_t res = 0;
	res = GT911_Init();
	tp_dev.scan=GT911_Scan;
	tp_dev.touchtype|=0X80;
	tp_dev.touchtype|=lcddev.dir;
	return res;
}
