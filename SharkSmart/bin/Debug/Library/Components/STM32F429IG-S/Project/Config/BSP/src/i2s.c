#include "i2s.h"
#include "delay.h"

I2S_HandleTypeDef I2S3_Handler;			//I2S3句柄

DMA_HandleTypeDef I2S3_TXDMA_Handler;   //I2S3发送DMA句柄
//I2S3以及I2SExt同时初始化！！当I2S模式为I2S_MODE_MASTER_TX的时候I2SExt的模式自动设置为I2S_MODE_SLAVE_RX
//I2S_Standard:I2S标准,可以设置：I2S_STANDARD_PHILIPS/I2S_STANDARD_MSB/
//				       I2S_STANDARD_LSB/I2S_STANDARD_PCM_SHORT/I2S_STANDARD_PCM_LONG
//I2S_Mode:I2S工作模式,可以设置：I2S_MODE_SLAVE_TX/I2S_MODE_SLAVE_RX/I2S_MODE_MASTER_TX/I2S_MODE_MASTER_RX
//I2S_Clock_Polarity:时钟电平，可以设置为:I2S_CPOL_LOW/I2S_CPOL_HIGH
//I2S_DataFormat:数据长度,可以设置：I2S_DATAFORMAT_16B/I2S_DATAFORMAT_16B_EXTENDED/I2S_DATAFORMAT_24B/I2S_DATAFORMAT_32B
void I2S3_Init(uint32_t I2S_Standard,uint32_t I2S_Mode,uint32_t I2S_Clock_Polarity,uint32_t I2S_DataFormat)
{

  I2S3_Handler.Instance = SPI3;
  I2S3_Handler.Init.Mode = I2S_Mode;
  I2S3_Handler.Init.Standard = I2S_Standard;
  I2S3_Handler.Init.DataFormat = I2S_DataFormat;
  I2S3_Handler.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  I2S3_Handler.Init.AudioFreq = I2S_AUDIOFREQ_DEFAULT;
  I2S3_Handler.Init.CPOL = I2S_Clock_Polarity;
  I2S3_Handler.Init.ClockSource = I2S_CLOCK_PLL;
  HAL_I2S_Init(&I2S3_Handler);

	SPI3->CR2|=1<<1;									//SPI2 TX DMA请求使能.
	__HAL_I2S_ENABLE(&I2S3_Handler);					//使能I2S2	
}


void HAL_I2S_MspInit(I2S_HandleTypeDef* i2sHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2sHandle->Instance==SPI3)
  {
    __HAL_RCC_SPI3_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2S3 GPIO Configuration    
    PC7     ------> I2S3_MCK
    PA4     ------> I2S3_WS
    PB4     ------> I2S3_ext_SD
    PC10     ------> I2S3_CK
    PB5     ------> I2S3_SD 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_I2S3ext;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

//采样率计算公式:Fs=I2SxCLK/[256*(2*I2SDIV+ODD)]
//I2SxCLK=(HSE/pllm)*PLLI2SN/PLLI2SR
//一般HSE=8Mhz 
//pllm:在Sys_Clock_Set设置的时候确定，一般是8
//PLLI2SN:一般是192~432 
//PLLI2SR:2~7
//I2SDIV:2~255
//ODD:0/1
//I2S分频系数表@pllm=8,HSE=8Mhz,即vco输入频率为1Mhz
//表格式:采样率/10,PLLI2SN,PLLI2SR,I2SDIV,ODD
const uint16_t I2S_PSC_TBL[][5]=
{
	{800 ,256,5,12,1},		//8Khz采样率
	{1102,429,4,19,0},		//11.025Khz采样率 
	{1600,213,2,13,0},		//16Khz采样率
	{2205,429,4, 9,1},		//22.05Khz采样率
	{3200,213,2, 6,1},		//32Khz采样率
	{4410,271,2, 6,0},		//44.1Khz采样率
	{4800,258,3, 3,1},		//48Khz采样率
	{8820,316,2, 3,1},		//88.2Khz采样率
	{9600,344,2, 3,1},  	//96Khz采样率
	{17640,361,2,2,0},  	//176.4Khz采样率 
	{19200,393,2,2,0},  	//192Khz采样率
};

//开启I2S的DMA功能,HAL库没有提供此函数
//因此我们需要自己操作寄存器编写一个
void I2S_DMA_Enable(void)
{
    uint32_t tempreg=0;
    tempreg=SPI3->CR2;    	//先读出以前的设置			
	tempreg|=1<<1;			//使能DMA
	SPI3->CR2=tempreg;		//写入CR1寄存器中
}

//设置SAIA的采样率(@MCKEN)
//samplerate:采样率,单位:Hz
//返回值:0,设置成功;1,无法设置.
uint8_t I2S3_SampleRate_Set(uint32_t samplerate)
{   
    uint8_t i=0; 
	uint32_t tempreg=0;
    RCC_PeriphCLKInitTypeDef RCCI2S3_ClkInitSture;  
	
	for(i=0;i<(sizeof(I2S_PSC_TBL)/10);i++)//看看改采样率是否可以支持
	{
		if((samplerate/10)==I2S_PSC_TBL[i][0])break;
	}
    if(i==(sizeof(I2S_PSC_TBL)/10))return 1;//搜遍了也找不到
	
    RCCI2S3_ClkInitSture.PeriphClockSelection=RCC_PERIPHCLK_I2S;	//外设时钟源选择 
    RCCI2S3_ClkInitSture.PLLI2S.PLLI2SN=(uint32_t)I2S_PSC_TBL[i][1];    	//设置PLLI2SN
    RCCI2S3_ClkInitSture.PLLI2S.PLLI2SR=(uint32_t)I2S_PSC_TBL[i][2];    	//设置PLLI2SR
    HAL_RCCEx_PeriphCLKConfig(&RCCI2S3_ClkInitSture);             	//设置时钟
	
	RCC->CR|=1<<26;					//开启I2S时钟
	while((RCC->CR&1<<27)==0);		//等待I2S时钟开启成功. 
	tempreg=I2S_PSC_TBL[i][3]<<0;	//设置I2SDIV
	tempreg|=I2S_PSC_TBL[i][4]<<8;	//设置ODD位
	tempreg|=1<<9;					//使能MCKOE位,输出MCK
	SPI3->I2SPR=tempreg;			//设置I2SPR寄存器 
	return 0;
}  

//I2S3 TX DMA配置
//设置为双缓冲模式,并开启DMA传输完成中断
//buf0:M0AR地址.
//buf1:M1AR地址.
//num:每次传输数据量
void I2S3_TX_DMA_Init(uint8_t* buf0,uint8_t *buf1,uint16_t num)
{  
    __HAL_RCC_DMA1_CLK_ENABLE();                                    		//使能DMA1时钟
    __HAL_LINKDMA(&I2S3_Handler,hdmatx,I2S3_TXDMA_Handler);         		//将DMA与I2S联系起来
	
    I2S3_TXDMA_Handler.Instance=DMA1_Stream5;                       		//DMA1数据流4                     
    I2S3_TXDMA_Handler.Init.Channel=DMA_CHANNEL_0;                  		//通道0
    I2S3_TXDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;         		//存储器到外设模式
    I2S3_TXDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;             		//外设非增量模式
    I2S3_TXDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                 		//存储器增量模式
    I2S3_TXDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;   	//外设数据长度:16位
    I2S3_TXDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;    	//存储器数据长度:16位
    I2S3_TXDMA_Handler.Init.Mode=DMA_CIRCULAR;                      		//使用循环模式 
    I2S3_TXDMA_Handler.Init.Priority=DMA_PRIORITY_HIGH;             		//高优先级
    I2S3_TXDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;          		//不使用FIFO
    I2S3_TXDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;             		//存储器单次突发传输
    I2S3_TXDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;          		//外设突发单次传输 
    HAL_DMA_DeInit(&I2S3_TXDMA_Handler);                            		//先清除以前的设置
    HAL_DMA_Init(&I2S3_TXDMA_Handler);	                            		//初始化DMA

    HAL_DMAEx_MultiBufferStart(&I2S3_TXDMA_Handler,(uint32_t)buf0,(uint32_t)&SPI3->DR,(uint32_t)buf1,num);//开启双缓冲
    __HAL_DMA_DISABLE(&I2S3_TXDMA_Handler);                         		//先关闭DMA 
    delay_us(10);                                                   		//10us延时，防止-O2优化出问题 	
    __HAL_DMA_ENABLE_IT(&I2S3_TXDMA_Handler,DMA_IT_TC);             		//开启传输完成中断
    __HAL_DMA_CLEAR_FLAG(&I2S3_TXDMA_Handler,DMA_FLAG_TCIF1_5);     		//清除DMA传输完成中断标志位
	
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn,0,0);                    		//DMA中断优先级
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

//I2S DMA回调函数指针
void (*i2s_tx_callback)(void);	//TX回调函数 

//DMA1_Stream4中断服务函数
void DMA1_Stream5_IRQHandler(void)
{  
    if(__HAL_DMA_GET_FLAG(&I2S3_TXDMA_Handler,DMA_FLAG_TCIF1_5)!=RESET) //DMA传输完成
    {
        __HAL_DMA_CLEAR_FLAG(&I2S3_TXDMA_Handler,DMA_FLAG_TCIF1_5);     //清除DMA传输完成中断标志位
		if(i2s_tx_callback!=NULL) i2s_tx_callback();	//执行回调函数,读取数据等操作在这里面处理  
    } 
} 
//I2S开始播放
void I2S_Play_Start(void)
{   	
	__HAL_DMA_ENABLE(&I2S3_TXDMA_Handler);//开启DMA TX传输  			
}

//关闭I2S播放
void I2S_Play_Stop(void)
{ 
	__HAL_DMA_DISABLE(&I2S3_TXDMA_Handler);//结束播放;		 	 
} 