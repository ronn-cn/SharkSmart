#ifndef __OS_H
#define __OS_H	 
#include <stdint.h>
#include "os_task_prio.h"


/*
*********************************************************************************************************
*       Possible values for 'opt' argument of OSSemDel(), OSMboxDel(), OSQDel() and OSMutexDel()
*********************************************************************************************************
*/
#define  OS_DEL_NO_PEND                 0u
#define  OS_DEL_ALWAYS                  1u

#define OS_ERR      0
#define OS_OK       1

/*
*********************************************************************************************************
*                                             ERROR CODES
*********************************************************************************************************
*/
#define OS_ERR_NONE                     0u   //没有错误

#define OS_ERR_EVENT_TYPE               1u
#define OS_ERR_POST_NULL_PTR            3u
#define OS_ERR_PEVENT_NULL              4u
#define OS_ERR_INVALID_OPT              7u
#define OS_ERR_ID_INVALID               8u
#define OS_ERR_PDATA_NULL               9u

#define OS_ERR_TIMEOUT                 10u
#define OS_ERR_EVENT_NAME_TOO_LONG     11u
#define OS_ERR_PNAME_NULL              12u
#define OS_ERR_PEND_LOCKED             13u
#define OS_ERR_PEND_ABORT              14u
#define OS_ERR_ILLEGAL_CREATE_RUN_TIME 19u

#define OS_ERR_MBOX_FULL               20u

#define OS_ERR_Q_FULL                  30u
#define OS_ERR_Q_EMPTY                 31u

#define OS_ERR_PRIO_EXIST              40u
#define OS_ERR_PRIO                    41u
#define OS_ERR_PRIO_INVALID            42u

#define OS_ERR_SCHED_LOCKED            50u
#define OS_ERR_SEM_OVF                 51u

#define OS_ERR_TASK_DEL                61u
#define OS_ERR_TASK_DEL_IDLE           62u
#define OS_ERR_TASK_DEL_REQ            63u
#define OS_ERR_TASK_NAME_TOO_LONG      65u
#define OS_ERR_TASK_NO_MORE_TCB        66u
#define OS_ERR_TASK_NOT_EXIST          67u
#define OS_ERR_TASK_NOT_SUSPENDED      68u
#define OS_ERR_TASK_OPT                69u
#define OS_ERR_TASK_RESUME_PRIO        70u
#define OS_ERR_TASK_SUSPEND_IDLE       71u
#define OS_ERR_TASK_SUSPEND_PRIO       72u
#define OS_ERR_TASK_WAITING            73u

#define OS_TASK_STATE_DEL       0u
#define OS_TASK_STATE_RUN       1u
#define OS_TASK_STATE_RDY       2u
#define OS_TASK_STATE_DLY       3u
#define OS_TASK_STATE_SUSPENDED 4u

#define OS_TRUE                 1
#define OS_FALSE                0

#define OS_STAT_NULL            0
#define OS_STAT_FALSE           1
#define OS_STAT_TRUE            2


#define OS_EXCEPT_STK_SIZE 		1024u					//堆栈大小
#define OS_MAX_Task 			100u					//最大任务数(优先级数)
#define IDLE_STK_SIZE 			512					//空闲任务堆栈大小
#define STAT_STK_SIZE 			256					//空闲任务堆栈大小
#define OS_MAX_Event 			100u					//最大事件数
#define OS_PRIO_SELF			0xFFu				//任务自身
#define System_Ticks  			1000u					//每1000/System_Ticks ms进入一次中断
#define OS_MAX_Queue            100
/*---------------------------------------
-* 条件编译
-*---------------------------------------*/

/*---------------------------------------
-* 事件状态
-*---------------------------------------*/
#define  OS_STAT_PEND_OK                0u  
#define  OS_STAT_PEND_TO                1u
#define  OS_STAT_PEND_ABORT				2u

#define  OS_STAT_MUTEX_DLY              2u
#define  OS_STAT_MUTEX_NO_DLY           3u
#define  OS_STAT_Q_OK                	0u 
#define  OS_STAT_Q_TO                	1u
#define  OS_STAT_Q_ABORT				2u

/*---------------------------------------
-* Description：利用编译器扩展功能获得程序状态字，保存在局部变量cpu_sr
-*---------------------------------------*/
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}

typedef struct 	OS_Tcb		   
{
	uint32_t   *StkPtr;         //任务栈顶
	uint32_t    DLy;            //任务延时
    const char *NamePtr;        //任务名字
    uint8_t     OSTCBPrio;      //任务优先级
}TCB; //任务控制块

typedef struct 	OS_Ecb		   
{
	uint8_t 	OSEventTbl[OS_MAX_Task];    //事件等待表
	uint8_t 	state[OS_MAX_Task];         //任务时间状态
	uint32_t  	Cnt;                        //计数器
                                            //    信号量：信号量的值
                                            //    队列：  队列内消息数量

	uint32_t  	Prio;                       //优先级：互斥信号量

	void 		**Addr;//地址
    //uint32_t  	QMsg[OS_MAX_Queue];  //其中每个消息的长度
    uint32_t  	Size;
    uint32_t 	QStart;
    uint32_t 	QEnd;
}ECB; //事件控制块

extern uint32_t CPU_ExceptStk[OS_EXCEPT_STK_SIZE];//主任务堆栈
extern uint32_t *CPU_ExceptStkBase;//指向的是数组最后一个元素

extern TCB      TCB_Task[OS_MAX_Task];//任务控制块
extern TCB      *p_TCB_Cur;//指向当前任务的tcb
extern TCB      *p_TCBHightRdy;//指向最高级任务的tcb
extern uint32_t IDLE_STK[IDLE_STK_SIZE];
extern uint8_t  OSTaskTbl[OS_MAX_Task];

extern uint32_t OS_Time;	
extern uint8_t  OSCPUUsage;
extern uint32_t OS_IDLE_Ctr;
extern uint32_t OS_IDLE_CtrMAX;
extern uint8_t  OS_Running ;

extern void OSCtxSw(void);
extern void OSStartHighRdy(void);
extern uint32_t  OS_CPU_SR_Save(void);
extern void OS_CPU_SR_Restore(uint32_t  cpu_sr);
extern uint32_t  OSTimeGet(void);

void OSStatInit(void);

void    OSTaskSuspend(uint8_t  prio) ;
void    OSTaskResume(uint8_t prio);
uint8_t OSTaskCreate(void (*task)(void *arg),const char * p_name, void *arg, uint32_t  *stk, uint8_t  prio);
void    OSTaskStateSet(uint8_t prio, uint8_t state);
void    OSStart(void);
void    OSTimeDly(uint32_t  ticks);
void    OS_Sched(void);
void    OSSchedLock(void);
void    OSSchedUnlock(void);


ECB * OSSemCreate(uint8_t  cnt);
uint8_t OSSemPend(ECB  *pevent,uint32_t  time);
uint8_t OSSemPost(ECB  *pevent);
uint8_t OSSemDel(ECB  *pevent, uint8_t opt);
uint32_t  OSSemQuery(ECB  *pevent);

ECB * OSMutexCreate(void);
uint8_t OSMutexPend(ECB  *pevent,uint32_t  time);
uint8_t OSMutexPost(ECB  *pevent);
uint8_t OSMutexDel(ECB  *pevent, uint8_t opt);

ECB * OSQCreate(void	*start[],uint8_t  size);
void * OSQPend(ECB  *pevent,uint32_t time, uint8_t * perr);
uint8_t OSQPost(ECB  *pevent,void * pmsg);//, uint32_t  msg_size);
uint8_t OSQDel(ECB  *pevent, uint8_t opt);
uint8_t  OSQQuery(ECB * pevent);
void OS_SysTick_Handler(void);


#endif


