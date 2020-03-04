#include "delay.h"
#include "sys.h"
#include "rtos.h"
static uint32_t fac_us=0;							//us延时倍乘数

//us级延时时,关闭任务调度(防止打断us级延迟)
void delay_osschedlock(void)
{								
	OSSchedLock();							
}

//us级延时时,恢复任务调度
void delay_osschedunlock(void)
{
	OSSchedUnlock();						
}
			   

void delay_init(uint8_t SYSCLK)
{
	uint32_t reload;
	fac_us=SYSCLK;						//不论是否使用OS,fac_us都需要使用
	reload=SYSCLK;					    //每秒钟的计数次数 单位为K	   
	reload*=1000000/1000;	//根据delay_ostickspersec设定溢出时间
											//reload为24位寄存器,最大值:16777216,在180M下,约合0.745s左右	
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK
}								    
//延时nus
//nus为要延时的us数.	
//nus:0~190887435(最大值即2^32/fac_us@fac_us=22.5)	 
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	   
	delay_osschedlock(); 	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
	delay_osschedunlock();
}

//延时nms
//nms:要延时的ms数
void delay_ms(uint16_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}
	