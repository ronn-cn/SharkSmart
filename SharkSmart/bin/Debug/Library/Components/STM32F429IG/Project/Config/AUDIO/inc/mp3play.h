#ifndef __MP3PLAY_H__ 
#define __MP3PLAY_H__ 
#include <sys.h>
#include <mp3dec.h>
#define MP3_TITSIZE_MAX		40		//����������󳤶�
#define MP3_ARTSIZE_MAX		40		//����������󳤶�
#define MP3_FILE_BUF_SZ    5*1024	//MP3����ʱ,�ļ�buf��С
 
//ID3V1 ��ǩ 
typedef struct 
{
    uint8_t id[3];		   	//ID,TAG������ĸ
    uint8_t title[30];		//��������
    uint8_t artist[30];		//����������
	uint8_t year[4];		//���
	uint8_t comment[30];	//��ע
	uint8_t genre;			//���� 
}__attribute__ ((__packed__)) ID3V1_Tag;

//ID3V2 ��ǩͷ 
typedef struct 
{
    uint8_t id[3];		   	//ID
    uint8_t mversion;		//���汾��
    uint8_t sversion;		//�Ӱ汾��
    uint8_t flags;			//��ǩͷ��־
    uint8_t size[4];		//��ǩ��Ϣ��С(��������ǩͷ10�ֽ�).����,��ǩ��С=size+10.
}__attribute__ ((__packed__)) ID3V2_TagHead;

//ID3V2.3 �汾֡ͷ
typedef struct 
{
    uint8_t id[4];		   	//֡ID
    uint8_t size[4];		//֡��С
    uint16_t flags;			//֡��־
}__attribute__ ((__packed__)) ID3V23_FrameHead;

//MP3 Xing֡��Ϣ(û��ȫ���г���,���г����õĲ���)
typedef struct 
{
    uint8_t id[4];		   	//֡ID,ΪXing/Info
    uint8_t flags[4];		//��ű�־
    uint8_t frames[4];		//��֡��
	uint8_t fsize[4];		//�ļ��ܴ�С(������ID3)
}__attribute__ ((__packed__)) MP3_FrameXing;
 
//MP3 VBRI֡��Ϣ(û��ȫ���г���,���г����õĲ���)
typedef struct 
{
    uint8_t id[4];		   	//֡ID,ΪXing/Info
	uint8_t version[2];		//�汾��
	uint8_t delay[2];		//�ӳ�
	uint8_t quality[2];		//��Ƶ����,0~100,Խ������Խ��
	uint8_t fsize[4];		//�ļ��ܴ�С
	uint8_t frames[4];		//�ļ���֡�� 
}__attribute__ ((__packed__)) MP3_FrameVBRI;


//MP3���ƽṹ��
typedef struct 
{
    uint8_t title[MP3_TITSIZE_MAX]; //��������
    uint8_t artist[MP3_ARTSIZE_MAX];//����������
    uint32_t totsec ;				//���׸�ʱ��,��λ:��
    uint32_t cursec ;				//��ǰ����ʱ��
	
    uint32_t bitrate;	   			//������
	uint32_t samplerate;			//������
	uint16_t outsamples;			//PCM�����������С(��16λΪ��λ),������MP3,�����ʵ�����*2(����DAC���)
	
	uint32_t datastart;				//����֡��ʼ��λ��(���ļ������ƫ��)
}__attribute__ ((__packed__)) __mp3ctrl ;

extern __mp3ctrl * mp3ctrl;



void mp3_i2s_dma_tx_callback(void) ;
void mp3_fill_buffer(uint16_t* buf,uint16_t size,uint8_t nch);
uint8_t mp3_id3v1_decode(uint8_t* buf,__mp3ctrl *pctrl);
uint8_t mp3_id3v2_decode(uint8_t* buf,uint32_t size,__mp3ctrl *pctrl);
uint8_t mp3_get_info(uint8_t *pname,__mp3ctrl* pctrl);
uint8_t mp3_play_song(uint8_t* fname);
#endif
