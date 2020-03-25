#include "config.h"
#include "log.h"
#include "malloc.h"	 
#include<string.h>


//config����д�ӿ� 
//AddrOffset ƫ�Ƶ�ַ
//pBuffer    Ҫд������
//NumToWrite Ҫд����4�ֽ���
//�����ݷ����Ͳ�������
void Config_Write(uint32_t AddrOffset,uint32_t *pBuffer,uint32_t NumToWrite)
{
	if( AddrOffset+CONFIG_BASEADDR < CONFIG_BASEADDR ||  AddrOffset % 4 ||  AddrOffset+CONFIG_BASEADDR >= APP_START_ADDERSS)
	{
		LOG(LEVEL_ERROR,"%s\n","Illegal address");
		return; //�Ƿ���ַ
	}
	uint32_t addrx;
	uint32_t endaddr;
	uint32_t *back_burrer = NULL; 
	uint32_t SectorError=0;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_FLASH_Unlock();							//���� 
   
	addrx= AddrOffset+CONFIG_BASEADDR;				//д�����ʼ��ַ
	endaddr= AddrOffset+CONFIG_BASEADDR+NumToWrite*4;	//д��Ľ�����ַ
	
	while(addrx<endaddr)		
	{
		if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
		{   
			back_burrer = mymalloc(SRAMEX,CONFIG_PARTITION_SIZE); 
			if(back_burrer == NULL)
            {
                LOG(LEVEL_ERROR,"MALLOC ERROR������\n");
				return;
            }
			Config_Read(0,&back_burrer[0],CONFIG_PARTITION_SIZE/4); //��������config������SARAM 
           
            FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //�������ͣ��������� 
            FlashEraseInit.Sector=STMFLASH_GetFlashSector(CONFIG_BASEADDR);  //Ҫ����������
            FlashEraseInit.NbSectors=1;                             //һ��ֻ����һ������
            FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //��ѹ��Χ��VCC=2.7~3.6V֮��!!
            
			if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
            {
                LOG(LEVEL_ERROR,"erase error :%x\n",SectorError);
                break;
            }
		}else addrx+=4;
        FLASH_WaitForLastOperation(FLASH_WAITETIME); 
	} 
    FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);
	
	if(FlashStatus==HAL_OK)
	{
        
		if(back_burrer != NULL) 	//֮ǰ���ݹ�����
		{
			memcpy((uint8_t *)back_burrer+AddrOffset,pBuffer,NumToWrite*4);
			uint32_t Offset=0;
			for(int i=0;i<CONFIG_PARTITION_SIZE/4;i++)
			{
                if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,CONFIG_BASEADDR+Offset,back_burrer[i])!=HAL_OK)
				{ 
					LOG(LEVEL_ERROR,"%s\n","RAM TO FLASH Write Error!");
					break;	//д���쳣
				}
				
				Offset +=4;
			}

			myfree(SRAMEX,back_burrer);
			back_burrer = NULL;
			
		}else
		{
	
			while( AddrOffset+CONFIG_BASEADDR<endaddr)//д����
			{
                if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,AddrOffset+CONFIG_BASEADDR,*pBuffer)!=HAL_OK)
				{ 
					LOG(LEVEL_ERROR,"%s\n","Flash Write Error!");
					break;	//д���쳣
				}
				LOG(LEVEL_ERROR,"%s\n","write data");
				AddrOffset+=4;
				pBuffer++;
			} 
		}
	}
    
	HAL_FLASH_Lock();//����
}


//config�������ӿ�
//AddrOffset ƫ�Ƶ�ַ
//pBuffer    Ҫ�������ݻ�����
//NumTowRead Ҫ������4�ֽ���

void Config_Read(uint32_t AddrOffset,uint32_t *pBuffer,uint32_t NumToRead)
{
	 if(AddrOffset+CONFIG_BASEADDR < CONFIG_BASEADDR || AddrOffset % 4 || AddrOffset+CONFIG_BASEADDR > (CONFIG_BASEADDR+CONFIG_PARTITION_SIZE) || NumToRead > CONFIG_PARTITION_SIZE/4)
	 {
        LOG(LEVEL_ERROR,"Config Read Error Addr!!!\n");
		return; //�Ƿ���ַ
	 }
	 
	 STMFLASH_Read(AddrOffset+CONFIG_BASEADDR,pBuffer,NumToRead);
}

//config���������ӿ�
//������������

void Config_Erase(void)
{
	HAL_FLASH_Unlock();
    
    FLASH_EraseInitTypeDef FlashEraseInit;
    
	
    uint8_t sector_base;
  
    uint32_t SectorError=0;
   
    sector_base = STMFLASH_GetFlashSector(CONFIG_BASEADDR);
  
    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //�������ͣ��������� 
    FlashEraseInit.Sector=sector_base;   //Ҫ����������
    FlashEraseInit.NbSectors=1;                             //һ��ֻ����һ������
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //��ѹ��Χ��VCC=2.7~3.6V֮��!!
    if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
    {
        LOG(LEVEL_DEBUG,"erase error :%x\n",SectorError);	
    }
	FLASH_WaitForLastOperation(FLASH_WAITETIME); 
    HAL_FLASH_Lock();  	
}



