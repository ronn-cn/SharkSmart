#include "l2_usb_communication.h"
#include "log.h"
#include "stmflash.h"
#include "usb_communication.h"
#include  <string.h>
#include "config.h"
#include "malloc.h"
#include "sys.h"
#include "adc.h"
#include "ff.h"
#include "exfuns.h"
#include "rtos.h"

static void resolve_read_memory_command(uint8_t key,const uint8_t * value ,uint16_t length);
static void ret_memory_data(uint32_t base_addr, uint16_t num_items);
static void read_memory(uint32_t base_addr, uint16_t num_items, uint32_t * read_buffer , uint8_t is_big_endian);
static void resolve_config_table_command(uint8_t key, uint8_t * first_key_pos ,uint16_t length, uint16_t all_length);
static void resolve_device_status_command(uint8_t key, uint8_t * first_key_pos ,uint16_t length, uint16_t all_length);
static void resolve_device_management_command(uint8_t key, uint8_t * first_key_pos ,uint16_t length, uint16_t all_length);
static void resolve_task_management_command(uint8_t key, uint8_t * first_key_pos ,uint16_t length, uint16_t all_length);
static void resolve_debug_command(uint8_t key,const uint8_t * value ,uint16_t length);

/****************************************************************************
* Tempory send buffer
*****************************************************************************/
uint32_t global_recovery_flag = 0;
uint8_t * global_reponse_buffer;				//��������ָ��
uint8_t * global_current_reponse_buffer;		//��������ָ��
uint16_t  global_current_reponse_package_length;//�������ݳ���
uint8_t global_alias[32] = ALIAS_STRING;		//��������

device_typedef dev[DEV_MAX_NUM];				//�豸�����

uint8_t global_send_message_active_flag = 0;
//L2���ݽ���    
//data : L2 data
//length : L2 data length 
void l2_frame_resolve(uint8_t * data, uint16_t length)
{
	LOG(LEVEL_DEBUG,"l2 frame resolve. L2_package length:%d byte\n",length)
	//para check
	if((!data) || (length == 0)) 
	{
		LOG(LEVEL_ERROR,"l2_frame_resolve Illegal parameters!\n");
		return ;
	}
	
	global_current_reponse_buffer = global_reponse_buffer;
	global_current_reponse_package_length = 0;
	
	COMMUNICATE_COMMAND command_id;
	uint8_t first_key;                    /* first key of L2 payload*/
	uint16_t first_value_length;          /* length of first value */
	
	command_id      = (COMMUNICATE_COMMAND)data[0];
	LOG(LEVEL_DEBUG,"command_id:%x\n",command_id);
	//data[1] is version and reserve
	first_key = data[2];
	big_little_memcpy16(&first_value_length,&data[3]);	/* Occupancy length 2 byte*/
	switch(command_id)
	{
		case CONFIG_INFO_CMD_ID:				//������Ϣ	0x01 	
			resolve_config_table_command(first_key,data+ L2_HEADER_SIZE,first_value_length, length);
			break;					
		case READ_MEMORY_COMMAND_ID:			//���洢��	0x02
			resolve_read_memory_command(first_key,data+ L2_FIRST_VALUE_POS,first_value_length);
			break;
		case DEBUG_VARIABLE_COMMAND_ID:
			resolve_debug_command(first_key,data+ L2_FIRST_VALUE_POS,first_value_length);
			break;
		case DEVICE_STATUS_INFO_COMMAND_ID:		//�豸״̬ 	0x04
			resolve_device_status_command(first_key,data+ L2_HEADER_SIZE,first_value_length, length);
			break;		
		case DEVICE_MANAGEMENT_COMMAND_ID:		//�豸����  0x05
			resolve_device_management_command(first_key,data+ L2_HEADER_SIZE,first_value_length, length);
			break;
		case TASK_MANAGEMENT_COMMAND_ID:      //�������  0x06
			resolve_task_management_command(first_key,data+ L2_HEADER_SIZE,first_value_length, length);
			break;	
		default:
			break;
	}
}

//��������ֽ���תС�������ֽ��� 32λ
void big_little_memcpy32(uint32_t * dest, const uint8_t *src)
{
	*dest  = src[0]<<24;
	*dest |= src[1]<<16;
	*dest |= src[2]<<8;
	*dest |= src[3];
}

//��������ֽ���תС�������ֽ��� 16λ
void big_little_memcpy16(uint16_t * dest, const uint8_t *src)
{
	*dest = (src[0]<<8) + src[1];
}

//С��uint32_t����ת�����uint32_t����
uint32_t little_to_big32(uint32_t little)
{
	uint32_t big_value = 0;
	big_value  = ((uint8_t)little)<<24;
	big_value |= ((uint8_t)(little>>8))<<16;
	big_value |= ((uint8_t)(little>>16))<<8;
	big_value |= (uint8_t)(little>>24);
	return big_value;

}
//��������Ϣ
static void ret_cpu_info(uint8_t * data,uint8_t ret_value_length)
{
	memset(data, 0, 3 + ret_value_length);					//���������
	data[0] = KEY_GET_CPUINFO_RETURN;						//keyͷ
	data[1] = (uint8_t)((ret_value_length & 0xFF00) >> 8);	//keyvalue����
	data[2] = (uint8_t)(ret_value_length & 0x00FF);			
	strcpy((char *)&data[3],(char *)CPU_MODEL_STRING);		//CPU��Ϣ(�ַ���)
	data[19] = (uint8_t)((CPU_FREQUENCY & 0xFF00) >> 8);	//CPUƵ��
	data[20] = (uint8_t)(CPU_FREQUENCY & 0x00FF);
}

//�ڴ���Ϣ
static void ret_mem_info(uint8_t *data, uint8_t ret_value_length)
{
	memset(data, 0, 3 + ret_value_length);					//���������
	data[0] = KEY_GET_MEMORYINFO_RETURN;					//keyͷ
	data[1] = (uint8_t)((ret_value_length & 0xFF00) >> 8);	//key value����
	data[2] = (uint8_t)(ret_value_length & 0x00FF);
	
	data[3] = (uint8_t)((MEM_IN_RAM_SIZE & 0xFF00) >> 8);	//�ڲ�RAM��С
	data[4] = (uint8_t)(MEM_IN_RAM_SIZE & 0x00FF);
	data[5] = (uint8_t)((MEM_IN_FLASH_SIZE & 0xFF00) >> 8);	//�ڲ�Flash��С
	data[6] = (uint8_t)(MEM_IN_FLASH_SIZE & 0x00FF);
	data[7] = (uint8_t)((MEM_EX_RAM_SIZE & 0xFF00) >> 8);	//�ⲿRAM��С
	data[8] = (uint8_t)(MEM_EX_RAM_SIZE & 0x00FF);
	data[9] = (uint8_t)((MEM_EX_FLASH_SIZE & 0xFF00) >> 8);	//�ⲿFlash��С
	data[10] = (uint8_t)(MEM_EX_FLASH_SIZE & 0x00FF);
	strcpy((char *)&data[11],(char *)MEM_OTHER_STRING);		//�����洢��
	data[19] = (uint8_t)((MEM_OTHER_SIZE & 0xFF00) >> 8);	//�����洢����С
	data[20] = (uint8_t)(MEM_OTHER_SIZE & 0x00FF);			
}

//������ΨһID
static void ret_id_info(uint8_t *data, uint8_t ret_value_length)
{
	memset(data, 0, 3 + ret_value_length);					//���������
	data[0] = KEY_GET_SOLEID_RETURN;						//keyͷ
	data[1] = (uint8_t)((ret_value_length & 0xFF00) >> 8);	//key value ����
	data[2] = (uint8_t)(ret_value_length & 0x00FF);
	data[3] = STM32_ID_SIZE;								//STM32ID��С(12Byte)
	memcpy(&(data[4]), (uint32_t *)STM32_ID_ADDR, 12);		//STM32ID
}

//���ñ���
static void set_name(uint8_t *name_data)
{
	uint8_t name_len = name_data[0];	//�±����ĳ���
	
	memset(global_alias, 0, 32);		//���ԭ����
	memcpy(global_alias, &(name_data[1]), name_len);
	
	Config_Write(ALIAS_ADDR, (uint32_t *)global_alias, ALIAS_SIZE);		//����������FLASH�е�CONFIG����
	
	LOG(LEVEL_DEBUG,"name: %s  len: %d\n", global_alias, name_len);
}

//���ر���
static void ret_get_name(uint8_t *data, uint8_t ret_value_length)
{
	Config_Read(ALIAS_ADDR, (uint32_t *)global_alias, ALIAS_SIZE);		//��ȡFLASH�е�CONFIG�����ı���
	if((global_alias[0] < '0') || (global_alias[0] > 'z'))			//��ֵΪ0xFF ��˵�� FLASH��û�б���  ��Ҫʹ��Ĭ����
	{
		memset(global_alias, 0, 32);	//���������
		memcpy(global_alias, ALIAS_STRING, strlen(ALIAS_STRING));	//ʹ��Ĭ����
	}
	memset(data, 0, 3 + ret_value_length);
	data[0] = KEY_GET_NAME_RETURN;
	data[1] = (uint8_t)((ret_value_length & 0xFF00) >> 8);
	data[2] = (uint8_t)(ret_value_length & 0x00FF);
	data[3] = strlen((char *)global_alias);
	memcpy(&(data[4]), global_alias, strlen((char *)global_alias));
}

//�����豸��
static uint16_t ret_device_mangement_info(uint8_t *data)
{
	uint16_t key_length = 0;
	
	uint8_t * sub_dev_size;
	uint8_t all_sub_num=0;
	for(uint8_t dev_i = 0; dev_i < DEV_MAX_NUM; dev_i++)			//ͳ�ƹ��ж������豸
	{
		all_sub_num+=dev[dev_i].sub_dev_num;
	}
	memset(data, 0, (DEV_NAME_LENGTH+2)*DEV_MAX_NUM + L2_PAYLOAD_HEADER_SIZE+(DEV_NAME_LENGTH+1)*all_sub_num);			//��մ�������
	
	uint8_t * buf = &(data[3]);
	sub_device_typedef *p_sub;
	for(uint8_t dev_i = 0; dev_i < DEV_MAX_NUM; dev_i++)
	{
		if(dev[dev_i].name[0] != 0)							//�豸������Ϊ��  Ϊ����˵�����豸
		{
			strcpy((char *)buf, (char *)dev[dev_i].name);	//�豸��
			buf += DEV_NAME_LENGTH;						
			*buf = dev[dev_i].descriptor;					//�豸������
			buf++;
			sub_dev_size = buf;
			buf++;											//�����豸��С 
			p_sub=dev[dev_i].sub_dev_first;
			while(p_sub!=NULL)
			{
				strcpy((char *)buf, (char *)p_sub->sub_name);			//���豸��
				buf += DEV_NAME_LENGTH;
				*buf = p_sub->sub_descriptor;						//���豸������
				buf++;
				p_sub=p_sub->sub_dev_next;
			}
			*sub_dev_size = buf - sub_dev_size - 1;			//�������豸���� = ��ǰָ�� - ��ʼ
		}
	}
	key_length = buf - data - L2_PAYLOAD_HEADER_SIZE;		//key�ĳ��� = ��ǰָ�� - ��ʼָ�� - key��ͷ
	
	data[0] = KEY_READ_DEVICE_MANAGEMENT_RETURN;
	data[1] = (uint8_t)((key_length & 0xFF00) >> 8);
	data[2] = (uint8_t)(key_length & 0x00FF);
	
	return key_length;
}

//����CPUʹ����
static void ret_cpu_usage_info(uint8_t *data, uint8_t ret_value_length)
{
	memset(data, 0, L2_PAYLOAD_HEADER_SIZE + ret_value_length);
	data[0] = KEY_READ_CPU_USAGE_RETURN;
	data[1] = (uint8_t)((ret_value_length & 0xFF00) >> 8);
	data[2] = (uint8_t)(ret_value_length & 0x00FF);
	data[3] = OSCPUUsage;
}

//���ش洢��ʹ����
static void ret_mem_usage_info(uint8_t *data, uint8_t ret_value_length)
{
	uint16_t in_ram_margin 		= 0;	//KB
	uint16_t in_flash_margin 	= 0;	//KB
	uint16_t ex_ram_margin 		= 0;	//MB
	uint16_t ex_flash_margin 	= 0;	//MB
	uint16_t other_mem_margin	= 0;	//KB
	
	uint32_t margin_temp = 0;
	
	in_ram_margin = (uint16_t)(my_mem_margin(SRAMIN) >> 4);			//�ڲ�RAM
	ex_ram_margin = (uint16_t)(my_mem_margin(SRAMEX) >> 4 >> 10);	//�ⲿRAM
	
	in_flash_margin = 482;											//�ڲ�Flash �޷����� �ٶ�ֵ
	exf_getfree((uint8_t *)"0:",NULL,&margin_temp);						//�ⲿFlash
	ex_flash_margin = margin_temp >> 10;		
	
	other_mem_margin = (uint16_t)(my_mem_margin(SRAMCCM) >> 4);		//�����洢  �ڲ�Ram CCM
	
	memset(data, 0, L2_PAYLOAD_HEADER_SIZE + ret_value_length);
	data[0] = KEY_READ_MEM_USAGE_RETURN;
	data[1] = (uint8_t)((ret_value_length & 0xFF00) >> 8);
	data[2] = (uint8_t)(ret_value_length & 0x00FF);
	
	data[3] = (uint8_t)((in_ram_margin & 0xFF00) >> 8);				//�ڲ�RAMʣ��
	data[4] = (uint8_t)(in_ram_margin & 0x00FF);
	data[5] = (uint8_t)((in_flash_margin & 0xFF00) >> 8);			//�ڲ�Flashʣ��
	data[6] = (uint8_t)(in_flash_margin & 0x00FF);
	data[7] = (uint8_t)((ex_ram_margin & 0xFF00) >> 8);				//�ⲿRAMʣ��
	data[8] = (uint8_t)(ex_ram_margin & 0x00FF);	
	data[9] = (uint8_t)((ex_flash_margin & 0xFF00) >> 8);			//�ⲿFlashʣ��
	data[10] = (uint8_t)(ex_flash_margin & 0x00FF);		
	
	strcpy((char*)(&data[11]), OTHER_MEM_NAME);						//�����洢��
	
	data[19] = (uint8_t)((other_mem_margin & 0xFF00) >> 8);			//�����洢��ʣ��
	data[20] = (uint8_t)(other_mem_margin & 0x00FF);	
}

//�����豸�¶�
static void ret_device_temp_info(uint8_t *data, uint8_t ret_value_length)
{
	int16_t temp = 0;
	memset(data, 0, L2_PAYLOAD_HEADER_SIZE + ret_value_length);
	data[0] = KEY_READ_DEVICE_TEMP_RETURN;
	data[1] = (uint8_t)((ret_value_length & 0xFF00) >> 8);
	data[2] = (uint8_t)(ret_value_length & 0x00FF);
	temp = Get_Temprate();
	data[3] = temp/ 100;
}

//���ص�Դ��Ϣ
static void ret_power_info(uint8_t *data, uint8_t ret_value_length)
{
	uint16_t vcc_5v_mv = Get_5V_mV();
	uint16_t vcc_5v_mA = Get_5V_mA();
	memset(data, 0, L2_PAYLOAD_HEADER_SIZE + ret_value_length);
	data[0] = KEY_READ_POWER_INFO_RETURN;
	data[1] = (uint8_t)((ret_value_length & 0xFF00) >> 8);
	data[2] = (uint8_t)(ret_value_length & 0x00FF);
	data[3] = (uint8_t)((vcc_5v_mv & 0x00FF) << 8);	
	data[4] = (uint8_t)((vcc_5v_mv & 0xFF));
	data[5] = (uint8_t)((vcc_5v_mA & 0x00FF) << 8);	
	data[6] = (uint8_t)((vcc_5v_mA & 0xFF));
}

//���������
static uint16_t ret_task_management_info(uint8_t *data )
{
	memset(data, 0, 1 + TASK_MAX_QUANTITY*TASK_BYTES+ L2_PAYLOAD_HEADER_SIZE);	
	uint16_t key_length = 0;
	uint8_t i = 0;
	uint8_t task_quantity = 0;									//��������
	data[0] = KEY_READ_TASK_MANAGEMENT_RETURN;
	uint8_t *data_temp = (uint8_t*)(&(data[4]));
	for(i = 0;i<TASK_MAX_QUANTITY - 3;i++)
	{
		if(OSTaskTbl[i] == OS_TASK_STATE_DEL)
			continue;		
		task_quantity++;
		strcpy((char*)data_temp, (char*)TCB_Task[i].NamePtr);	//ǰ30��Byte
		data_temp += 30;
		*data_temp = i;												//���ȼ�
		data_temp++;
		*data_temp = (OSTaskTbl[i]  == OS_TASK_STATE_RDY) ? TASK_START : TASK_STOP; //task_status
		data_temp++;
	}
	key_length = data_temp - data - L2_PAYLOAD_HEADER_SIZE;
	data[1] = (uint8_t)((key_length & 0xFF00) >> 8);
	data[2] = (uint8_t)(key_length & 0x00FF);
	data[3] = (uint8_t)(task_quantity);
	return key_length;
}

//�����һ��key�Ƿ���Ч   ��Ч����1 ��Ч����0
uint8_t check_next_key_valid(uint8_t * start_pos,uint8_t * current_pos, uint16_t all_length)
{
	int16_t latency = all_length - ((uint32_t)current_pos - (uint32_t)start_pos) - L2_HEADER_SIZE;
	
	LOG(LEVEL_DEBUG,"latency : %d\n",latency);
	if((*current_pos != 0) && (latency >= L2_PAYLOAD_HEADER_SIZE))
		return 1;
	else
		return 0;
}
/*
key : 	��һ��key��ֵ
data: 	��һ��key�ĵ�ַ
length: ��һ��key�ĳ���
all_length:	key���ܳ���
*/
static void resolve_config_table_command(uint8_t key, uint8_t * first_key_pos ,uint16_t length, uint16_t all_length)
{
	uint8_t *key_pos = first_key_pos;						//key����ʼ��ַ
	uint8_t *key_value = key_pos + L2_PAYLOAD_HEADER_SIZE;	//key�����ݵ�ַ
	uint8_t response_is_need = FALSE;						//Ĭ�ϲ���Ҫ����
	global_current_reponse_buffer = global_reponse_buffer;	//���ص�����ָ��
	global_current_reponse_buffer[0] = CONFIG_INFO_CMD_ID;	//���ص�����
	global_current_reponse_buffer[1] = L2_HEADER_VERSION;	//���ص�ͷ�İ汾
	global_current_reponse_buffer += L2_HEADER_SIZE;		//��������ָ�����2��CMD��
	global_current_reponse_package_length = L2_HEADER_SIZE;	//�������ݳ���

__CONTINUE_PROCESSING:	
	LOG(LEVEL_DEBUG,"key : %d   length : %d\n", key, length);
	switch(key)
	{
		case KEY_GET_CPUINFO_REQUEST:		//��ȡ��������Ϣ����  	0x01
			if(length == 0)
			{	
				LOG(LEVEL_DEBUG,"KEY_GET_CPUINFO_REQUEST\n");
				response_is_need = TRUE;	//��Ҫ����
				ret_cpu_info(global_current_reponse_buffer,CONFIG_INFO_KEY02_VALUE_LENTH);	//��CPU��Ϣ��ӽ����ͻ�����
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + CONFIG_INFO_KEY02_VALUE_LENTH;	//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + CONFIG_INFO_KEY02_VALUE_LENTH;	//��������ָ�����
			}
			break;
		case KEY_GET_MEMORYINFO_REQUEST:	//��ȡ�洢����Ϣ����		0x03
			if(length == 0)
			{
				LOG(LEVEL_DEBUG,"KEY_GET_MEMORYINFO_REQUEST\n");
				response_is_need = TRUE;	//��Ҫ����
				ret_mem_info(global_current_reponse_buffer, CONFIG_INFO_KEY04_VALUE_LENTH);	//���洢����Ϣ��ӽ����ͻ�����
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + CONFIG_INFO_KEY04_VALUE_LENTH;	//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + CONFIG_INFO_KEY04_VALUE_LENTH;	//��������ָ�����
			}
			break;
		case KEY_GET_SOLEID_REQUEST:		//��ȡ�豸ΨһID����		0x05
			if(length == 0)
			{
				LOG(LEVEL_DEBUG,"KEY_GET_SOLEID_REQUEST\n");
				response_is_need = TRUE;	//��Ҫ����
				ret_id_info(global_current_reponse_buffer, CONFIG_INFO_KEY06_VALUE_LENTH);	//��оƬΨһID��ӵ����ͻ�����
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + CONFIG_INFO_KEY06_VALUE_LENTH;	//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + CONFIG_INFO_KEY06_VALUE_LENTH;	//��������ָ�����
			}
			break;
		case KEY_SETNAME:					//���ñ���				0x07
			if(length == 33)				//���ݳ���Ϊ33
			{
				LOG(LEVEL_DEBUG,"KEY_SETNAME\n");
				set_name(key_value);			//��������
			}
			break;
		case KEY_GET_NAME_REQUEST:			//��ȡ��������			0x08
			if(length == 0)
			{
				LOG(LEVEL_DEBUG,"KEY_GET_NAME_REQUEST\n");
				response_is_need = TRUE;	//��Ҫ����
				ret_get_name(global_current_reponse_buffer, CONFIG_INFO_KEY09_VALUE_LENTH);
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + CONFIG_INFO_KEY09_VALUE_LENTH;	//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + CONFIG_INFO_KEY09_VALUE_LENTH;	//��������ָ�����				
			}
			break;
		default:
            break;
	}
	key_pos += L2_PAYLOAD_HEADER_SIZE + length;			//key��ָ������ƣ�����һ��key
	if(check_next_key_valid(first_key_pos, key_pos, all_length))	//�жϺ����Ƿ�������
	{
		key = key_pos[0];								//��һ��key��ֵ
		big_little_memcpy16(&length, &key_pos[1]);		//��һ��key�ĳ���
		key_value = key_pos + L2_PAYLOAD_HEADER_SIZE;		//��һ��key������ָ��
		goto __CONTINUE_PROCESSING;
	}
	
	if(response_is_need  == TRUE)
	{
		usb_send_L1data(global_reponse_buffer,global_current_reponse_package_length);
		global_current_reponse_buffer = NULL;
		global_current_reponse_package_length = 0;
	}
}

//��ȡ�ڴ�
static void resolve_read_memory_command(uint8_t key,const uint8_t * value ,uint16_t length)
{
	switch(key)
	{
		case 0x01:
			if(length == 6)
			{
				uint32_t base_addr;
				uint16_t num_items;
//				LOG(LEVEL_DEBUG,"%x %x %x %x %x %x \n",value[0],value[1],value[2],value[3],value[4],value[5]);
			    big_little_memcpy32(&base_addr,value);
				big_little_memcpy16(&num_items,value+4);
				
//				LOG(LEVEL_DEBUG,"base:%x  num_items:%x\n",base_addr,num_items);
				ret_memory_data(base_addr,num_items);
			}
			
			break;
		
		default:
            break;
	}
}

static void resolve_device_status_command(uint8_t key, uint8_t * first_key_pos ,uint16_t length, uint16_t all_length)
{
	uint16_t key_length = 0;								//key��value����
	uint8_t *key_pos = first_key_pos;						//key����ʼ��ַ
	// uint8_t *key_value = key_pos + L2_PAYLOAD_HEADER_SIZE;	//key�����ݵ�ַ
	uint8_t response_is_need = FALSE;						//Ĭ�ϲ���Ҫ����
	global_current_reponse_buffer = global_reponse_buffer;	//���ص�����ָ��
	global_current_reponse_buffer[0] = DEVICE_STATUS_INFO_COMMAND_ID;		//���ص�����
	global_current_reponse_buffer[1] = L2_HEADER_VERSION;	//���ص�ͷ�İ汾
	global_current_reponse_buffer += L2_HEADER_SIZE;		//��������ָ�����2��CMD��
	global_current_reponse_package_length = L2_HEADER_SIZE;	//�������ݳ���

__CONTINUE_PROCESSING:	
	LOG(LEVEL_DEBUG,"key : %d   length : %d\n", key, length);
	switch(key)
	{
		case KEY_READ_CPU_USAGE_REQUEST:	//CPUʹ����
			if(length == 0)
			{
				LOG(LEVEL_DEBUG,"KEY_READ_CPU_USAGE_REQUEST\n");
				response_is_need = TRUE;					//��Ҫ����
				ret_cpu_usage_info(global_current_reponse_buffer,DEVICE_STATUS_KEY02_VALUE_LENTH);	//CPUʹ����
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + DEVICE_STATUS_KEY02_VALUE_LENTH;		//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + DEVICE_STATUS_KEY02_VALUE_LENTH;		//��������ָ�����
				LOG(LEVEL_DEBUG,"key_length : %d\n",key_length);
			}
			break;
		case KEY_READ_MEM_USAGE_REQUEST:	//�洢��ʹ����
			if(length == 0)
			{
				LOG(LEVEL_DEBUG,"KEY_READ_MEM_USAGE_REQUEST\n");
				response_is_need = TRUE;	//��Ҫ����
				ret_mem_usage_info(global_current_reponse_buffer,DEVICE_STATUS_KEY04_VALUE_LENTH);	//�洢��ʹ����
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + DEVICE_STATUS_KEY04_VALUE_LENTH;		//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + DEVICE_STATUS_KEY04_VALUE_LENTH;		//��������ָ�����
				LOG(LEVEL_DEBUG,"key_length : %d\n",key_length);
			}
			break;
		case KEY_READ_DEVICE_TEMP_REQUEST:	//�豸�¶�
			if(length == 0)
			{
				LOG(LEVEL_DEBUG,"KEY_READ_DEVICE_TEMP_REQUEST\n");
				response_is_need = TRUE;	//��Ҫ����
				ret_device_temp_info(global_current_reponse_buffer,DEVICE_STATUS_KEY06_VALUE_LENTH);//�豸�¶�
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + DEVICE_STATUS_KEY06_VALUE_LENTH;		//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + DEVICE_STATUS_KEY06_VALUE_LENTH;		//��������ָ�����
				LOG(LEVEL_DEBUG,"key_length : %d\n",key_length);
			}
			break;
		case KEY_READ_POWER_INFO_REQUEST:	//��Դ��Ϣ
			if(length == 0)
			{
				LOG(LEVEL_DEBUG,"KEY_READ_POWER_INFO_REQUEST\n");
				response_is_need = TRUE;	//��Ҫ����
				ret_power_info(global_current_reponse_buffer, DEVICE_STATUS_KEY08_VALUE_LENTH);		//��Դ��Ϣ
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + DEVICE_STATUS_KEY08_VALUE_LENTH;		//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + DEVICE_STATUS_KEY08_VALUE_LENTH;		//��������ָ�����
				LOG(LEVEL_DEBUG,"key_length : %d\n",key_length);
			}
			break;	
		case KEY_SEND_MESSAGE_ACTIVE_OPEN:
			if(length == 0)
			{
				global_send_message_active_flag = 1;
			}	
			break;
		case KEY_SEND_MESSAGE_ACTIVE_CLOSE:
			if(length == 0)
			{
				global_send_message_active_flag = 0;
			}		
			break;						
		default:
            break;
	}
	key_pos += L2_PAYLOAD_HEADER_SIZE + length;			//key��ָ������ƣ�����һ��key
	if(check_next_key_valid(first_key_pos, key_pos, all_length))	//�жϺ����Ƿ�������
	{
		key = key_pos[0];								//��һ��key��ֵ
		big_little_memcpy16(&length, &key_pos[1]);		//��һ��key�ĳ���
		// key_value = key_pos + L2_PAYLOAD_HEADER_SIZE;	//��һ��key������ָ��
		goto __CONTINUE_PROCESSING;
	}
	
	if(response_is_need  == TRUE)
	{
		usb_send_L1data(global_reponse_buffer,global_current_reponse_package_length);
		global_current_reponse_buffer = NULL;
		global_current_reponse_package_length = 0;
	}
}

//�豸����
static void resolve_device_management_command(uint8_t key, uint8_t * first_key_pos ,uint16_t length, uint16_t all_length)
{
	uint16_t key_length;									//key��value����
	uint8_t *key_pos = first_key_pos;						//key����ʼ��ַ
	// uint8_t *key_value = key_pos + L2_PAYLOAD_HEADER_SIZE;	//key�����ݵ�ַ
	uint8_t response_is_need = FALSE;						//Ĭ�ϲ���Ҫ����
	global_current_reponse_buffer = global_reponse_buffer;	//���ص�����ָ��
	global_current_reponse_buffer[0] = DEVICE_MANAGEMENT_COMMAND_ID;		//���ص�����
	global_current_reponse_buffer[1] = L2_HEADER_VERSION;	//���ص�ͷ�İ汾
	global_current_reponse_buffer += L2_HEADER_SIZE;		//��������ָ�����2��CMD��
	global_current_reponse_package_length = L2_HEADER_SIZE;	//�������ݳ���

__CONTINUE_PROCESSING:	
	LOG(LEVEL_DEBUG,"key : %d   length : %d\n", key, length);
	switch(key)
	{
		case KEY_READ_DEVICE_MANAGEMENT_REQUEST:			//�豸����
			if(length == 0)
			{	
				LOG(LEVEL_DEBUG,"KEY_READ_DEVICE_MANAGEMENT_REQUEST\n");
				response_is_need = TRUE;	//��Ҫ����
				key_length = ret_device_mangement_info(global_current_reponse_buffer);			//�豸����ͷ
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + key_length;	//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + key_length;	//��������ָ�����
				LOG(LEVEL_DEBUG,"key_length : %d\n",key_length);
			}
			break;
		default:
            break;
	}
	key_pos += L2_PAYLOAD_HEADER_SIZE + length;			//key��ָ������ƣ�����һ��key
	if(check_next_key_valid(first_key_pos, key_pos, all_length))	//�жϺ����Ƿ�������
	{
		key = key_pos[0];								//��һ��key��ֵ
		big_little_memcpy16(&length, &key_pos[1]);		//��һ��key�ĳ���
		// key_value = key_pos + L2_PAYLOAD_HEADER_SIZE;	//��һ��key������ָ��
		goto __CONTINUE_PROCESSING;
	}
	
	if(response_is_need  == TRUE)
	{
		usb_send_L1data(global_reponse_buffer,global_current_reponse_package_length);
		global_current_reponse_buffer = NULL;
		global_current_reponse_package_length = 0;
	}
}

/*
base_addr: 		���洢������ַ
num_items: 		������word(32bit)
read_buffer:	����������
is_big_endian:	���������ݴ��Ϊ��˻���С�� 0x01��� 0x00 С��
*/
static void read_memory(uint32_t base_addr, uint16_t num_items, uint32_t * read_buffer , uint8_t is_big_endian)
{	

	uint32_t i = 0;
	uint32_t offset = 0;
	for(i=0 ;i<num_items ;i++)
	{
		if(is_big_endian == 1) //���
		{	
			read_buffer[i] = little_to_big32(STMFLASH_ReadWord(base_addr+offset));
		}
		else				  //С��
		{
			read_buffer[i] = STMFLASH_ReadWord(base_addr+offset);
		}
		offset +=4;
	}
}

/*
base_addr: ��ȡ��ʼ��ַ
num_items: ��ȡ����(32λ���ݶ�ȡ)
�洢���ɶ���ַ��Χ:
0x00000000------0x001FFFFF
0x08000000------0x081FFFFF
0x1FFEC000------0x1FFF7FFF
0x20000000------0x2002FFFF
0x30000000------0x40007FFF
0x40010000------0x40017FFF
0x40020000------0x40022BFF
0x40023000------0x400233FF
0x40023800------0x40024FFF
0x40026000------0x400267FF
0x40028000------0x40029FFF
0x40040000------0x4007FFFF
0x50000000------0x5003FFFF
0x50050000------0x500503FF
0x50060000------0x50060BFF
0x60000000------0x63FFFFFF
0x80000000------0x8FFFFFFF
0xA0000000------0xA0000FFF
0xC0000000------0xFFFFFFFF
*/
static void ret_memory_data(uint32_t base_addr, uint16_t num_items)
{
	uint16_t l2_package_length;
	uint16_t value_length=0;
	uint8_t	err_code = 0;
	
	global_reponse_buffer[0] = READ_MEMORY_COMMAND_ID;
	global_reponse_buffer[1] = L2_HEADER_VERSION;
	global_reponse_buffer[2] = KEY_READ_MEMORY_RETURN;

	if(base_addr % 4 != 0 
		|| (base_addr < 0x08000000) & (base_addr > 0x001FFFFF) || (base_addr > 0x081FFFFF) & (base_addr < 0x1FFEC000) 
		|| (base_addr > 0x1FFF7FFF) & (base_addr < 0x20000000) || (base_addr > 0x2002FFFF) & (base_addr < 0x30000000) 
		|| (base_addr > 0x40007FFF) & (base_addr < 0x40010000) || (base_addr > 0x40017FFF) & (base_addr < 0x40020000)
		|| (base_addr > 0x40022BFF) & (base_addr < 0x40023000) || (base_addr > 0x400233FF) & (base_addr < 0x40023800)
		|| (base_addr > 0x40024FFF) & (base_addr < 0x40026000) || (base_addr > 0x400267FF) & (base_addr < 0x40028000)
		|| (base_addr > 0x40029FFF) & (base_addr < 0x40040000) || (base_addr > 0x4007FFFF) & (base_addr < 0x50000000)
		|| (base_addr > 0x5003FFFF) & (base_addr < 0x50050000) || (base_addr > 0x500503FF) & (base_addr < 0x50060000)
		|| (base_addr > 0x50060BFF) & (base_addr < 0x60000000) || (base_addr > 0x63FFFFFF) & (base_addr < 0x80000000)
	    || (base_addr > 0x8FFFFFFF) & (base_addr < 0xA0000000) || (base_addr > 0xA0000FFF) & (base_addr < 0xC0000000))                                                            
	{
		err_code = 0x01;							//���洢��ʧ�ܣ���ַ����Χ
	}
	
	if(num_items > 0x2000)
	{
		err_code = 0x03;							//���洢��ʧ�ܣ�Ҫ����word�������Ϸ�
	}
	if(err_code ==0)
	{
		value_length = num_items*4 + 3;
		global_reponse_buffer[3] = (uint8_t)((value_length & 0xFF00) >> 8);
		global_reponse_buffer[4] = (uint8_t)(value_length & 0x00FF);
		global_reponse_buffer[5] = err_code;
		global_reponse_buffer[6] = (uint8_t)(num_items & 0xff00)>>8;
		global_reponse_buffer[7] = (uint8_t)(num_items & 0x00FF);
		read_memory(base_addr,num_items,(uint32_t *)&(global_reponse_buffer[8]),1);//���
	}
	else if(err_code ==0x01 || err_code ==0x03)
	{
		value_length = 1;
		global_reponse_buffer[3] = (uint8_t)((value_length & 0xFF00) >> 8);
		global_reponse_buffer[4] = (uint8_t)(value_length & 0x00FF);
		global_reponse_buffer[5] = err_code;
		
	}
	l2_package_length = L2_HEADER_SIZE + L2_PAYLOAD_HEADER_SIZE + value_length;
	usb_send_L1data(global_reponse_buffer,l2_package_length);
}
/*
key:key��ֵ��
value����Я�������ݣ�
length�����ݵĴ�С��
*/
static void resolve_debug_command(uint8_t key,const uint8_t * value ,uint16_t length)
{	
	
	uint8_t response_is_need = FALSE;						//Ĭ�ϲ���Ҫ����
	global_current_reponse_buffer = global_reponse_buffer;	//���ص�����ָ��
	global_current_reponse_buffer[0] = DEBUG_VARIABLE_COMMAND_ID;		//���ص�����
	global_current_reponse_buffer[1] = L2_HEADER_VERSION;	//���ص�ͷ�İ汾
	global_current_reponse_buffer += L2_HEADER_SIZE;		//��������ָ�����2��CMD��
	global_current_reponse_package_length = L2_HEADER_SIZE;	//�������ݳ���		
	switch(key)
	{
		case DEBUG_SYSTEM_RESTQRT:
			HAL_NVIC_SystemReset(); //ϵͳ����
			break; 
		case DEBUG_SYSTEM_RECOVERY:
			global_recovery_flag = 1;
			Config_Write(RECOVERY_FLAG_ADDR, &global_recovery_flag, 1);
			LOG(LEVEL_DEBUG,"DEBUG_SYSTEM_RECOVERY\n");
			response_is_need = TRUE;					//��Ҫ����
			global_reponse_buffer[0] = DEBUG_VARIABLE_COMMAND_ID;
			global_reponse_buffer[1] = L2_HEADER_VERSION;
			global_reponse_buffer[2] = DEBUG_SYSTEM_RECOVERY_RETURN;
			global_reponse_buffer[3] = 0;
			global_reponse_buffer[4] = 0;
		
			global_current_reponse_package_length = 5;
			break;
		case DEBUG_SYSTEM_STATE:
			global_recovery_flag = 1;
			LOG(LEVEL_DEBUG,"DEBUG_SYSTEM_STATE\n");
			response_is_need = TRUE;					//��Ҫ����
			global_reponse_buffer[0] = DEBUG_VARIABLE_COMMAND_ID;
			global_reponse_buffer[1] = L2_HEADER_VERSION;
			global_reponse_buffer[2] = DEBUG_SYSTEM_STATE_RETURN;
			global_reponse_buffer[3] = DEBUG_SYSTEM_STATE_APP;
			global_reponse_buffer[4] = 0;
		
			global_current_reponse_package_length = 4;
			break;
		default:
				break;
	}

	if(response_is_need  == TRUE)
	{
		usb_send_L1data(global_reponse_buffer,global_current_reponse_package_length);
		global_current_reponse_buffer = NULL;
		global_current_reponse_package_length = 0;
	}	
}
//�������
static void resolve_task_management_command(uint8_t key, uint8_t * first_key_pos ,uint16_t length, uint16_t all_length)
{
	uint16_t key_length;									//key��value����
	// uint8_t *key_pos = first_key_pos;						//key����ʼ��ַ
	// uint8_t *key_value = key_pos + L2_PAYLOAD_HEADER_SIZE;	//key�����ݵ�ַ
	uint8_t response_is_need = FALSE;						//Ĭ�ϲ���Ҫ����
	global_current_reponse_buffer = global_reponse_buffer;	//���ص�����ָ��
	global_current_reponse_buffer[0] = TASK_MANAGEMENT_COMMAND_ID;		//���ص�����
	global_current_reponse_buffer[1] = L2_HEADER_VERSION;	//���ص�ͷ�İ汾
	global_current_reponse_buffer += L2_HEADER_SIZE;		//��������ָ�����2��CMD��
	global_current_reponse_package_length = L2_HEADER_SIZE;	//�������ݳ���	
	LOG(LEVEL_DEBUG,"key : %d   length : %d\n", key, length);
	switch(key)
	{
		case KEY_READ_TASK_MANAGEMENT_REQUEST:				//��ȡ��������Ϣ����  	0x01
			if(length == 0)
			{	
				LOG(LEVEL_DEBUG,"KEY_READ_TASK_MANAGEMENT_REQUEST\n");
				response_is_need = TRUE;					//��Ҫ����
				key_length = ret_task_management_info(global_current_reponse_buffer);			
				global_current_reponse_package_length 	+= L2_PAYLOAD_HEADER_SIZE + key_length;	//�������ݳ�������
				global_current_reponse_buffer 			+= L2_PAYLOAD_HEADER_SIZE + key_length;	//��������ָ�����
				LOG(LEVEL_DEBUG,"key_length : %d\n",key_length);
			}
			break;
		default:
			break;
	}
	if(response_is_need  == TRUE)
	{
		usb_send_L1data(global_reponse_buffer,global_current_reponse_package_length);
		global_current_reponse_buffer = NULL;
		global_current_reponse_package_length = 0;
	}
}

void usb_send_message_active(uint8_t l2_head_cmd, uint8_t *playload, uint16_t playload_len)
{
	global_reponse_buffer[0] = l2_head_cmd;
	global_reponse_buffer[1] = L2_HEADER_VERSION;
	memcpy(global_reponse_buffer + 2, playload, playload_len);
	usb_send_L1data(global_reponse_buffer, playload_len + 2);
}

void usb_send_message_active_cpuuse(void)
{
	uint8_t send_data[8] = {0};
	send_data[0] = KEY_READ_CPU_USAGE_RETURN;
	send_data[1] = 0;
	send_data[2] = 1;
	send_data[3] = OSCPUUsage;	
	usb_send_message_active(DEVICE_STATUS_INFO_COMMAND_ID, send_data, 4);
}