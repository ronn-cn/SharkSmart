#include "bsp.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ftl.h"  
#include "adc.h"
#include "gpio.h"
#include "disk.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include "sdram.h"
#include "malloc.h"
#include "myiic.h"
#include "spi.h"
#include "user_usb.h"
#include "can.h"
#include "fram.h"
#include "log.h"
#include "rtc.h"
#include "rng.h"
#include "lwip_comm.h" 
#include "timer.h"
#include "wm8978.h"
#include "m2_interface.h"

void BSP_Init(void)
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;

    uint8_t weekday;
    uint8_t date;
    uint8_t month;
    uint16_t year;
    uint8_t sdram_res = 0;
    uint8_t fram_res = 0;
    uint8_t TP_res = 0;
    uint8_t rtc_res = 0;
    uint16_t bsp_5v_mv = 0;
    HAL_Init();                     			//��ʼ��HAL�� 
    Stm32_Clock_Init(384,25,2,8);  				//����ʱ��,192Mhz 
    delay_init(192);                			//��ʼ����ʱ����
    
    EV_GPIO_Init(PD3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
    EV_GPIO_Init(PA15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
    EV_GPIO_Init(PD7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);

    EV_GPIO_Set(PD3, RESET);
    EV_GPIO_Set(PA15, RESET);
    EV_GPIO_Set(PD7, RESET);


    UART_LOG_Init();                            //log���ڳ�ʼ��
    sdram_res = Mem_Init();									//�ڴ��ʼ��
    if(sdram_res)
    {
        LCD_LOG(LEVEL_ERROR, "Sdram init error!\n");
        LOG(LEVEL_ERROR, "Sdram init error!\n");
        //_Error_Handler(__FILE__, __LINE__);     //��������ж�
    }
    else
    {
        LCD_LOG(LEVEL_DEBUG, "Sdram init error!\n");
        LOG(LEVEL_DEBUG, "Sdram init OK!\n");
    }
    //EV_LCD_Init();                   			//��ʼ��LCD
    Gui_Init();
    LCD_LOG_Init();                                 //��ʾ��ӡ��ʼ��

    BSP_ADC_Init();
    do{
        bsp_5v_mv = Get_5V_mV();
        if((bsp_5v_mv > 4700) && (bsp_5v_mv < 5300))
            break;
        LCD_LOG(LEVEL_ERROR, "Bsp 5V source %dmV!Low power!!!\n", bsp_5v_mv);
        LOG(LEVEL_ERROR, "Bsp 5V source %dmV!Low power!!!\n", bsp_5v_mv);
        delay_ms(2000);
    }while(1);
    LCD_LOG(LEVEL_DEBUG, "Bsp 5V source %dmV!\n", bsp_5v_mv);
    LOG(LEVEL_DEBUG, "Bsp 5V source %dmV!\n", bsp_5v_mv);
 

    rtc_res = EV_RTC_Init(RTC);                                 //
    if(rtc_res)
    {
        LCD_LOG(LEVEL_ERROR, "RTC init error!\n");
        LOG(LEVEL_ERROR, "RTC init error!\n");
    }
    else
    {
        LCD_LOG(LEVEL_DEBUG, "RTC init OK!\n");
        LOG(LEVEL_DEBUG, "RTC init OK!\n");
    }

    EV_RTC_Get(RTC, &sec, &min, &hour, &weekday, &date, &month, &year);
    LCD_LOG(LEVEL_DEBUG, "RTC Time 20%02d/%02d/%02d ", year, month, date);
    LCD_LOG(LEVEL_DEBUG, " %02d:%02d:%02d\n", hour, min, sec);
    LOG(LEVEL_DEBUG, "RTC Time 20%02d/%02d/%02d ", year, month, date);
    LOG(LEVEL_DEBUG, " %02d:%02d:%02d\n", hour, min, sec);

	fram_res = FRAM_Init();					                //FRAM��ʼ��
    if(fram_res)
    {
        LCD_LOG(LEVEL_ERROR, "fram init error!\n");
        LOG(LEVEL_ERROR, "fram init error!\n");
    }
    else
    {
        LCD_LOG(LEVEL_DEBUG, "fram init OK!\n");
        LOG(LEVEL_DEBUG, "fram init OK!\n");
    }

    LCD_LOG(LEVEL_DEBUG, "Try to init TP!\n");
    LOG(LEVEL_DEBUG, "Try to init TP!\n");
    TP_res=TP_Init();				    				//��������ʼ�� 
    if(TP_res)
    {
        LCD_LOG(LEVEL_ERROR, "TP init error!\n");
        LOG(LEVEL_ERROR, "TP init error!\n");
    }else{
        LCD_LOG(LEVEL_DEBUG, "TP init OK!\n");
        LOG(LEVEL_DEBUG, "TP init OK!\n");
    }

    while(WM8978_Init())
    {
        LOG(LEVEL_DEBUG, "Audio init error \n");
        LCD_LOG(LEVEL_DEBUG, "Audio init error!\n");
        delay_ms(200);
    }
    LCD_LOG(LEVEL_DEBUG, "Audio init OK!\n");
    LOG(LEVEL_DEBUG, "Audio init OK!\n");
	DISK_Init();								//NAND��ʼ�� 
    LOG(LEVEL_INFO,"NAND Disk OK!\n ");
    m2_task_creat();
}