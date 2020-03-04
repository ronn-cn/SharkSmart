#ifndef __L2_COMMUNICATION_H
#define __L2_COMMUNICATION_H
#include "sys.h"

#define	L2_CONTENT_BUFFER_SIZE	(65531)
#define GLOBAL_RESPONSE_BUFFER_SIZE (0x2000*4+128)

#define L2_HEADER_SIZE   				(2)		/*L2 header length*/
#define L2_HEADER_VERSION 				(0x00)	/*L2 header version*/
#define L2_KEY_SIZE         			(1)
#define L2_PAYLOAD_HEADER_SIZE			(3)		/*L2 payload header*/

#define L2_FIRST_VALUE_POS 				(L2_HEADER_SIZE + L2_PAYLOAD_HEADER_SIZE)

/**********************���ñ�����**************************/
//CPU����
#define	CPU_MODEL_STRING				"STM32F429IGT6"
#define CPU_FREQUENCY					192		//MHz
#define CONFIG_INFO_KEY02_VALUE_LENTH	18		
//�洢������
#define MEM_IN_RAM_SIZE					128		//KB
#define MEM_EX_RAM_SIZE					32		//MB
#define MEM_IN_FLASH_SIZE				1024	//KB
#define MEM_EX_FLASH_SIZE				128		//MB
#define MEM_OTHER_STRING				"NULL"
#define MEM_OTHER_SIZE					0		//KB
#define CONFIG_INFO_KEY04_VALUE_LENTH	18
//�豸ID
#define STM32_ID_ADDR					0x1FFF7A10
#define STM32_ID_SIZE					12		//B
#define CONFIG_INFO_KEY06_VALUE_LENTH	33
//��ȡ����
#define CONFIG_INFO_KEY09_VALUE_LENTH	33
/*********************************************************/

/***********************�豸״̬��Ϣ***********************/
#define DEVICE_STATUS_KEY02_VALUE_LENTH	5
#define DEVICE_STATUS_KEY04_VALUE_LENTH	18
#define DEVICE_STATUS_KEY06_VALUE_LENTH	2
#define DEVICE_STATUS_KEY08_VALUE_LENTH	5

#define OTHER_MEM_NAME					"MEMCCM"

/*********************************************************/

/************************ �� �� �� ***********************/
#define DEV_NAME_LENGTH					16		//�豸���ֳ���
#define DEV_MAX_NUM						15		//����豸��

//���豸  ������һ��IIC��SPI�����µ��豸
typedef struct sub_device
{
	uint8_t sub_descriptor;						//���豸������
	uint8_t sub_name[DEV_NAME_LENGTH];			//���豸����
    struct sub_device *sub_dev_next;                 //���豸
}sub_device_typedef; 
//���豸 ����SPI IIC USB
typedef struct device
{
	uint8_t name[DEV_NAME_LENGTH];				//�豸����
	uint8_t descriptor;							//���豸������
    uint8_t sub_dev_num;                        //���豸����
	sub_device_typedef *sub_dev_first;                //���豸
}device_typedef;
/*********************************************************/

/************************ ������� ***********************/
#define TASK_MAX_QUANTITY		OS_MAX_Task		//�����������
#define TASK_BYTES				32				//task bytes

#define TASK_START				0x00			//����״̬����ʼ
#define	TASK_STOP				0x01   			//����״̬��ֹͣ
/*********************************************************/

/* Command ID */
typedef enum {
    CONFIG_INFO_CMD_ID = 0x01,
    READ_MEMORY_COMMAND_ID = 0x02,
    DEBUG_VARIABLE_COMMAND_ID = 0x03,
    DEVICE_STATUS_INFO_COMMAND_ID = 0x04,
    DEVICE_MANAGEMENT_COMMAND_ID = 0x05,
    TASK_MANAGEMENT_COMMAND_ID = 0x06
   
}COMMUNICATE_COMMAND;

typedef enum  {
    KEY_GET_CPUINFO_REQUEST   = 0x01,
    KEY_GET_CPUINFO_RETURN  = 0x02,
	KEY_GET_MEMORYINFO_REQUEST = 0x03,
	KEY_GET_MEMORYINFO_RETURN = 0x04,
	KEY_GET_SOLEID_REQUEST = 0x05,
	KEY_GET_SOLEID_RETURN = 0x06,
	KEY_SETNAME = 0X07,
	KEY_GET_NAME_REQUEST = 0x08,
	KEY_GET_NAME_RETURN = 0x09
   
}CONFIG_TABLE_KEY;

typedef enum  {
    KEY_READ_MEMORY_REQUEST   = 0x01,
    KEY_READ_MEMORY_RETURN  = 0x02
   
}READ_MEMORY_KEY;

typedef enum  {
    KEY_READ_DEVICE_MANAGEMENT_REQUEST   = 0x01,
    KEY_READ_DEVICE_MANAGEMENT_RETURN  = 0x02
   
}READ_DEVICE_MANAGEMENT_KEY;

typedef enum  {
    KEY_READ_CPU_USAGE_REQUEST	= 0x01,
    KEY_READ_CPU_USAGE_RETURN	= 0x02,
    KEY_READ_MEM_USAGE_REQUEST	= 0x03,
    KEY_READ_MEM_USAGE_RETURN	= 0x04,	
	KEY_READ_DEVICE_TEMP_REQUEST= 0x05,
	KEY_READ_DEVICE_TEMP_RETURN	= 0x06,
	KEY_READ_POWER_INFO_REQUEST	= 0x07,
	KEY_READ_POWER_INFO_RETURN	= 0x08,
    KEY_SEND_MESSAGE_ACTIVE_OPEN    = 0x09,
    KEY_SEND_MESSAGE_ACTIVE_CLOSE   = 0x0A    
}READ_DEVICE_STATUS_KEY;

typedef enum  {
    DEBUG_SYSTEM_RESTQRT        = 0x01,

    DEBUG_SYSTEM_RECOVERY       = 0x02,
    DEBUG_SYSTEM_RECOVERY_RETURN= 0x03,
   
    DEBUG_SYSTEM_STATE          = 0x04,
    DEBUG_SYSTEM_STATE_RETURN   = 0x05,
}DEBUG_KEY;

typedef enum  {
    DEBUG_SYSTEM_STATE_BOOT     = 0x00,
    DEBUG_SYSTEM_STATE_APP      = 0x01
}DEBUG_SYSTEM_STATE_KEY;

typedef enum  {
    KEY_READ_TASK_MANAGEMENT_REQUEST   = 0x01,
    KEY_READ_TASK_MANAGEMENT_RETURN  = 0x02
}READ_TASK_MANAGEMENT_KEY;

extern uint8_t * global_reponse_buffer;
extern uint32_t global_recovery_flag;
extern uint8_t global_send_message_active_flag;
//��������ֽ���תС�������ֽ��� 16λ
void big_little_memcpy16(uint16_t * dest, const uint8_t *src);

//��������ֽ���תС�������ֽ��� 32λ
void big_little_memcpy32(uint32_t * dest, const uint8_t *src);

//С��uint32_t����ת�����uint32_t����
uint32_t little_to_big32(uint32_t little);
void l2_frame_resolve(uint8_t * data, uint16_t length);




void usb_send_message_active_cpuuse(void);
#endif
