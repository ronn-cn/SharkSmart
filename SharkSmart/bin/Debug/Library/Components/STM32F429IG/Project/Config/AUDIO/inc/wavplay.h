#ifndef __WAVPLAY_H
#define __WAVPLAY_H
#include "sys.h" 
#include "ff.h"
 
#define WAV_I2S_TX_DMA_BUFSIZE    15360		//����WAV TX DMA �����С(����192Kbps@24bit��ʱ��,������Ҫ����8192��Ų��Ῠ)
 
//RIFF��
typedef struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"RIFF",��0X46464952
    uint32_t ChunkSize ;		   	//���ϴ�С;�ļ��ܴ�С-8
    uint32_t Format;	   			//��ʽ;WAVE,��0X45564157
}__attribute__((packed)) ChunkRIFF ;
//fmt��
typedef struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"fmt ",��0X20746D66
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:20.
    uint16_t AudioFormat;	  	//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
	uint16_t NumOfChannels;		//ͨ������;1,��ʾ������;2,��ʾ˫����;
	uint32_t SampleRate;			//������;0X1F40,��ʾ8Khz
	uint32_t ByteRate;			//�ֽ�����; 
	uint16_t BlockAlign;			//�����(�ֽ�); 
	uint16_t BitsPerSample;		//�����������ݴ�С;4λADPCM,����Ϊ4
//	uint16_t ByteExtraData;		//���ӵ������ֽ�;2��; ����PCM,û���������
}__attribute__((packed)) ChunkFMT;	   
//fact�� 
typedef struct 
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"fact",��0X74636166;
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4.
    uint32_t NumOfSamples;	  	//����������; 
}__attribute__((packed)) ChunkFACT;
//LIST�� 
typedef struct 
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"LIST",��0X74636166;
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4. 
}__attribute__((packed)) ChunkLIST;

//data�� 
typedef struct 
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"data",��0X5453494C
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size) 
}__attribute__((packed)) ChunkDATA;

//wavͷ
typedef struct
{ 
	ChunkRIFF riff;	//riff��
	ChunkFMT fmt;  	//fmt��
//	ChunkFACT fact;	//fact�� ����PCM,û������ṹ��	 
	ChunkDATA data;	//data��		 
}__attribute__((packed)) __WaveHeader; 

//wav ���ſ��ƽṹ��
typedef struct
{ 
    uint16_t audioformat;			//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
	uint16_t nchannels;				//ͨ������;1,��ʾ������;2,��ʾ˫����; 
	uint16_t blockalign;				//�����(�ֽ�);  
	uint32_t datasize;				//WAV���ݴ�С 

    uint32_t totsec ;				//���׸�ʱ��,��λ:��
    uint32_t cursec ;				//��ǰ����ʱ��
	
    uint32_t bitrate;	   			//������(λ��)
	uint32_t samplerate;				//������ 
	uint16_t bps;					//λ��,����16bit,24bit,32bit
	
	uint32_t datastart;				//����֡��ʼ��λ��(���ļ������ƫ��)
}__attribute__((packed)) __wavctrl; 


//���ֲ��Ų����������
typedef enum {
	AP_OK=0X00,				//�����������
	AP_NEXT,				//������һ��
	AP_PREV,				//������һ��
	AP_ERR=0X80,			//�����д���(û����������,������ʾ����) 
}APRESULT;

//���ֲ��ſ�����
typedef struct
{  
	//2��I2S�����BUF
	u8 *i2sbuf1;
	u8 *i2sbuf2; 
	u8 *tbuf;			//��ʱ����
	FIL *file;			//��Ƶ�ļ�ָ�� 	
	u32(*file_seek)(u32);//�ļ�������˺��� 

	vu8 status;			//bit0:0,��ͣ����;1,��������
						//bit1:0,��������;1,��������  
						//bit2~3:����
						//bit4:0,�����ֲ���;1,���ֲ����� (������)		
						//bit5:0,�޶���;1,ִ����һ���и����(������)
						//bit6:0,�޶���;1,������ֹ����(���ǲ�ɾ����Ƶ��������),������ɺ�,���������Զ������λ
 						//bit7:0,��Ƶ����������ɾ��/����ɾ��;1,��Ƶ����������������(�������ִ��)
	
	u8 mode;			//����ģʽ
						//0,ȫ��ѭ��;1,����ѭ��;2,�������;
	
	u8 *path;			//��ǰ�ļ���·��
	u8 *name;			//��ǰ���ŵ�MP3��������
	u16 namelen;		//name��ռ�ĵ���.
	u16 curnamepos;		//��ǰ��ƫ��

    u32 totsec ;		//���׸�ʱ��,��λ:��
    u32 cursec ;		//��ǰ����ʱ�� 
    u32 bitrate;	   	//������(λ��)
	u32 samplerate;		//������ 
	u16 bps;			//λ��,����16bit,24bit,32bit
	
	u16 curindex;		//��ǰ���ŵ���Ƶ�ļ�����
	u16 mfilenum;		//�����ļ���Ŀ	    
	u16 *mfindextbl;	//��Ƶ�ļ�������
	
	
}__attribute__((packed)) __audiodev; 
extern __audiodev audiodev;	//���ֲ��ſ�����

//ȡ2��ֵ����Ľ�Сֵ.
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
















