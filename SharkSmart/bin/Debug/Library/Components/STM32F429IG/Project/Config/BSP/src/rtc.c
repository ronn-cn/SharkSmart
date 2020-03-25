#include "rtc.h"
#include "delay.h"

RTC_HandleTypeDef RTC_Handler;  //RTC���


uint8_t EV_RTC_Set(RTC_TypeDef * Instance, uint8_t Sec, uint8_t Min, uint8_t Hour, uint8_t Weekday, uint8_t Date, uint8_t Month, uint16_t Year)
{
	RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;
	
	RTC_TimeStructure.Hours=Hour;
	RTC_TimeStructure.Minutes=Min;
	RTC_TimeStructure.Seconds=Sec;
	RTC_TimeStructure.TimeFormat=RTC_HOURFORMAT12_AM;
	RTC_TimeStructure.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation=RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);	

	RTC_DateStructure.Date=Date;
	RTC_DateStructure.Month=Month;
	RTC_DateStructure.WeekDay=Weekday;
	RTC_DateStructure.Year=Year;
	return HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);    
}
uint8_t EV_RTC_Get(RTC_TypeDef * Instance, uint8_t * Sec, uint8_t * Min, uint8_t * Hour, uint8_t * Weekday, uint8_t * Date, uint8_t * Month, uint16_t * Year)
{
	RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;

    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);  
    HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);

	*Hour = RTC_TimeStructure.Hours;
	*Min = RTC_TimeStructure.Minutes;
	*Sec = RTC_TimeStructure.Seconds;

	*Date = RTC_DateStructure.Date;
	*Month = RTC_DateStructure.Month;
	*Weekday = RTC_DateStructure.WeekDay;
	*Year = RTC_DateStructure.Year;
  
  return HAL_OK;
}
//RTC��ʼ��
//����ֵ:0,��ʼ���ɹ�;
//       2,�����ʼ��ģʽʧ��;
uint8_t EV_RTC_Init(RTC_TypeDef * Instance)
{      
    uint8_t sec = 1;
    uint8_t min = 1;
    uint8_t hour = 1;

    uint8_t weekday = 1;
    uint8_t date = 1;
    uint8_t month = 1;
    uint16_t year = 1900;
	
	RTC_Handler.Instance=Instance;
    RTC_Handler.Init.HourFormat=RTC_HOURFORMAT_24;//RTC����Ϊ24Сʱ��ʽ 
    RTC_Handler.Init.AsynchPrediv=0X7F;           //RTC�첽��Ƶϵ��(1~0X7F)
    RTC_Handler.Init.SynchPrediv=0XFF;            //RTCͬ����Ƶϵ��(0~7FFF)   
    RTC_Handler.Init.OutPut=RTC_OUTPUT_DISABLE;     
    RTC_Handler.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    RTC_Handler.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK) return 2;
      
    if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0)!=0X5050)//�Ƿ��һ������
    { 
        EV_RTC_Set(Instance, sec, min, hour, weekday, date, month, year);	        //����ʱ�� ,����ʵ��ʱ���޸�
        HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5050);//����Ѿ���ʼ������
    }
    return 0;
}


//RTC�ײ�������ʱ������
//�˺����ᱻHAL_RTC_Init()����
//hrtc:RTC���
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();//ʹ�ܵ�Դʱ��PWR
	HAL_PWR_EnableBkUpAccess();//ȡ����������д����
    
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;//LSE����
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  //RTCʹ��LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;//����ΪRTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;//RTCʱ��ԴΪLSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        
    __HAL_RCC_RTC_ENABLE();//RTCʱ��ʹ��
}

//��������ʱ��(����������,24Сʱ��)
//week:���ڼ�(1~7) @ref  RTC_WeekDay_Definitions
//hour,min,sec:Сʱ,����,����
void RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //Сʱ
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //����
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //��
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
    
    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;//��ȷƥ�����ڣ�ʱ����
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//������
    RTC_AlarmSturuct.AlarmDateWeekDay=week; //����
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     //����A
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

//�����Ի��Ѷ�ʱ������  
/*wksel:  @ref RTCEx_Wakeup_Timer_Definitions
#define RTC_WAKEUPCLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)
#define RTC_WAKEUPCLOCK_CK_SPRE_16BITS      ((uint32_t)0x00000004)
#define RTC_WAKEUPCLOCK_CK_SPRE_17BITS      ((uint32_t)0x00000006)
*/
//cnt:�Զ���װ��ֵ.����0,�����ж�.
void RTC_Set_WakeUp(uint32_t wksel,uint16_t cnt)
{ 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);//���RTC WAKE UP�ı�־
	
	HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,cnt,wksel);            //������װ��ֵ��ʱ�� 
	
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);
}
    
//RTC����A�жϴ���ص�����
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{

}

//RTC WAKE UP�жϷ�����
void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 
}

//RTC WAKE UP�жϴ���
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
}