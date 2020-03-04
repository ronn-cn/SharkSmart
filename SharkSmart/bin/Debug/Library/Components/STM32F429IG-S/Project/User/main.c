#include "GLOBAL.h"

uint32_t* TASK_START_STK;
void TaskStart(void* p_arg);

uint32_t* TASK_UsbData_STK;
void TaskUsbData(void* p_arg);

void Recovery();

//主函数
int main(void)
{
    //板级支持包初始化，是介于主板硬件和操作系统中驱动层程序之间的一层，
    //主要是实现对操作系统的支持，为上层的驱动程序提供访问硬件设备寄存器的函数包，使之能够更好的运行于硬件主板。
    //包括时钟初始化、内存初始化、LCD初始化、RTC初始化、ADC初始化、触摸屏初始化、NAND初始化等
    BSP_Init();

    //启动任务创建
    TASK_START_STK = mymalloc(SRAMEX, 256 * 4);
    OSTaskCreate(TaskStart, "Start", (void*)0, (uint32_t*)&TASK_START_STK[255], 0);
    //开启UCOS
    OSStart();
}

//start任务
void TaskStart(void* p_arg)
{
    uint32_t cpu_sr;

    //初始化统计任务
    OSStatInit();
    OS_ENTER_CRITICAL();
    TASK_UsbData_STK = mymalloc(SRAMEX, 256 * 4);
    OSTaskCreate(TaskUsbData, "TaskUsbData", (void*)0, (uint32_t*)&TASK_UsbData_STK[255], 10);
    OS_EXIT_CRITICAL();
    //延时1秒钟等待进入恢复
    Recovery();

    UnitRegister();
    TaskRegister();

    OS_ENTER_CRITICAL();
    __HAL_RCC_CRC_CLK_ENABLE();
    //挂起当前任务
    OSTaskSuspend(0);
    OS_EXIT_CRITICAL();
}

void TaskUsbData(void* p_arg)
{
    USB_ModeConfig();
    uint32_t usb_send_message_time_interval_ms = 1000;
    uint32_t usb_send_message_time_last = 0;
    uint8_t* l2_content_buffer;
    l2_content_buffer = mymalloc(SRAMEX, L2_CONTENT_BUFFER_SIZE);
    global_reponse_buffer = mymalloc(SRAMEX, GLOBAL_RESPONSE_BUFFER_SIZE);
    if ((l2_content_buffer == NULL) || (global_reponse_buffer == NULL))
    {
        LOG(LEVEL_ERROR, "Usb buffer malloc fail!\n");
        myfree(SRAMEX, l2_content_buffer);
        myfree(SRAMEX, global_reponse_buffer);
        OSTaskSuspend(10);
        return;
    }
    uint16_t l2_package_length = 0;
    uint8_t receive_state;
    while (1)
    {
        receive_state = usb_recv_L1data(l2_content_buffer, &l2_package_length);
        if (receive_state > 0)
        {
            l2_frame_resolve(l2_content_buffer, l2_package_length);
        }

        if (global_send_message_active_flag == 1)
        {
            if ((OSTimeGet() - usb_send_message_time_last) >= usb_send_message_time_interval_ms)
            {
                usb_send_message_time_last = OSTimeGet();
                usb_send_message_active_cpuuse();
            }
        }
        OSTimeDly(1);
    }
}

void Recovery()
{
    uint8_t usb_wait_cnt = 0;
	OSTimeDly(500);
	LOG(LEVEL_WARNING, "If I have a problem, please stop me.\n");
	LCD_LOG(LEVEL_WARNING, "If I have a problem, please stop me.\n");
	while (1)
	{
		if (global_recovery_flag == 0)
		{
			if (usb_wait_cnt < 10) //10 * 100 ms内做出反应
			{
				LOG(LEVEL_WARNING, ">>\r\n");
				LCD_LOG(LEVEL_WARNING, ">>\r\n");
				usb_wait_cnt++;
			}
			else
			{
				LOG(LEVEL_WARNING, "Next will run the app!\n");
				LCD_LOG(LEVEL_WARNING, "Next will run the app!\n");
				break;
			}
		}
		delay_ms(200);
	}
}