#include "sys.h"
#include "delay.h"
#include "log.h"
#include "string.h"
#include "usb_communication.h"
#include "usbd_customhid_core.h"
#include "ff.h"
#include "exfuns.h"
#include "malloc.h"	   
#include "usbd_usr.h"
#include "usb_hid_data.h"
/**
 * @param L2_Content_Buffer: Ҫ���͵�L2 package 
 * @param L2_Content_Len: Ҫ���͵�L2 package���� ���65531
 */  

void usb_send_L1data(uint8_t * L2_Content_Buffer, uint16_t L2_Content_Len)
{
	int all_send_len = L2_Content_Len;
	int have_send_len = 0;
	
	uint16_t send_times;
	uint16_t send_last;
	uint8_t l1_send_buf[64] = {0};
	
	l1_send_buf[0] = 0xAB;
	l1_send_buf[1] = 0xBA;
	l1_send_buf[2] = (uint8_t)((L2_Content_Len & 0xFF00) >> 8);
	l1_send_buf[3] = (uint8_t)((L2_Content_Len & 0x00FF));
	if(L2_Content_Len <= 60)  //�������L2���С��60���ֽڣ�ֱ�ӷ���
	{
		memcpy((uint8_t *)(l1_send_buf + 4), L2_Content_Buffer, L2_Content_Len);
		USB_HID_Send(l1_send_buf, 64);
	}
	else
	{
		memcpy((uint8_t *)(l1_send_buf + 4), L2_Content_Buffer, 60);
		USB_HID_Send(l1_send_buf, 64);		//�Ƚ���ͷ�����ݰ����͹�ȥ
		
		have_send_len = 60;	//�Ѿ�����60�ֽڸ�����
		
		send_times = (uint16_t)(all_send_len - 60) / 64;	//�������ݷֶ��ٸ�64�ֽڰ�
		send_last = (uint16_t)(all_send_len - 60) % 64;	//ʣ�µĲ���64�ֽڵ����ݵ�������
		for(uint16_t i = 0; i < send_times; i ++)
		{
			USB_HID_Send((unsigned char *)L2_Content_Buffer + have_send_len, 64);
			have_send_len += 64;
		}
		if(send_last != 0)	//�ж��Ƿ��в���64�ֽڵ�����
		{
			memset(l1_send_buf, 0, 64);
			memcpy(l1_send_buf, L2_Content_Buffer + have_send_len, send_last);
			USB_HID_Send(l1_send_buf, 64);
		}
	}
}

/** 
 * @param L2_Content_Buffer:���L2 package�Ļ����� 
 * @param L2_Content_Len:	���յ���L2 package�ĳ��� ���65531
 * @param ����ֵ:���հ�״̬���ɹ����յ�һ��l2_package ����0x01 δ�ɹ����յ� ����0x00;
 */  
uint8_t usb_recv_L1data(uint8_t * L2_Content_Buffer, uint16_t * L2_Content_Len)
{
	if(global_l1_receive_state== TRUE)	//�ж��Ƿ���յ��µ����ݰ�
	{
		memcpy(L2_Content_Buffer, global_usb_recv_data, global_usb_recv_data_len); 	//�����ݴ�USB�Ļ��������Ƶ�L2�Ļ�����
		*L2_Content_Len = global_usb_recv_data_len;
		
		global_l1_receive_state = FALSE;	//��ձ�־��ʹ�ж��ж����ܹ���������
		return 0x01;						//�ɹ����յ����ݰ� ����1 
	}
	else
	{
		return 0x00;						//û�н��յ����ݰ� ����0
	}
}

//USB HID�жϵ��ô˳���
uint32_t usb_all_rece_len = 0;
uint8_t usb_recv_L0data(uint8_t *usb_hid_data, uint8_t len)
{
	static uint32_t last_time = 0;
	static uint32_t now_time = 0;

	now_time = HAL_GetTick();
	if(now_time - last_time > 1000)
	{
		global_usb_new_head = 0; 
		global_l1_receive_state = 0;
	}
	last_time = now_time;


        if(global_usb_new_head == 0 && global_l1_receive_state == 0)
		{
			if(usb_hid_data[0] == 0xAB && usb_hid_data[1] == 0xBA)
			{
				global_usb_recv_data_len = (usb_hid_data[2] << 8) | (usb_hid_data[3]);		//L2�������ܳ���
				global_usb_new_head = TRUE;
				usb_all_rece_len = FALSE;
				memcpy(global_usb_recv_data + usb_all_rece_len, usb_hid_data + 4, 60);	//ȥ��L1��ͷֻʣ��60���ֽ�
				usb_all_rece_len = 60;
				if(usb_all_rece_len >= global_usb_recv_data_len)						//����С
				{//�����ѽ�����
					global_usb_new_head =FALSE;
					global_l1_receive_state = TRUE;
				}
			}
		}
		else if(global_l1_receive_state == FALSE)
		{
			memcpy(global_usb_recv_data + usb_all_rece_len, usb_hid_data, 64);
			usb_all_rece_len += 64;
			if(usb_all_rece_len >= global_usb_recv_data_len)
			{//�����ѽ�����      
				global_usb_new_head =FALSE;
				global_l1_receive_state = TRUE;
			}
		}
    return global_l1_receive_state;
}