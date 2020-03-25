#include "usb_bsp.h"
#include "sys.h"  
#include "delay.h" 
#include "log.h" 
  
//USB OTG �ײ�IO��ʼ��
//pdev:USB OTG�ں˽ṹ��ָ��
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
     GPIO_InitTypeDef  GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();                   //ʹ��GPIOAʱ��
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();              //ʹ��OTG FSʱ��

    //����PA11 12
    GPIO_InitStruct.Pin=GPIO_PIN_11|GPIO_PIN_12;    //PA11 12
    GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;           //����
    GPIO_InitStruct.Pull=GPIO_NOPULL;               //��������
    GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;          //����
    GPIO_InitStruct.Alternate=GPIO_AF10_OTG_FS;     //����ΪOTG FS
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);         //��ʼ��
}

//USB OTG �ж�����,����USB FS�ж�
//pdev:USB OTG�ں˽ṹ��ָ��
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{  	
    HAL_NVIC_SetPriority(OTG_FS_IRQn,0,3);          //��ռ���ȼ�0�������ȼ�3
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);                //ʹ��OTG USB FS�ж� 
}

//USB OTG �ж�����,����USB FS�ж�
//pdev:USB OTG�ں˽ṹ��ָ��
void USB_OTG_BSP_DisableInterrupt(void)
{ 
}
//USB OTG �˿ڹ�������(������δ�õ�)
//pdev:USB OTG�ں˽ṹ��ָ��
//state:0,�ϵ�;1,�ϵ�
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{ 
}
//USB_OTG �˿ڹ���IO����(������δ�õ�)
//pdev:USB OTG�ں˽ṹ��ָ��
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{ 
} 
void usb_delay_ms(uint32_t nms);
void usb_delay_us(uint32_t nus);
//USB_OTG us����ʱ����
//�����̲���SYSTEM�ļ��е�delay.c�����delay_us����ʵ��
//�ٷ����̲��õ��Ƕ�ʱ��2��ʵ�ֵ�.
//usec:Ҫ��ʱ��us��.
void USB_OTG_BSP_uDelay (const uint32_t usec)
{ 
   	usb_delay_us(usec);
}
//USB_OTG ms����ʱ����
//�����̲���SYSTEM�ļ��е�delay.c�����delay_ms����ʵ��
//�ٷ����̲��õ��Ƕ�ʱ��2��ʵ�ֵ�.
//msec:Ҫ��ʱ��ms��.
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
	usb_delay_ms(msec);
}

void usb_delay_us(uint32_t nus)
{
    uint8_t temp = 192;
    for(; nus > 0; nus--)
    {
        for(; temp > 0; temp--)
        {
            __NOP();
        }
    }

}
void usb_delay_ms(uint32_t nms)
{
    uint32_t nus = 1000 * nms;
    usb_delay_us(nus);

}



















