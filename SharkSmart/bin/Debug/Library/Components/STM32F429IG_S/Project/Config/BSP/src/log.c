#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "log.h"
#include "stdarg.h"
#include "lcd.h"

typedef struct
{
  char  line[100];
  uint32_t color;
}__LCD_LOG_line_t;

static uint8_t is_uart_log_ok = 0;
static uint8_t is_lcd_log_ok = 0;

void UART_LOG_Init(void) 
{
    UART1_Init();
    is_uart_log_ok = 1;
}



void uart_log(uint8_t level, const char * format, ...)
{
    if(is_uart_log_ok != 1)
        return;
    if( level >= __LEVEL__ ) {
        char str[512];
        va_list ap;
        va_start(ap, format);
        (void)vsnprintf(str, sizeof(str), format, ap);
        va_end(ap);
        EV_UART_Send(USART1, (uint8_t *)str, strlen((const char  *)str));
        //HAL_UART_Transmit(&UART1_Handler, (uint8_t *)str, strlen((const char  *)str), 0xFFFF);
    }
}  

void _Error_Handler(char *file, int line)
{
    LOG(LEVEL_ERROR, "file [%s] line[%d] error!\r\n", file, line);
    delay_ms(10);
    while(1)
    {
    }
}

__LCD_LOG_line_t LCD_CacheBuffer [LCD_CACHE_DEPTH];

uint32_t LCD_LineColor = WHITE;
uint16_t LCD_CacheBuffer_xptr = 0;

uint16_t LCD_CacheBuffer_yptr_bottom = 0;
void LCD_LOG_Init( void)
{
    EV_LCD_Clear(BLACK, 1);
    
    LCD_CacheBuffer_yptr_bottom = 0;
    LCD_CacheBuffer_xptr = 0;
    is_lcd_log_ok = 1;
}

void LCD_LOG_UpdateDisplay(void)
{
    uint8_t i = 0;
    
    if(is_lcd_log_ok != 1)
        return;
    for(i = 0; i <= LCD_CacheBuffer_yptr_bottom; i++)
    {
        EV_LCD_ShowString(0, i * 16, 200, 16, 16, 0,(uint8_t *)(LCD_CacheBuffer[i].line), BLACK, LCD_CacheBuffer[i].color, 1);
    }
}


void lcd_printf(uint8_t level, const char * format, ...)
{

    if( level >= __LEVEL__ ) 
    {    
        uint32_t color;
        char str[128];
        uint8_t i = 0;
        uint16_t idx;
        uint16_t ibdx = 0;
        va_list ap;
        va_start(ap, format);
        (void)vsnprintf(str, sizeof(str), format, ap);
        va_end(ap);

        switch(level)
        {
            case LEVEL_VERBOSE:
            case LEVEL_INFO:
                color = WHITE;
                break;
            case LEVEL_DEBUG:
                color = GREEN;
                break;
            case LEVEL_WARNING:
                color = YELLOW;
                break;
            case LEVEL_ERROR:
            case LEVEL_NONE:
                color = RED;
                break;
            default:
                color = WHITE;
                break;
        }

        LCD_CacheBuffer[LCD_CacheBuffer_yptr_bottom].color = color;
        while(str[i] != '\0')
        {
            if(( LCD_CacheBuffer_xptr < (lcddev.width) / 8 ) &&  ( str[i] != '\n'))
            {
                LCD_CacheBuffer[LCD_CacheBuffer_yptr_bottom].line[LCD_CacheBuffer_xptr++] = str[i];
            }
            else
            {
                for(idx = LCD_CacheBuffer_xptr ; idx < (lcddev.width) / 8; idx++)
                {
                    LCD_CacheBuffer[LCD_CacheBuffer_yptr_bottom].line[idx] = ' ';
                }   
                LCD_CacheBuffer_xptr = 0;
                LCD_CacheBuffer_yptr_bottom ++; 
                
                
                if(LCD_CacheBuffer_yptr_bottom > LCD_CACHE_DEPTH - 1)
                {
                    for(ibdx = 0; ibdx <= LCD_CACHE_DEPTH - 1; ibdx++)
                    {
                        memcpy(&LCD_CacheBuffer[ibdx], &LCD_CacheBuffer[ibdx + 1], sizeof(__LCD_LOG_line_t));
                    }
                    LCD_CacheBuffer_yptr_bottom = LCD_CACHE_DEPTH - 1;
                }

                LCD_CacheBuffer[LCD_CacheBuffer_yptr_bottom].color = color;
                if( str[i] != '\n')
                {
                    LCD_CacheBuffer[LCD_CacheBuffer_yptr_bottom].line[LCD_CacheBuffer_xptr++] = str[i];
                }

                

            }
                
            i++;
        }
         LCD_LOG_UpdateDisplay();
    }
}

