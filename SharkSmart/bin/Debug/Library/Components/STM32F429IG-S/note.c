
/*******************************************************GPIO*****************************************************************/
//	Pin 	管脚号		PA0--PI15
//	Mode 	管脚模式	GPIO_MODE_OUTPUT_PP	输出
//						GPIO_MODE_INPUT		输入
//	Pull	管脚上下拉	GPIO_NOPULL			不拉
//						GPIO_PULLUP			上拉
//						GPIO_PULLDOWN		下拉
//	State	管脚状态	RESET				低电平
//						SET					高电平
uint8_t EV_GPIO_Init(uint8_t Pin, uint8_t Mode, uint8_t Pull);
uint8_t EV_GPIO_Set(uint8_t Pin, uint8_t State);
uint8_t EV_GPIO_Get(uint8_t Pin);
uint8_t EV_GPIO_Toggle(uint8_t Pin);
/*******************************************************EXTI*****************************************************************/
//	Mode	触发方式	GPIO_MODE_IT_FALLING下降沿触发
//						GPIO_MODE_IT_RISING	上升沿触发
//	Callback中断回调	
uint8_t EV_EXTI_Init(uint8_t Pin, uint8_t Mode,  void (*Callback)(void));
/*******************************************************ADC******************************************************************/
//	Instance序号		ADC1 ADC2
//	Pin		管脚
//	Channel	通道
//	其中Get函数返回值为采集的电压值，采集电压范围0-3.3V对应数值0-4095
uint8_t EV_ADC_Init(ADC_TypeDef * Instance, uint8_t Pin, uint8_t Channel);
uint16_t EV_ADC_Get(ADC_TypeDef * Instance, uint8_t Channel);
/*******************************************************CAN*****************************************************************/
//	Instance序号		CAN1 CAN2
//	TxPin	Tx管脚
//	RxPin	Rx管脚
//	Id		自身Id		11位
//  Speed	速率		推荐500000
//	Recv_Callback		Data				数据
//						Len					数据长度
uint8_t EV_CAN_Init(CAN_TypeDef * Instance, uint8_t TxPin, uint8_t RxPin, uint32_t Id, uint32_t Speed, uint8_t (* Recv_Callback)(uint8_t *Data, uint8_t Len));
uint8_t EV_CAN_Send(CAN_TypeDef * Instance, uint32_t Id, uint8_t *Data, uint8_t Len);
/*******************************************************DAC*****************************************************************/
//	Instance序号		DAC
//	Pin		管脚
// 	Channel 通道
//	Value	输出值		0-4095对应电压为0-3.3V
uint8_t EV_DAC_Init(DAC_TypeDef *Instance, uint8_t Pin, uint8_t Channel);
uint8_t EV_DAC_Set(DAC_TypeDef *Instance, uint8_t Channel, uint16_t Value);
uint8_t EV_DAC_Start(DAC_TypeDef *Instance, uint8_t Channel);
uint8_t EV_DAC_Stop(DAC_TypeDef *Instance, uint8_t Channel);
/*******************************************************I2C*****************************************************************/
//	Instance序号		I2C1 I2C2 I2C3
//	SckPin	Scl管脚
//	SdaPin	Sda管脚
//	I2CAddr	设备I2C地址
//	RegAddr	设备寄存器地址
//	RegAddrSize	寄存器地址长度	I2C_MEMADD_SIZE_8BIT 	8位
//								I2C_MEMADD_SIZE_16BIT	16位
//	Data	数据
//  Len		数据长度
uint8_t EV_I2C_Init(I2C_TypeDef * Instance, uint8_t SclPin, uint8_t SdaPin);
uint8_t EV_I2C_Read(I2C_TypeDef * Instance, uint8_t I2CAddr, uint8_t *Data, uint16_t Len);
uint8_t EV_I2C_Write(I2C_TypeDef * Instance, uint8_t I2CAddr, uint8_t *Data, uint16_t Len);
uint8_t EV_I2C_ReadReg(I2C_TypeDef * Instance, uint8_t I2CAddr, uint16_t RegAddr, uint32_t RegAddrSize, uint8_t *Data, uint16_t Len);
uint8_t EV_I2C_WriteReg(I2C_TypeDef * Instance, uint8_t I2CAddr, uint16_t RegAddr, uint32_t RegAddrSize, uint8_t *Data, uint16_t Len);
/*******************************************************RNG*****************************************************************/
//	Instance序号		RNG
uint8_t EV_RNG_Init(RNG_TypeDef *Instance);
uint32_t EV_RNG_Get(RNG_TypeDef *Instance);
/*******************************************************TIM*****************************************************************/
//	Instance序号		TIM1 TIM2 TIM3 TIM4 TIM5 TIM6 TIM7 TIM8 TIM9 TIM10 TIM11 TIM12 TIM13 TIM14
//	Time	定时时间	单位1ms 
//	Callback中断回调
uint8_t EV_TIM_Init(TIM_TypeDef * Instance, uint32_t Time, void (*Callback)(void));
uint8_t EV_TIM_Start(TIM_TypeDef * Instance);
uint8_t EV_TIM_Stop(TIM_TypeDef * Instance);
/*******************************************************PWM*****************************************************************/
//	Instance序号
//	Pin 	管脚
// 	Channel 通道
//  Fre		频率
//	Dutymax	满占空比时的数值
// 	Dutymax * Fre 必须小于96000000
uint8_t EV_PWM_Init(TIM_TypeDef * Instance, uint8_t Pin, uint8_t Channel, uint32_t Fre, uint32_t Dutymax);
uint8_t EV_PWM_Set(TIM_TypeDef * Instance, uint8_t Channel, uint16_t Duty);
/*******************************************************UART*****************************************************************/
//	Instance序号		USART1 USART2 USART3 UART4 UART5 USART6 UART7 UART8
// 	RxPin	RX管脚
//	TxPin   Tx管脚
//	BaudRate波特率
//	Recv_Callback	接收中断
//	Data	数据
//	Len		数据长度
uint8_t EV_UART_Init(USART_TypeDef *Instance, uint8_t TxPin, uint8_t RxPin, uint32_t BaudRate, uint8_t (*Recv_Callback)(uint8_t *Data, uint8_t Len));
uint8_t EV_UART_Send(USART_TypeDef *Instance, uint8_t *Data, uint16_t Len);
/*******************************************************RTC*****************************************************************/
//	Instance序号		RTC
uint8_t EV_RTC_Init(RTC_TypeDef * Instance);
uint8_t EV_RTC_Set(RTC_TypeDef * Instance, uint8_t Sec, uint8_t Min, uint8_t Hour, uint8_t Weekday, uint8_t Date, uint8_t Month, uint16_t Year);
uint8_t EV_RTC_Get(RTC_TypeDef * Instance, uint8_t * Sec, uint8_t * Min, uint8_t * Hour, uint8_t * Weekday, uint8_t * Date, uint8_t * Month, uint16_t * Year);
/*******************************************************SPI*****************************************************************/
//	Instance序号		SPI1 SPI2 SPI3 SPI4 SPI5 SPI6
//	ClkPin	时钟管脚
//	MosiPin	主出从入管脚
//	MisoPin	主入从出管脚
//	BaudRatePrescaler	数据时钟极性
//	CLKPolarity			时钟极性
//	CLKPhase			时钟速率	SPI_BAUDRATEPRESCALER_2		速度最快
//									SPI_BAUDRATEPRESCALER_4			|
//									SPI_BAUDRATEPRESCALER_8			|
//									SPI_BAUDRATEPRESCALER_16		|
//									SPI_BAUDRATEPRESCALER_32		|
//									SPI_BAUDRATEPRESCALER_64		|
//									SPI_BAUDRATEPRESCALER_128		|
//									SPI_BAUDRATEPRESCALER_256	速度最慢
//	Data	数据
//	Len		数据长度
uint8_t EV_SPI_Init(SPI_TypeDef * Instance, uint8_t ClkPin, uint8_t MosiPin, uint8_t MisoPin, uint32_t BaudRatePrescaler, uint32_t CLKPolarity, uint32_t CLKPhase);
uint8_t EV_SPI_Send(SPI_TypeDef * Instance, uint8_t *Data, uint16_t Len);
uint8_t EV_SPI_Recv(SPI_TypeDef * Instance, uint8_t *Data, uint16_t Len);
/*******************************************************LCD*****************************************************************/
//	Color	颜色
//	percent	亮度百分比	0--100
//	backcolor	背景色
//	pointcolor	画笔色
//	x		起始x
//	y		起始y
//	width	显示长度
//	height	显示高度
//	size	字体大小	16 24 32
//	p		字符串
void EV_LCD_Init(void);
void EV_LCD_Clear(uint32_t color);
void EV_LCD_BacklightSet(uint8_t percent);
void EV_LCD_SetColor(uint32_t backcolor, uint32_t pointcolor);
void EV_LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);
/*******************************************************RTOS*****************************************************************/
//	prio 	任务优先级
//	task	任务函数
//	p_name	任务名称
//	arg		参数
//	stk		任务内存
//	ticks   ms
void    OSTaskSuspend(uint8_t  prio) ;
void    OSTaskResume(uint8_t prio);
uint8_t OSTaskCreate(void (*task)(void *arg),const char * p_name, void *arg, uint32_t  *stk, uint8_t  prio);
void    OSTimeDly(uint32_t  ticks);
//在执行不能白打断的代码时使用以下语句
uint32_t cpu_sr;
OS_ENTER_CRITICAL();
//这里执行不能被打断的代码
OS_EXIT_CRITICAL();
/*******************************************************malloc*****************************************************************/
//	memx	内存区域	SRAMEX
//	ptr		数据指针
//	size	大小
void myfree(uint8_t memx,void *ptr);
void *mymalloc(uint8_t memx,uint32_t size);
/*******************************************************audio*****************************************************************/
//	vol		音量		0--100
//	fname	文件路径
void audio_start(void);
void audio_stop(void); 
void audio_vol_set(uint8_t vol);
uint8_t wav_play_song(uint8_t* fname);
uint8_t mp3_play_song(uint8_t* fname);

/*******************************************************fatfs*****************************************************************/
FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);				/* Open or create a file */
FRESULT f_close (FIL* fp);											/* Close an open file object */
FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);			/* Read data from a file */
FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	/* Write data to a file */
FRESULT f_lseek (FIL* fp, FSIZE_t ofs);								/* Move file pointer of a file object */
FRESULT f_truncate (FIL* fp);										/* Truncate file */
FRESULT f_sync (FIL* fp);											/* Flush cached data of a writing file */
FRESULT f_opendir (DIR* dp, const TCHAR* path);						/* Open a directory */
FRESULT f_closedir (DIR* dp);										/* Close an open directory */
FRESULT f_readdir (DIR* dp, FILINFO* fno);							/* Read a directory item */
FRESULT f_findfirst (DIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern);	/* Find first file */
FRESULT f_findnext (DIR* dp, FILINFO* fno);							/* Find next file */
FRESULT f_mkdir (const TCHAR* path);								/* Create a sub directory */
FRESULT f_unlink (const TCHAR* path);								/* Delete an existing file or directory */
FRESULT f_rename (const TCHAR* path_old, const TCHAR* path_new);	/* Rename/Move a file or directory */
FRESULT f_stat (const TCHAR* path, FILINFO* fno);					/* Get file status */
FRESULT f_chmod (const TCHAR* path, BYTE attr, BYTE mask);			/* Change attribute of the file/dir */
FRESULT f_utime (const TCHAR* path, const FILINFO* fno);			/* Change timestamp of the file/dir */
FRESULT f_chdir (const TCHAR* path);								/* Change current directory */
FRESULT f_chdrive (const TCHAR* path);								/* Change current drive */
FRESULT f_getcwd (TCHAR* buff, UINT len);							/* Get current directory */
FRESULT f_getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs);	/* Get number of free clusters on the drive */
FRESULT f_getlabel (const TCHAR* path, TCHAR* label, DWORD* vsn);	/* Get volume label */
FRESULT f_setlabel (const TCHAR* label);							/* Set volume label */
FRESULT f_forward (FIL* fp, UINT(*func)(const BYTE*,UINT), UINT btf, UINT* bf);	/* Forward data to the stream */
FRESULT f_expand (FIL* fp, FSIZE_t szf, BYTE opt);					/* Allocate a contiguous block to the file */
FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt);			/* Mount/Unmount a logical drive */
FRESULT f_mkfs (const TCHAR* path, BYTE sfd, UINT au);				/* Create a file system on the volume */
FRESULT f_fdisk (BYTE pdrv, const DWORD szt[], void* work);			/* Divide a physical drive into some partitions */
int f_putc (TCHAR c, FIL* fp);										/* Put a character to the file */
int f_puts (const TCHAR* str, FIL* cp);								/* Put a string to the file */
int f_printf (FIL* fp, const TCHAR* str, ...);						/* Put a formatted string to the file */
TCHAR* f_gets (TCHAR* buff, int len, FIL* fp);						/* Get a string from the file */

#define f_eof(fp) ((int)((fp)->fptr == (fp)->obj.objsize))
#define f_error(fp) ((fp)->err)
#define f_tell(fp) ((fp)->fptr)
#define f_size(fp) ((fp)->obj.objsize)
#define f_rewind(fp) f_lseek((fp), 0)
#define f_rewinddir(dp) f_readdir((dp), 0)


/*******************************************************socket*****************************************************************/
void socketaddr_set(struct sockaddr *addr, char *ip, uint16_t *port);
void socketaddr_get(struct sockaddr *addr, char *ip, uint16_t *port);


/********************************************************BLE*******************************************************************/
//	Data      		数据
//	Len		  		数据长度
//	Recv_Callback		data				数据
//						len					数据长度
uint8_t EV_BLE_Recv_Init(void (*Recv_Callback)(uint8_t *data, uint16_t len));
uint8_t EV_BLE_Send(uint8_t *Data, uint16_t Len);
/********************************************************ETH*******************************************************************/
//	WiFi_Name		无线网名字
//	Password		无线网密码
//	Path			升级服务器网址
//	f_Name			升级文件列表文件名称
//	Port			升级服务器端口号
//	Data			数据
//	Len				数据长度
//	S_Addr			IP地址转换的32位值
//	Recv_Callback		Data				数据
//						Len					数据长度
//						S_Addr				IP地址转换的32位值
uint8_t EV_WIFI_SET_SSID(char *WiFi_Name,char *Password);
uint8_t EV_Add_Updata_Info_Manage(char *Path,char *f_Name,uint16_t Port);
int EV_ETH_User_Send(uint8_t *Data,int Len,uint32_t S_Addr);
uint8_t EV_ETH_User_Recv_Init(uint8_t (*Recv_Callback)(uint8_t *Data,int Len,uint32_t S_Addr));


