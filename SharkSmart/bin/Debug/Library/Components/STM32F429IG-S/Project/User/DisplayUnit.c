#include "GLOBAL.h"
uint32_t*	TASKVIEW_STK;
void		TaskView(void *p_arg);

void DisplayUnit_Init();
void DisplayUnit_Init()
{
    //EV_LCD_DirSet(0);
    //RAW_Load();
    TASKVIEW_STK = mymalloc(SRAMEX, 512 * 4);
    OSTaskCreate(TaskView,"TaskView", (void*)0, (uint32_t*)&TASKVIEW_STK[511], EMWINDEMO_TASK_PRIO_NUM);
}

void TaskView(void* p_arg)
{
	CreatePage0();
    CreatePage1();
	ShowPage(page0);
    while (1)
    {
		GuiTouchExec();
        OSTimeDly(15);
    }
}