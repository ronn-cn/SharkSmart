#include "gt911.h"
#include "ctiic.h"
#include "log.h"
#include "delay.h" 
#include "string.h" 
#include "touch.h" 
#include "malloc.h"

#define WR_GT911_CONFIG

#ifdef WR_GT911_CONFIG
//GT911 配置表 184字节不加校验位
uint8_t GT911_Cfg[] = {
	0x69,0x20,0x03,0xE0,0x01,0x05,0x0D,0x00,0x02,0x0F,
	0x28,0x0F,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87,0x29,0x0A,
	0x2D,0x2F,0x0F,0x0A,0x00,0x00,0x00,0x02,0x02,0x1D,
	0x00,0x00,0x00,0x00,0x00,0x03,0x64,0x32,0x00,0x00,
	0x00,0x1E,0x50,0x94,0xC5,0x02,0x07,0x00,0x00,0x04,
	0xA3,0x21,0x00,0x8C,0x28,0x00,0x78,0x31,0x00,0x69,
	0x3B,0x00,0x5D,0x48,0x00,0x5D,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
	0x12,0x14,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x1D,
	0x1E,0x1F,0x20,0x21,0x22,0x24,0x26,0x28,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,									//校验码：0x80,0x01
};
#endif

//改变INT管脚为输入
void GT911_INT_Change(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	 __HAL_RCC_GPIOE_CLK_ENABLE();          
	
	GPIO_Initure.Pin=GPIO_PIN_3;            
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);	
}



//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
uint8_t GT911_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
     uint8_t i;
     uint8_t ret=0;
     CT_IIC_Start();  
     CT_IIC_Send_Byte(GT_CMD_WR);       //发送写命令      
     CT_IIC_Wait_Ack();
     CT_IIC_Send_Byte(reg>>8);       //发送高8位地址
     CT_IIC_Wait_Ack();                                                           
     CT_IIC_Send_Byte(reg&0XFF);       //发送低8位地址
     CT_IIC_Wait_Ack();  
     for(i=0;i<len;i++)
     {       
         CT_IIC_Send_Byte(buf[i]);      //发数据
         ret=CT_IIC_Wait_Ack();
         if(ret)break;  
     }
     CT_IIC_Stop();                    //产生一个停止条件        
     return ret; 
}

//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度              
void GT911_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
    uint8_t i; 
    CT_IIC_Start();    
    CT_IIC_Send_Byte(GT_CMD_WR);   //发送写命令      
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg>>8);       //发送高8位地址
    CT_IIC_Wait_Ack();                                                           
    CT_IIC_Send_Byte(reg&0XFF);       //发送低8位地址
    CT_IIC_Wait_Ack();  
    CT_IIC_Start();              
    CT_IIC_Send_Byte(GT_CMD_RD);   //发送读命令           
    CT_IIC_Wait_Ack();       
    for(i=0;i<len;i++)
    {       
        buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据      
    } 
    CT_IIC_Stop();//产生一个停止条件    
}
#ifdef WR_GT911_CONFIG
//发送配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
uint8_t GT911_Send_Cfg(uint8_t mode)
{
     uint8_t buf[2];
     uint8_t i=0;
     buf[0]=0;
     buf[1]=mode;    //是否写入到GT911 FLASH?  即是否掉电保存
     for(i=0;i<sizeof(GT911_Cfg);i++)buf[0]+=GT911_Cfg[i];//计算校验和
     buf[0]=(~buf[0])+1;
     GT911_WR_Reg(GT_CFGS_REG,(uint8_t*)GT911_Cfg,sizeof(GT911_Cfg));//发送寄存器配置
     GT911_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
     return 0;
}
#endif

uint8_t GT911_Init(void)
{    
    uint8_t tmp[4]={0};

	GPIO_InitTypeDef GPIO_Initure;
    //初始化管脚
	__HAL_RCC_GPIOC_CLK_ENABLE();          
	__HAL_RCC_GPIOB_CLK_ENABLE();  
    __HAL_RCC_GPIOE_CLK_ENABLE();  

    //RST   PE2
	GPIO_Initure.Pin=GPIO_PIN_2;            
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_3;            
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
    CT_IIC_Init();      	//初始化电容屏的I2C总线 

    GT_RST = 0;
    GT_INT = 1;
    delay_us(200);
    GT_RST = 1;
    delay_ms(6);
    GT_INT = 0;
    delay_ms(55);
    GT911_INT_Change();
    delay_ms(50);

    //读取PID
    GT911_RD_Reg(GT_PID_REG, tmp, 4);
	LOG(LEVEL_INFO,"gt911_pid:%s\n",tmp);
    if(strncmp((char * )tmp,"911",4) != 0)
    {
        LOG(LEVEL_INFO,"GT911 INIT ERR\n");
        return 1;
    }
	
    //修改配置
    tmp[0] = 0x02;
    GT911_WR_Reg(GT_CTRL_REG, tmp, 1);
    GT911_RD_Reg(GT_CFGS_REG, tmp, 1);
    //LOG(LEVEL_INFO,"tmp[0]:%x___%c\n",tmp[0],tmp[0]);
#ifdef WR_GT911_CONFIG
    if(tmp[0]<GT911_Cfg[0])
    {
        GT911_Send_Cfg(1);
    }
#endif
    delay_ms(10);
    tmp[0] = 0x00;
    GT911_WR_Reg(GT_CTRL_REG, tmp, 1);
     
    return 0;
}

//GT911触摸屏(采用查询方式)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
const uint16_t GT911_XY_TBL[5] = {GT911_XY1_REG,GT911_XY2_REG,GT911_XY3_REG,GT911_XY4_REG,GT911_XY5_REG};
uint8_t GT911_Scan(uint8_t mode)
{
    uint8_t i;
    uint8_t tmp;
    uint8_t temp;
    uint8_t res = 0;
    uint8_t data[4] = {0};
    GT911_RD_Reg(GT_GSTID_REG,&tmp,1);              //读取寄存器标志位

    if((tmp&0x80) && ((tmp&0x0f)>0))                //判断是否触发，以及触摸点数
    {
        for(i=0;i<(tmp&0x0f);i++)
        {
            GT911_RD_Reg(GT911_XY_TBL[i], data, 4);
            if(tp_dev.touchtype&0X01)//横屏
            {
                tp_dev.x[i] = data[1]<<8 | data[0];
                tp_dev.y[i] = data[3]<<8 | data[2]; 
            }else
            {
                tp_dev.x[i] = 479-(data[3]<<8 | data[2]);
                tp_dev.y[i] = data[1]<<8 | data[0]; 
            }  
        }
        //LOG(LEVEL_INFO, "%d, %d\r\n", tp_dev.x[0], tp_dev.y[0]);
        temp=0XFF<<(tmp&0X0f);//将点的个数转换为1的位数,匹配tp_dev.sta定义 
		//LOG(LEVEL_INFO, "temp %d\r\n", temp);
		tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
        res = 1;

    }
    if((tmp&0x80) == 0)
    { 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
            for(i=0;i<(tmp&0X0f);i++)        //将所有点清空
            {
                tp_dev.x[i]=0xffff;
			    tp_dev.y[i]=0xffff;
            }
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 
    tmp = 0;
    GT911_WR_Reg(GT_GSTID_REG, &tmp, 1);        //将寄存器手动清空
    return res;
    
}

