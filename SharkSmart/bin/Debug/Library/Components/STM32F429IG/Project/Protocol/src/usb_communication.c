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
 * @param L2_Content_Buffer: 要发送的L2 package 
 * @param L2_Content_Len: 要发送的L2 package长度 最大65531
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
	if(L2_Content_Len <= 60)  //如果数据L2层包小于60个字节，直接发送
	{
		memcpy((uint8_t *)(l1_send_buf + 4), L2_Content_Buffer, L2_Content_Len);
		USB_HID_Send(l1_send_buf, 64);
	}
	else
	{
		memcpy((uint8_t *)(l1_send_buf + 4), L2_Content_Buffer, 60);
		USB_HID_Send(l1_send_buf, 64);		//先将带头的数据包发送过去
		
		have_send_len = 60;	//已经发送60字节个数据
		
		send_times = (uint16_t)(all_send_len - 60) / 64;	//余下数据分多少个64字节包
		send_last = (uint16_t)(all_send_len - 60) % 64;	//剩下的不够64字节的数据单独发送
		for(uint16_t i = 0; i < send_times; i ++)
		{
			USB_HID_Send((unsigned char *)L2_Content_Buffer + have_send_len, 64);
			have_send_len += 64;
		}
		if(send_last != 0)	//判断是否有不到64字节的数据
		{
			memset(l1_send_buf, 0, 64);
			memcpy(l1_send_buf, L2_Content_Buffer + have_send_len, send_last);
			USB_HID_Send(l1_send_buf, 64);
		}
	}
}

/** 
 * @param L2_Content_Buffer:存放L2 package的缓冲区 
 * @param L2_Content_Len:	接收到的L2 package的长度 最大65531
 * @param 返回值:接收包状态，成功接收到一个l2_package 返回0x01 未成功接收到 返回0x00;
 */  
uint8_t usb_recv_L1data(uint8_t * L2_Content_Buffer, uint16_t * L2_Content_Len)
{
	if(global_l1_receive_state== TRUE)	//判断是否接收到新的数据包
	{
		memcpy(L2_Content_Buffer, global_usb_recv_data, global_usb_recv_data_len); 	//将数据从USB的缓冲区复制到L2的缓冲区
		*L2_Content_Len = global_usb_recv_data_len;
		
		global_l1_receive_state = FALSE;	//清空标志，使中断中断中能够接收数据
		return 0x01;						//成功接收到数据包 返回1 
	}
	else
	{
		return 0x00;						//没有接收到数据包 返回0
	}
}

//USB HID中断调用此程序
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
				global_usb_recv_data_len = (usb_hid_data[2] << 8) | (usb_hid_data[3]);		//L2的数据总长度
				global_usb_new_head = TRUE;
				usb_all_rece_len = FALSE;
				memcpy(global_usb_recv_data + usb_all_rece_len, usb_hid_data + 4, 60);	//去掉L1的头只剩下60个字节
				usb_all_rece_len = 60;
				if(usb_all_rece_len >= global_usb_recv_data_len)						//数据小
				{//数据已接受完
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
			{//数据已接受完      
				global_usb_new_head =FALSE;
				global_l1_receive_state = TRUE;
			}
		}
    return global_l1_receive_state;
}