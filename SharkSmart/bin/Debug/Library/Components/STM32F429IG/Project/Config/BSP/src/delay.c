#include "delay.h"
#include "sys.h"
#include "rtos.h"
static uint32_t fac_us=0;							//us��ʱ������

//us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
void delay_osschedlock(void)
{								
	OSSchedLock();							
}

//us����ʱʱ,�ָ��������
void delay_osschedunlock(void)
{
	OSSchedUnlock();						
}
			   

void delay_init(uint8_t SYSCLK)
{
	uint32_t reload;
	fac_us=SYSCLK;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload=SYSCLK;					    //ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/1000;	//����delay_ostickspersec�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��180M��,Լ��0.745s����	
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //����SYSTICK
}								    
//��ʱnus
//nusΪҪ��ʱ��us��.	
//nus:0~190887435(���ֵ��2^32/fac_us@fac_us=22.5)	 
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	   
	delay_osschedlock(); 	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	delay_osschedunlock();
}

//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(uint16_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}
	