#include "sys.h"


#define UART_LOG_ENABLE     1
#define LCD_LOG_ENABLE      1

#define LEVEL_VERBOSE       1
#define LEVEL_INFO          2
#define LEVEL_DEBUG         3
#define LEVEL_WARNING       4
#define LEVEL_ERROR         5
#define LEVEL_NONE          6

#define __LEVEL__   LEVEL_INFO

#define LCD_CACHE_DEPTH     25




void UART_LOG_Init(void);


void _Error_Handler(char *file, int line);
void uart_log(uint8_t level, const char * format, ...);
void LCD_LOG_Init( void);
void lcd_printf(uint8_t level, const char * format, ...);


#if defined(UART_LOG_ENABLE)
#define LOG(level, format, ...)         uart_log(level, format, ##__VA_ARGS__);
#else
#define LOG(level, format, ...)
#endif

#if defined(LCD_LOG_ENABLE)
#define LCD_LOG(level, format, ...)     {lcd_printf(level, format,##__VA_ARGS__);}
#else
#define LCD_LOG(level, format, ...)
#endif


