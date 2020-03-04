#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "log.h"	 
#include "delay.h"	 
#include "ltdc.h"
#include "timer.h"
#include "touch.h"
#include "ff.h"
#include "rtos.h"
#include "malloc.h"
#include "string.h"
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
			 
static void LCD_BacklightInit(void)
{
    EV_PWM_Init(TIM14, PF9, TIM_CHANNEL_1, 200, 5000);
}

/******************************** Public **********************************************/

//dir:0,竖屏；1,横屏
void EV_LCD_DirSet(uint8_t dir)
{
    lcddev.dir=dir;         //横屏/竖屏
	
    LTDC_Display_Dir(dir);
    lcddev.width=lcdltdc.width;
	lcddev.height=lcdltdc.height; 

	tp_dev.touchtype &= 0xFE;//横屏还是竖屏 
	tp_dev.touchtype |= lcddev.dir;
	return;
}


void EV_LCD_BacklightSet(uint8_t percent)
{
	uint32_t compare;
	compare = percent * 40 + 1060;
	compare = (compare > 4999) ? 4999 : compare;
	EV_PWM_Set(TIM14, TIM_CHANNEL_1, compare);
}
//初始化lcd
//该初始化函数可以初始化各种型号的LCD(详见本.c文件最前面的描述)
void EV_LCD_Init(void)
{ 	
	LCD_BacklightInit();  
	LTDC_Init();			  
	EV_LCD_DirSet(1);		//1:横屏  0:竖屏

	EV_LCD_BacklightSet(50);
	EV_LCD_Clear(BLACK, 0);
} 




uint16_t EV_LCD_ReadPoint(uint16_t x,uint16_t y, uint8_t layerx)
{
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//超过了范围,直接返回

	LTDC_Select_Layer(layerx);	
	return LTDC_Read_Point(x,y);
}			 

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void EV_LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color, uint8_t layerx)
{
	LTDC_Select_Layer(layerx);
    LTDC_Draw_Point(x,y,color);
}	 
 

//清屏函数
//color:要清屏的填充色
void EV_LCD_Clear(uint16_t color, uint8_t layerx)
{
	LTDC_Select_Layer(layerx);
	LTDC_Clear(color);
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void EV_LCD_FillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t layerx)
{          
	LTDC_Select_Layer(layerx);
	LTDC_Fill(x1,y1,x2,y2,color);
	
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void EV_LCD_FillColor(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color, uint8_t layerx)
{  
	LTDC_Select_Layer(layerx);
	LTDC_Color_Fill(x1,y1,x2,y2,color);
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void EV_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t layerx)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		EV_LCD_DrawPoint(uRow,uCol, color, layerx);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void EV_LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t layerx)
{
	EV_LCD_DrawLine(x1,y1,x2,y1, color, layerx);
	EV_LCD_DrawLine(x1,y1,x1,y2, color, layerx);
	EV_LCD_DrawLine(x1,y2,x2,y2, color, layerx);
	EV_LCD_DrawLine(x2,y1,x2,y2, color, layerx);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void EV_LCD_DrawCircle(uint16_t x,uint16_t y,uint8_t r, uint16_t color, uint8_t layerx)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		EV_LCD_DrawPoint(x+a,y-b, color, layerx);             //5
 		EV_LCD_DrawPoint(x+b,y-a, color, layerx);             //0           
		EV_LCD_DrawPoint(x+b,y+a, color, layerx);             //4               
		EV_LCD_DrawPoint(x+a,y+b, color, layerx);             //6 
		EV_LCD_DrawPoint(x-a,y+b, color, layerx);             //1       
 		EV_LCD_DrawPoint(x-b,y+a, color, layerx);             
		EV_LCD_DrawPoint(x-a,y-b, color, layerx);             //2             
  		EV_LCD_DrawPoint(x-b,y-a, color, layerx);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 	

//画实心圆
//x0,y0:坐标 
//r:半径
//color:颜色
void EV_LCD_FillCircle(uint16_t x,uint16_t y,uint8_t r,uint16_t color, uint8_t layerx)
{											  
	uint32_t i;
	uint32_t imax = ((uint32_t)r*707)/1000+1;
	uint32_t sqmax = (uint32_t)r*(uint32_t)r+(uint32_t)r/2;
	uint32_t sx=r;
	EV_LCD_DrawLine(x-r,y,x+r,y,color,layerx);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+sx*sx)>sqmax)// draw lines from outside  
		{
 			if (sx>imax) 
			{
				EV_LCD_DrawLine(x-i+1,y+sx,x+i-1,y+sx,color,layerx);
				EV_LCD_DrawLine(x-i+1,y-sx,x+i-1,y-sx,color,layerx);
			}
			sx--;
		}
		// draw lines from inside (center)  
		EV_LCD_DrawLine(x-sx,y+i,x+sx,y+i,color,layerx);
		EV_LCD_DrawLine(x-sx,y-i,x+sx,y-i,color,layerx);
	}
}  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void EV_LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint16_t bkcolor, uint16_t ptcolor, uint8_t layerx)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
        if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)EV_LCD_DrawPoint(x,y,ptcolor, layerx);
			else EV_LCD_DrawPoint(x,y,bkcolor, layerx);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
  
void EV_LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode, char *p,uint16_t bkcolor, uint16_t ptcolor, uint8_t layerx)
{         
	uint16_t text_cnt = strlen(p);
	uint16_t x1 = x;
	uint16_t x2 = x + width;
	uint16_t y1 = y;
	uint16_t y2 = y + height;

	uint8_t width_char_cnt_max = (width * 2) / size;
	uint8_t height_char_cnt_max = (height) / size;

	if(text_cnt > (width_char_cnt_max * height_char_cnt_max))
	{
		text_cnt = width_char_cnt_max * height_char_cnt_max;
	}

	EV_LCD_FillRectangle(x1, y1, x2, y2, bkcolor, layerx);



	for(uint16_t i = 0; i < text_cnt; i++)
	{		
		if(mode & EV_TA_RIGHT)
		{
			x = x1 + ((i % width_char_cnt_max) * size / 2);
			if(text_cnt <= width_char_cnt_max)
			{
				x += (width - (text_cnt * size / 2));
			}
			else
			{
				if((i / width_char_cnt_max) != (text_cnt / width_char_cnt_max))
				{
					x += (width - (width_char_cnt_max * size / 2));
				}
				else
				{
					x += (width - (text_cnt % width_char_cnt_max) * size / 2);
				}
			}
		}
		else if(mode & EV_TA_HCENTER)
		{
			x = x1 + ((i % width_char_cnt_max) * size / 2);
			if(text_cnt <= width_char_cnt_max)
			{
				x += ((width - (text_cnt * size / 2)) / 2);
			}
			else
			{
				if((i / width_char_cnt_max) != (text_cnt / width_char_cnt_max))
				{
					x += ((width - (width_char_cnt_max * size / 2)) / 2);
				}
				else
				{
					x += ((width - (text_cnt % width_char_cnt_max) * size / 2) / 2);
				}
			}
		}
		else
		{
			x = x1 + ((i % width_char_cnt_max) * size / 2);
		}

		if(mode & EV_TA_BOTTOM)
		{
			if(text_cnt <= width_char_cnt_max)
			{
				y = y1 + height - size;
			}
			else
			{
				y = y2 - (text_cnt / width_char_cnt_max - (i / width_char_cnt_max) + 1) * size;
			}
		}
		else if(mode & EV_TA_VCENTER)
		{
			if(text_cnt <= width_char_cnt_max)
			{
				y = y1 + (height - size) / 2;
			}
			else
			{
				y = y1 + (height - (text_cnt / width_char_cnt_max + 1) * size) / 2;
				y += ((i / width_char_cnt_max) * size);
			}
		}
		else
		{
			y = y1 + ((i / width_char_cnt_max) * size);
		}

        EV_LCD_ShowChar(x,y,p[i],size,bkcolor, ptcolor, layerx);
	}
}
	
uint8_t EV_LCD_ShowUnicodeString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t size, uint8_t cnt, uint8_t mode, uint16_t *p, char * path, uint16_t bkcolor, uint16_t ptcolor, uint8_t layerx)
{
	static FIL file_fd;
	uint16_t x1 = x;
	uint16_t x2 = x + width;
	uint16_t y1 = y;
	uint16_t y2 = y + height;

	uint8_t width_char_cnt_max = (width * 2) / size;
	uint8_t height_char_cnt_max = (height) / size;

    uint8_t line_max_cnt = 0;
    uint16_t line_text_point_cnt[100] = {0};
    uint8_t line_text_start[100] = {0};
    uint8_t line_text_end[100] = {0};
    uint8_t text_point_cnt_temp = 0;

    line_text_start[line_max_cnt] = 0;

	if(cnt > (width_char_cnt_max * height_char_cnt_max))
	{
		cnt = width_char_cnt_max * height_char_cnt_max;
	}

	EV_LCD_FillRectangle(x1, y1, x2, y2, bkcolor, layerx);

	uint8_t res;
	res = f_open(&file_fd, path, FA_READ);
	if(res != 0) return res;

	uint8_t font_point_max = size*size/8;
	uint8_t font[font_point_max];    


    for(uint8_t i = 0; i < cnt; i++)
    {
        if(p[i] < 0xFF)
        {
            text_point_cnt_temp = (size) / 2;
        }
        else
        {
            text_point_cnt_temp = size;
        }
        line_text_point_cnt[line_max_cnt] += text_point_cnt_temp;
        if(line_text_point_cnt[line_max_cnt] > width)
        {
            line_text_point_cnt[line_max_cnt] -= text_point_cnt_temp;
            line_text_end[line_max_cnt] = i - 1;
            line_max_cnt++;
            line_text_start[line_max_cnt] = i;
            line_text_point_cnt[line_max_cnt] += text_point_cnt_temp;
        }
    }
	line_text_end[line_max_cnt] = (cnt - 1);
    uint8_t text_i = 0;
    for(uint8_t row_i = 0; row_i <= line_max_cnt; row_i++)
    {
        for(uint8_t col_j = 0; col_j <= (line_text_end[row_i] - line_text_start[row_i]); col_j++)
        {
            if(col_j == 0)
            {
                if(mode & EV_TA_RIGHT)
                {
                    x = x1 + width - line_text_point_cnt[row_i];
                }
                else if(mode & EV_TA_HCENTER)
                {
                    x = x1 + (width - line_text_point_cnt[row_i]) / 2;
                }
                else
                {
                    x = x1;
                }

                if(mode & EV_TA_BOTTOM)
                {
                    y = y2 - (line_max_cnt - row_i + 1) * size;
                }
                else if(mode & EV_TA_VCENTER)
                {
                    y = y1 + (height - (line_max_cnt + 1) * size) / 2 + row_i * size;
                }
                else
                {
                    y = y1 + row_i * size;
                }
            }
			{
				uint32_t addr =  p[text_i] * (font_point_max);
				
				UINT br;
				uint32_t cpu_sr;
				OS_ENTER_CRITICAL();
				res = f_lseek(&file_fd, addr);
				res = f_read(&file_fd, font, font_point_max, &br);
				OS_EXIT_CRITICAL();
				uint8_t temp,t1;
				uint16_t xx, yy;
				xx = x;
				yy = y;
				
				for(uint8_t i = 0; i < font_point_max; i++)
				{
					temp = font[i];
					for(t1=0;t1<8;t1++)
					{			    
						if(temp&0x80)
							EV_LCD_DrawPoint(xx, yy, ptcolor, layerx);
						temp<<=1;
						xx++;
						if((xx-x)==size)
						{
							xx=x;
							yy++;
						}
					} 
				}
			}
            if(p[text_i] < 0xFF)
            {
                x += (size / 2);
            }
            else
            {
                x += size;
            }
            
            text_i++;
            
        }
    }



	f_close(&file_fd);
	return 0;
}

uint8_t EV_LCD_FillFileData(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,char * path,uint8_t layerx)
{
	static FIL file_fd;
	UINT br;
	uint8_t res;
	uint32_t file_size;
	uint16_t * data;
	uint32_t cpu_sr;	

	res = f_open(&file_fd, path, FA_READ);
	if(res != 0) return res;

	file_size = f_size(&file_fd);
	data = mymalloc(SRAMEX, file_size);
	OS_ENTER_CRITICAL();
	res = f_read(&file_fd, data, file_size, &br);
	OS_EXIT_CRITICAL();

	//LOG(LEVEL_INFO, "read %d %d %d\n", res, br, file_size);
	EV_LCD_FillColor(x1, y1, x2, y2, data, layerx);

	myfree(SRAMEX, data);

	f_close(&file_fd);
	return 0;
}






























