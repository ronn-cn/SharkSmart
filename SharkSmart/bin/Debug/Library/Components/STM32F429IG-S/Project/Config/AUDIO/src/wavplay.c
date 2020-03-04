#include "wavplay.h" 
#include "usart.h" 
#include "delay.h" 
#include "malloc.h"
#include "ff.h"
#include "i2s.h"
#include "wm8978.h"
#include "log.h"
#include "rtos.h"


__audiodev audiodev;	 
 

static void wav_delay_ms(uint32_t nms)
{
    if (OS_Running == FALSE)
    {
        uint32_t tickstart = 0;
        tickstart = HAL_GetTick();
        while ((HAL_GetTick() - tickstart) < nms)
        {
        }
    }
    else
    {
        OSTimeDly(nms);
    }
    
}


////////////////////////////////////////////////////////////////////////////////////
//开始音频播放
void audio_start(void)
{
	audiodev.status|=1<<1;		//开启播放
	audiodev.status|=1<<0;		//非暂停状态
	I2S_Play_Start();
} 
//停止音频播放
void audio_stop(void)
{
	audiodev.status&=~(1<<0);	//暂停位清零
	audiodev.status&=~(1<<1);	//结束播放
	I2S_Play_Stop();
}    
//设置音量0-99
void audio_vol_set(uint8_t vol)
{
	WM8978_SPKvol_Set((uint8_t )vol/1.57);
}


__wavctrl wavctrl;		//WAV控制结构体
__IO uint8_t wavtransferend=0;	//i2s传输完成标志
__IO uint8_t wavwitchbuf=0;		//i2sbufx指示标志

//fname:文件路径+文件名
//wavx:wav 信息存放结构体指针
//返回值:0,成功;1,打开文件失败;2,非WAV文件;3,DATA区域未找到.
uint8_t wav_decode_init(uint8_t* fname,__wavctrl* wavx)
{
	FIL*ftemp;
	uint8_t *buf; 
	UINT br=0;
	uint8_t res=0;
	
	ChunkRIFF *riff;
	ChunkFMT *fmt;
	ChunkFACT *fact;
	ChunkDATA *data;
	ftemp=(FIL*)mymalloc(SRAMEX,sizeof(FIL));
	buf=mymalloc(SRAMEX,512);
	if(ftemp&&buf)	//内存申请成功
	{
		res=f_open(ftemp,(TCHAR*)fname,FA_READ);//打开文件
		if(res==FR_OK)
		{
			f_read(ftemp,buf,512,&br);	//读取512字节在数据
			riff=(ChunkRIFF *)buf;		//获取RIFF块
			if(riff->Format==0X45564157)//是WAV文件
			{
				fmt=(ChunkFMT *)(buf+12);	//获取FMT块 
				fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//读取FACT块
				if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)wavx->datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//具有fact/LIST块的时候(未测试)
				else wavx->datastart=12+8+fmt->ChunkSize;  
				data=(ChunkDATA *)(buf+wavx->datastart);	//读取DATA块
				if(data->ChunkID==0X61746164)//解析成功!
				{
					wavx->audioformat=fmt->AudioFormat;		//音频格式
					wavx->nchannels=fmt->NumOfChannels;		//通道数
					wavx->samplerate=fmt->SampleRate;		//采样率
					wavx->bitrate=fmt->ByteRate*8;			//得到位速
					wavx->blockalign=fmt->BlockAlign;		//块对齐
					wavx->bps=fmt->BitsPerSample;			//位数,16/24/32位
					
					wavx->datasize=data->ChunkSize;			//数据块大小
					wavx->datastart=wavx->datastart+8;		//数据流开始的地方. 
					 
					LOG(LEVEL_INFO, "wavx->audioformat:%d\r\n",wavx->audioformat);
					LOG(LEVEL_INFO, "wavx->nchannels:%d\r\n",wavx->nchannels);
					LOG(LEVEL_INFO, "wavx->samplerate:%d\r\n",wavx->samplerate);
					LOG(LEVEL_INFO, "wavx->bitrate:%d\r\n",wavx->bitrate);
					LOG(LEVEL_INFO, "wavx->blockalign:%d\r\n",wavx->blockalign);
					LOG(LEVEL_INFO, "wavx->bps:%d\r\n",wavx->bps);
					LOG(LEVEL_INFO, "wavx->datasize:%d\r\n",wavx->datasize);
					LOG(LEVEL_INFO, "wavx->datastart:%d\r\n",wavx->datastart);  
				}else res=3;//data区域未找到.
			}else res=2;//非wav文件
			
		}else res=1;//打开文件错误
	}
	f_close(ftemp);
	myfree(SRAMEX,ftemp);//释放内存
	myfree(SRAMEX,buf); 
	return 0;
}

//填充buf
//buf:数据区
//size:填充数据量
//bits:位数(16/24)
//返回值:读到的数据个数
uint32_t wav_buffill(uint8_t *buf,uint16_t size,uint8_t bits)
{
	uint16_t readlen=0;
	uint32_t bread;
	uint16_t i;
	uint8_t *p;
	if(bits==24)//24bit音频,需要处理一下
	{
		readlen=(size/4)*3;							//此次要读取的字节数
		f_read(audiodev.file,audiodev.tbuf,readlen,(UINT*)&bread);	//读取数据
		p=audiodev.tbuf;
		for(i=0;i<size;)
		{
			buf[i++]=p[1];
			buf[i]=p[2]; 
			i+=2;
			buf[i++]=p[0];
			p+=3;
		} 
		bread=(bread*4)/3;		//填充后的大小.
	}else 
	{
		f_read(audiodev.file,buf,size,(UINT*)&bread);//16bit音频,直接读取数据  
		if(bread<size)//不够数据了,补充0
		{
			for(i=bread;i<size-bread;i++)buf[i]=0; 
		}
	}
	return bread;
}  
//WAV播放时,I2S DMA传输回调函数
void wav_i2s_dma_tx_callback(void) 
{   
	uint16_t i;
	if(DMA1_Stream5->CR&(1<<19))
	{
		wavwitchbuf=0;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE;i++)//暂停
			{
				audiodev.i2sbuf1[i]=0;//填充0
			}
		}
	}else 
	{
		wavwitchbuf=1;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE;i++)//暂停
			{
				audiodev.i2sbuf2[i]=0;//填充0
			}
		}
	}
	wavtransferend=1;
} 
//得到当前播放时间
//fx:文件指针
//wavx:wav播放控制器
void wav_get_curtime(FIL*fx,__wavctrl *wavx)
{
	long long fpos;  	
 	wavx->totsec=wavx->datasize/(wavx->bitrate/8);	//歌曲总长度(单位:秒) 
	fpos=fx->fptr-wavx->datastart; 					//得到当前文件播放到的地方 
	wavx->cursec=fpos*wavx->totsec/wavx->datasize;	//当前播放到第多少秒了?	
}
//wav文件快进快退函数
//pos:需要定位到的文件位置
//返回值:当前文件位置(即定位后的结果)
uint32_t wav_file_seek(uint32_t pos)
{
	uint8_t temp;
	if(pos>audiodev.file->obj.objsize)
	{
		pos=audiodev.file->obj.objsize;
	}
	if(pos<wavctrl.datastart)pos=wavctrl.datastart;
	if(wavctrl.bps==16)temp=8;	//必须是8的倍数
	if(wavctrl.bps==24)temp=12;	//必须是12的倍数 
	if((pos-wavctrl.datastart)%temp)
	{
		pos+=temp-(pos-wavctrl.datastart)%temp;
	}
	f_lseek(audiodev.file,pos);
	return audiodev.file->fptr;
}
//播放某个WAV文件
//fname:wav文件路径.
//返回值:0,正常播放完成
//[b7]:0,正常状态;1,错误状态
//[b6:0]:b7=0时,表示操作码
//       b7=1时,表示有错误(这里不判定具体错误,0X80~0XFF,都算是错误)
uint8_t wav_play_song(uint8_t* fname)
{
	WM8978_SPKvol_Set(50);			//喇叭音量设置
	WM8978_ADDA_Cfg(1,0);	//开启DAC
	WM8978_Input_Cfg(0,0,0);//关闭输入通道
	WM8978_Output_Cfg(1,0);	//开启DAC输出   
	uint8_t res;  
	uint32_t fillnum; 
	audiodev.file=(FIL*)mymalloc(SRAMEX,sizeof(FIL));
	audiodev.i2sbuf1=mymalloc(SRAMEX,WAV_I2S_TX_DMA_BUFSIZE);
	audiodev.i2sbuf2=mymalloc(SRAMEX,WAV_I2S_TX_DMA_BUFSIZE);
	audiodev.tbuf=mymalloc(SRAMEX,WAV_I2S_TX_DMA_BUFSIZE);
	//audiodev.file_seek=wav_file_seek;
	if(audiodev.file&&audiodev.i2sbuf1&&audiodev.i2sbuf2&&audiodev.tbuf)
	{ 
		res=wav_decode_init(fname,&wavctrl);//得到文件的信息
		if(res==0)//解析文件成功
		{
			if(wavctrl.bps==16)
			{
				WM8978_I2S_Cfg(2,0);	//飞利浦标准,16位数据长度
				I2S3_Init(I2S_STANDARD_PHILIPS,I2S_MODE_MASTER_TX,I2S_CPOL_LOW,I2S_DATAFORMAT_16B_EXTENDED);	//飞利浦标准,主机发送,时钟低电平有效,16位扩展帧长度
			}else if(wavctrl.bps==24)
			{
				WM8978_I2S_Cfg(2,2);	//飞利浦标准,24位数据长度
				I2S3_Init(I2S_STANDARD_PHILIPS,I2S_MODE_MASTER_TX,I2S_CPOL_LOW,I2S_DATAFORMAT_24B);	//飞利浦标准,主机发送,时钟低电平有效,24位长度
			}
			I2S3_SampleRate_Set(wavctrl.samplerate);//设置采样率
			I2S3_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE/2); //配置TX DMA
			i2s_tx_callback=wav_i2s_dma_tx_callback;			//回调函数指wav_i2s_dma_callback
			audio_stop();
			wavtransferend = 0;
			res=f_open(audiodev.file,(TCHAR*)fname,FA_READ);	//打开文件
			if(res==0)
			{
				f_lseek(audiodev.file, wavctrl.datastart);		//跳过文件头
				fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);
				fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);
				audio_start();  
				while(res==0)
				{ 
					while(wavtransferend==0)//等待wav传输完成; 
					{
						wav_delay_ms(5);
					}
					wavtransferend=0;
					if(fillnum!=WAV_I2S_TX_DMA_BUFSIZE)//播放结束?
					{
						//res=KEY0_PRES;
						break;
					} 
 					if(wavwitchbuf)fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);//填充buf2
					else fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);//填充buf1
					while(1)
					{ 
						wav_get_curtime(audiodev.file,&wavctrl);//得到总时间和当前播放的时间 
						if((audiodev.status&0X01)==0)wav_delay_ms(10);
						else break;
					}
				}
				audio_stop(); 
			}else res=AP_ERR; 
		}else res=AP_ERR;
	}else res=AP_ERR; 
	myfree(SRAMEX,audiodev.tbuf);	//释放内存
	myfree(SRAMEX,audiodev.i2sbuf1);//释放内存
	myfree(SRAMEX,audiodev.i2sbuf2);//释放内存 
	myfree(SRAMEX,audiodev.file);	//释放内存 

	WM8978_ADDA_Cfg(0,0);
	WM8978_Output_Cfg(0,0);
	WM8978_SPKvol_Set(0);			//喇叭音量设置
	return res;
} 
	







