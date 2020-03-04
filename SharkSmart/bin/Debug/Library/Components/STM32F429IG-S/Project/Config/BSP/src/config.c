#include "config.h"
#include "log.h"
#include "malloc.h"	 
#include<string.h>


//config分区写接口 
//AddrOffset 偏移地址
//pBuffer    要写的数据
//NumToWrite 要写几个4字节数
//带备份分区和擦除功能
void Config_Write(uint32_t AddrOffset,uint32_t *pBuffer,uint32_t NumToWrite)
{
	if( AddrOffset+CONFIG_BASEADDR < CONFIG_BASEADDR ||  AddrOffset % 4 ||  AddrOffset+CONFIG_BASEADDR >= APP_START_ADDERSS)
	{
		LOG(LEVEL_ERROR,"%s\n","Illegal address");
		return; //非法地址
	}
	uint32_t addrx;
	uint32_t endaddr;
	uint32_t *back_burrer = NULL; 
	uint32_t SectorError=0;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_FLASH_Unlock();							//解锁 
   
	addrx= AddrOffset+CONFIG_BASEADDR;				//写入的起始地址
	endaddr= AddrOffset+CONFIG_BASEADDR+NumToWrite*4;	//写入的结束地址
	
	while(addrx<endaddr)		
	{
		if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
		{   
			back_burrer = mymalloc(SRAMEX,CONFIG_PARTITION_SIZE); 
			if(back_burrer == NULL)
            {
                LOG(LEVEL_ERROR,"MALLOC ERROR！！！\n");
				return;
            }
			Config_Read(0,&back_burrer[0],CONFIG_PARTITION_SIZE/4); //备份整个config分区到SARAM 
           
            FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
            FlashEraseInit.Sector=STMFLASH_GetFlashSector(CONFIG_BASEADDR);  //要擦除的扇区
            FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
            FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
            
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
        
		if(back_burrer != NULL) 	//之前备份过扇区
		{
			memcpy((uint8_t *)back_burrer+AddrOffset,pBuffer,NumToWrite*4);
			uint32_t Offset=0;
			for(int i=0;i<CONFIG_PARTITION_SIZE/4;i++)
			{
                if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,CONFIG_BASEADDR+Offset,back_burrer[i])!=HAL_OK)
				{ 
					LOG(LEVEL_ERROR,"%s\n","RAM TO FLASH Write Error!");
					break;	//写入异常
				}
				
				Offset +=4;
			}

			myfree(SRAMEX,back_burrer);
			back_burrer = NULL;
			
		}else
		{
	
			while( AddrOffset+CONFIG_BASEADDR<endaddr)//写数据
			{
                if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,AddrOffset+CONFIG_BASEADDR,*pBuffer)!=HAL_OK)
				{ 
					LOG(LEVEL_ERROR,"%s\n","Flash Write Error!");
					break;	//写入异常
				}
				LOG(LEVEL_ERROR,"%s\n","write data");
				AddrOffset+=4;
				pBuffer++;
			} 
		}
	}
    
	HAL_FLASH_Lock();//上锁
}


//config分区读接口
//AddrOffset 偏移地址
//pBuffer    要读的数据缓冲区
//NumTowRead 要读几个4字节数

void Config_Read(uint32_t AddrOffset,uint32_t *pBuffer,uint32_t NumToRead)
{
	 if(AddrOffset+CONFIG_BASEADDR < CONFIG_BASEADDR || AddrOffset % 4 || AddrOffset+CONFIG_BASEADDR > (CONFIG_BASEADDR+CONFIG_PARTITION_SIZE) || NumToRead > CONFIG_PARTITION_SIZE/4)
	 {
        LOG(LEVEL_ERROR,"Config Read Error Addr!!!\n");
		return; //非法地址
	 }
	 
	 STMFLASH_Read(AddrOffset+CONFIG_BASEADDR,pBuffer,NumToRead);
}

//config分区擦除接口
//擦除整个分区

void Config_Erase(void)
{
	HAL_FLASH_Unlock();
    
    FLASH_EraseInitTypeDef FlashEraseInit;
    
	
    uint8_t sector_base;
  
    uint32_t SectorError=0;
   
    sector_base = STMFLASH_GetFlashSector(CONFIG_BASEADDR);
  
    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
    FlashEraseInit.Sector=sector_base;   //要擦除的扇区
    FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
    if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
    {
        LOG(LEVEL_DEBUG,"erase error :%x\n",SectorError);	
    }
	FLASH_WaitForLastOperation(FLASH_WAITETIME); 
    HAL_FLASH_Lock();  	
}



