#ifndef __WAVPLAY_H
#define __WAVPLAY_H
#include "sys.h" 
#include "ff.h"
 
#define WAV_I2S_TX_DMA_BUFSIZE    15360		//定义WAV TX DMA 数组大小(播放192Kbps@24bit的时候,至少需要设置8192大才不会卡)
 
//RIFF块
typedef struct
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"RIFF",即0X46464952
    uint32_t ChunkSize ;		   	//集合大小;文件总大小-8
    uint32_t Format;	   			//格式;WAVE,即0X45564157
}__attribute__((packed)) ChunkRIFF ;
//fmt块
typedef struct
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"fmt ",即0X20746D66
    uint32_t ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:20.
    uint16_t AudioFormat;	  	//音频格式;0X01,表示线性PCM;0X11表示IMA ADPCM
	uint16_t NumOfChannels;		//通道数量;1,表示单声道;2,表示双声道;
	uint32_t SampleRate;			//采样率;0X1F40,表示8Khz
	uint32_t ByteRate;			//字节速率; 
	uint16_t BlockAlign;			//块对齐(字节); 
	uint16_t BitsPerSample;		//单个采样数据大小;4位ADPCM,设置为4
//	uint16_t ByteExtraData;		//附加的数据字节;2个; 线性PCM,没有这个参数
}__attribute__((packed)) ChunkFMT;	   
//fact块 
typedef struct 
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"fact",即0X74636166;
    uint32_t ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:4.
    uint32_t NumOfSamples;	  	//采样的数量; 
}__attribute__((packed)) ChunkFACT;
//LIST块 
typedef struct 
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"LIST",即0X74636166;
    uint32_t ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:4. 
}__attribute__((packed)) ChunkLIST;

//data块 
typedef struct 
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"data",即0X5453494C
    uint32_t ChunkSize ;		   	//子集合大小(不包括ID和Size) 
}__attribute__((packed)) ChunkDATA;

//wav头
typedef struct
{ 
	ChunkRIFF riff;	//riff块
	ChunkFMT fmt;  	//fmt块
//	ChunkFACT fact;	//fact块 线性PCM,没有这个结构体	 
	ChunkDATA data;	//data块		 
}__attribute__((packed)) __WaveHeader; 

//wav 播放控制结构体
typedef struct
{ 
    uint16_t audioformat;			//音频格式;0X01,表示线性PCM;0X11表示IMA ADPCM
	uint16_t nchannels;				//通道数量;1,表示单声道;2,表示双声道; 
	uint16_t blockalign;				//块对齐(字节);  
	uint32_t datasize;				//WAV数据大小 

    uint32_t totsec ;				//整首歌时长,单位:秒
    uint32_t cursec ;				//当前播放时长
	
    uint32_t bitrate;	   			//比特率(位速)
	uint32_t samplerate;				//采样率 
	uint16_t bps;					//位数,比如16bit,24bit,32bit
	
	uint32_t datastart;				//数据帧开始的位置(在文件里面的偏移)
}__attribute__((packed)) __wavctrl; 


//音乐播放操作结果定义
typedef enum {
	AP_OK=0X00,				//正常播放完成
	AP_NEXT,				//播放下一曲
	AP_PREV,				//播放上一曲
	AP_ERR=0X80,			//播放有错误(没定义错误代码,仅仅表示出错) 
}APRESULT;

//音乐播放控制器
typedef struct
{  
	//2个I2S解码的BUF
	u8 *i2sbuf1;
	u8 *i2sbuf2; 
	u8 *tbuf;			//零时数组
	FIL *file;			//音频文件指针 	
	u32(*file_seek)(u32);//文件快进快退函数 

	vu8 status;			//bit0:0,暂停播放;1,继续播放
						//bit1:0,结束播放;1,开启播放  
						//bit2~3:保留
						//bit4:0,无音乐播放;1,音乐播放中 (对外标记)		
						//bit5:0,无动作;1,执行了一次切歌操作(对外标记)
						//bit6:0,无动作;1,请求终止播放(但是不删除音频播放任务),处理完成后,播放任务自动清零该位
 						//bit7:0,音频播放任务已删除/请求删除;1,音频播放任务正在运行(允许继续执行)
	
	u8 mode;			//播放模式
						//0,全部循环;1,单曲循环;2,随机播放;
	
	u8 *path;			//当前文件夹路径
	u8 *name;			//当前播放的MP3歌曲名字
	u16 namelen;		//name所占的点数.
	u16 curnamepos;		//当前的偏移

    u32 totsec ;		//整首歌时长,单位:秒
    u32 cursec ;		//当前播放时长 
    u32 bitrate;	   	//比特率(位速)
	u32 samplerate;		//采样率 
	u16 bps;			//位数,比如16bit,24bit,32bit
	
	u16 curindex;		//当前播放的音频文件索引
	u16 mfilenum;		//音乐文件数目	    
	u16 *mfindextbl;	//音频文件索引表
	
	
}__attribute__((packed)) __audiodev; 
extern __audiodev audiodev;	//音乐播放控制器

//取2个值里面的较小值.
#ifndef AUDIO_MIN			
#define AUDIO_MIN(x,y)	((x)<(y)? (x):(y))
#endif

void audio_start(void);
void audio_stop(void); 
void audio_vol_set(uint8_t vol);

uint8_t wav_decode_init(uint8_t* fname,__wavctrl* wavx);
uint32_t wav_buffill(uint8_t *buf,uint16_t size,uint8_t bits);
void wav_i2s_dma_tx_callback(void); 
uint8_t wav_play_song(uint8_t* fname);
#endif
















