#include "sys.h"
#include "delay.h"
#include "log.h"
#include "ftl.h"  
#include "ff.h"
#include "exfuns.h"
#include "disk.h"
#include "config.h"
#include "string.h"

#define NAME_LEN_MAX    32
uint8_t local_name[NAME_LEN_MAX];


uint8_t check_name(uint8_t *name)
{
    uint8_t i;
    for(i = 0; (i < NAME_LEN_MAX) && (name[i] != 0); i++)
    {
        if((name[i] < ' ') || (name[i] > '~'))
        {
            break;
        }
    }
    
    if(i != 0)
    {
        if(name[i] == 0)
            return 0;
        else
            return 1;
    }
    else
    {
        return 1;
    }
    
}


void DISK_Init(void)
{
    uint8_t res;                    //��ʼ������ 
    for(uint8_t i=0;i<5;i++)
    {
        if(FTL_Init()==0)
        {
            break;
        }
        delay_ms(500);
        LOG(LEVEL_INFO,"NAND Error!   Please Check! \n");
    }
    exfuns_init();                  //Ϊfatfs��ر��������ڴ�	
    res=f_mount(fs[0],"0:",1);      //����NAND FLASH.
    if(res==FR_OK)
    {
        memset(local_name, 0, NAME_LEN_MAX);	//���������
        Config_Read(ALIAS_ADDR, (uint32_t *)local_name, ALIAS_SIZE);		//��ȡFLASH�е�CONFIG�����ı���
        if(check_name(local_name) == 0)
        {
            memset(local_name, 0, NAME_LEN_MAX);	//���������
            strcpy((char*)local_name, ALIAS_STRING);	//ʹ��Ĭ����
        }
        res = f_setlabel((const TCHAR *)local_name);
        //LOG(LEVEL_INFO, "res = %d, local_name %s \n\n", res, local_name);
    }else{
        LOG(LEVEL_INFO,"NAND Disk error\n ");
    }	
}