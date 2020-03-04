// cc.h����LWIP TCP/IPЭ��ջһ����
// ����: Adam Dunkels <adam@sics.se>

#ifndef __CC_H__
#define __CC_H__

#include "cpu.h"
#include "stdio.h"
#include "rtos.h"  //ʹ��UCOS Ҫ��Ӵ�ͷ�ļ���

//������ƽ̨�޹ص���������
typedef unsigned   char    u8_t;  	//�޷���8λ����  
typedef signed     char    s8_t;   	//�з���8λ���� 
typedef unsigned   short   u16_t;  	//�޷���16λ����
typedef signed     short   s16_t;   //�з���16λ����
typedef unsigned   long    u32_t;   //�޷���32λ����
typedef signed     long    s32_t;   //�з���32λ����
typedef u32_t mem_ptr_t;            //�ڴ��ַ������
typedef int sys_prot_t;				//�ٽ籣��������


#define SYS_ARCH_DECL_PROTECT(lev)	u32_t lev
#define SYS_ARCH_PROTECT(lev)		lev = OS_CPU_SR_Save() 	//UCOS II�н����ٽ���,���ж�
#define SYS_ARCH_UNPROTECT(lev)		OS_CPU_SR_Restore(lev)	//UCOS II���˳�A�ٽ��������ж� 


//���ݲ�ͬ�ı���������һЩ����
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

//LWIP��printf����ʱʹ�õ���һЩ����
#define U16_F "4d"
#define S16_F "4d"
#define X16_F "4x"
#define U32_F "8ld"
#define S32_F "8ld"
#define X32_F "8lx"

//�궨��
#ifndef LWIP_PLATFORM_ASSERT
#define LWIP_PLATFORM_ASSERT(x) 
#endif

#ifndef LWIP_PLATFORM_DIAG
#define LWIP_PLATFORM_DIAG(x)   
#endif

#endif /* __CC_H__ */
