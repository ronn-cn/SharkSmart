#ifndef __YQLCD_H
#define __YQLCD_H		
#include "sys.h"	 
#include "stdlib.h" 

//LCD重要参数集
typedef struct  
{		 	 
	uint16_t width;			//LCD 宽度
	uint16_t height;		//LCD 高度
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
#define WHITE         	 0xFFFF
#define BLACK         	 0x8000	  
#define BLUE         	 0x801F  
#define RED           	 0xFC00
#define GREEN         	 0x83E0
#define YELLOW        	 0xFFE0
#define TRANSPARENT      0x7FFF

#define EV_TA_LEFT			0x01
#define EV_TA_HCENTER		0x02
#define EV_TA_RIGHT			0x04
#define EV_TA_TOP			0x10
#define EV_TA_VCENTER		0x20
#define EV_TA_BOTTOM		0x40


void EV_LCD_Init(void);
void EV_LCD_DirSet(uint8_t dir);
void EV_LCD_BacklightSet(uint8_t percent);
void EV_LCD_Clear(uint16_t color, uint8_t layerx);

void EV_LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color, uint8_t layerx);
void EV_LCD_DrawCircle(uint16_t x,uint16_t y,uint8_t r, uint16_t color, uint8_t layerx);
void EV_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t layerx);
void EV_LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t layerx);
void EV_LCD_FillCircle(uint16_t x,uint16_t y,uint8_t r, uint16_t color, uint8_t layerx);
void EV_LCD_FillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t layerx);
void EV_LCD_FillColor(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color, uint8_t layerx);
uint16_t EV_LCD_ReadPoint(uint16_t x, uint16_t y, uint8_t layerx);

void EV_LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint16_t bkcolor, uint16_t ptcolor, uint8_t layerx);
void EV_LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode, char *p,uint16_t bkcolor, uint16_t ptcolor, uint8_t layerx);


uint8_t EV_LCD_ShowUnicodeString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t size, uint8_t cnt, uint8_t mode, uint16_t *p, char * path, uint16_t bkcolor, uint16_t ptcolor, uint8_t layerx);
uint8_t EV_LCD_FillFileData(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,char * path,uint8_t layerx);

#endif  
	 
	 



