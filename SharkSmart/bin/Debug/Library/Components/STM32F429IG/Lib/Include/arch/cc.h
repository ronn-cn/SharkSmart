// cc.h属于LWIP TCP/IP协议栈一部分
// 作者: Adam Dunkels <adam@sics.se>

#ifndef __CC_H__
#define __CC_H__

#include "cpu.h"
#include "stdio.h"
#include "rtos.h"  //使用UCOS 要添加此头文件！

//定义与平台无关的数据类型
typedef unsigned   char    u8_t;  	//无符号8位整数  
typedef signed     char    s8_t;   	//有符号8位整数 
typedef unsigned   short   u16_t;  	//无符号16位整数
typedef signed     short   s16_t;   //有符号16位整数
typedef unsigned   long    u32_t;   //无符号32位整数
typedef signed     long    s32_t;   //有符号32位整数
typedef u32_t mem_ptr_t;            //内存地址型数据
typedef int sys_prot_t;				//临界保护型数据


#define SYS_ARCH_DECL_PROTECT(lev)	u32_t lev
#define SYS_ARCH_PROTECT(lev)		lev = OS_CPU_SR_Save() 	//UCOS II中进入临界区,关中断
#define SYS_ARCH_UNPROTECT(lev)		OS_CPU_SR_Restore(lev)	//UCOS II中退出A临界区，开中断 


//根据不同的编译器定义一些符号
#if defined (__ICCARM__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT 
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_USE_INCLUDES

#elif defined (__CC_ARM)

#define PACK_STRUCT_BEGIN __packed
#define PACK_STRUCT_STRUCT 
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#elif defined (__GNUC__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#elif defined (__TASKING__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#endif

//LWIP用printf调试时使用到的一些类型
#define U16_F "4d"
#define S16_F "4d"
#define X16_F "4x"
#define U32_F "8ld"
#define S32_F "8ld"
#define X32_F "8lx"

//宏定义
#ifndef LWIP_PLATFORM_ASSERT
#define LWIP_PLATFORM_ASSERT(x) 
#endif

#ifndef LWIP_PLATFORM_DIAG
#define LWIP_PLATFORM_DIAG(x)   
#endif

#endif /* __CC_H__ */
