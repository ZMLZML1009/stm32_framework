#ifndef __SYS_H
#define __SYS_H	

/*
命名规范：（临时）
	1、系统使用外设功能的启用与否均用宏定义SYSTEM_SUPPORT_XX
	2、RTOS任务函数均使用xx_task()命名
	3、任务函数与任务名称一致，任务索引与函数一致，只是task部分简写为t
	4、函数注释规范如下：*/
	
	/*__________函数简述_____________*/
	/********************************
	*描述：函数详细描述——————！按需要进行修改！
	*参数：		1、...
				2、...
	*返回值：	1、NULL
	********************************/
	
	/*
	5、函数返回值0表示成功，非0表示失败，此非零数可以表示错误代码
	6、一类或者一个模块内的变量尽量用结构体打包，多组数据就用结构体数组
	*/
	
/*应用规范：（临时，待整理）
	1、IO尽量设计为低电平有效，高电平截止。
	2、使用C99模式编译！
	3、I2C使用软件实现
	4、FreeRTOS所有组件始终使用动态申请内存；FreeRTOS的配置文件除了用户按需更改区域，尽量不要动；一般不使用软件定时器，用硬件定时器或者周期延时替代即可
	5、尽量减少数据传输过程中的拷贝，常用指针、结构体指针当传输数据
	6、调用函数尽量不用NULL实参，尽量给一个具体的值
*/

#define SYSTEM_SUPPORT_OS		1				/*定义是否使用FreeRTOS，不是0就是1——————！按需要进行修改！
													FreeRTOS版本：v10.3.1
													默认用于任务的RAM堆栈大小为5KB，按需修改！
												*/
#define SYSTEM_SUPPORT_Menu		1				/*提供一个菜单模板，把系统的输入、输出、执行功能的标志位控制全部打包！注意，menu库包含了printf.h库和MyString.h库！*/
#define SYSTEM_SUPPORT_MyString	1				/*提供一个实现了string.h大部分字符操作函数的库*/
#define SYSTEM_SUPPORT_sprintf	1				/*包含且编译printf.h，github开源文件，无依赖，功能比较全。
													约占6KB，对于stm32够，对于其他小容量MCU则看“其他几个sprintf实现”文件夹里面的,不要纠结了。
													https://github.com/mpaland/printf
													提供API：
													int printf(const char* format, ...); 								不用
													int printf_uart(unsigned char uart,const char* format, ...); 		用，可选发送串口
																		串口选择可选：UART1/UART2/UART3，需要先使能相关串口
													int sprintf(char* buffer, const char* format, ...); 				不带字节数量限制
													int snprintf(char* buffer, size_t count, const char* format, ...); 	带字节数量限制，更安全
												注意：另在SYSTEM文件夹内提供strprintf.h，功能比较单一，但很小。
												*/
int myatoi(const char *str);					/*提供一个字符串转整形的实现*/
#define SYSTEM_SUPPORT_pid		0				/*提供一个pid算法实现库，集成了积分分离和变限积分，以及可选的不完全微分和微分先行，具体用法看pid.h里面*/

/*所有main的头文件都放在这里*/
#if SYSTEM_SUPPORT_OS
	#include "FreeRTOS.h"	//FreeRTOS使用,这里不要乱动	  
	#include "task.h"
	#include "TaskConfig.h"
#else
	#include "BareConfig.h"
#endif

#include "sysVar.h"								/*定义系统级常用的变量、数据类型和二进制表示宏*/
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"						/*在里面的stm32f1xx_hal_conf.h里面选择用哪些外设的HAL库——————！按需要进行修改！*/
#include "ringbuf.h"							/*实现的一个环形缓存，用于串口接收模式*/

#if SYSTEM_SUPPORT_Menu
	#include "sys_menu.h"
#endif

#if SYSTEM_SUPPORT_MyString
	#include "MyString.h"
#endif

#if SYSTEM_SUPPORT_sprintf
	#include "printf.h"
#endif

#if SYSTEM_SUPPORT_pid
	#include "pid.h"
#endif

/*_____MCU外设启用选择函数_____*/
#define SYSTEM_MCO_PA8_OUT		0				/*设置PA8为MCO输出，默认时钟源为HSE*/

/*配置使用RTC，确保LSE连接有36.768kHz的晶振，确保RTC有VBAT备用电源
说明：按需要在配置函数内设置：时间，闹钟，读存BKP
其他：系统在待机模式下可选被闹钟唤醒*/
#define SYSTEM_RTC_ENABLE		0				
/*配置使用CRC循环冗余校验
这个CRC计算模块使用常见的、在以太网中使用的计算多项式：
X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 +X8 + X7 + X5 + X4 + X2 + X + 1
写成16进制就是：0x04C11DB7*/
#define SYSTEM_CRC_ENABLE		0
/*
F103系列有以下8个定时器：其中x8/xB系列仅有1、2、3、4定时器，xE和以上有全八个。
高级定时器1、8（定时器中断，
	高级PWM（7路，具体在xxx引脚，重映射在xxx引脚，形式如HAL F1手册的225页的表）），
通用定时器2-5（2、5为32位）
	（4路，具体在xxx引脚，重映射在xxx引脚），定时中断，输入捕获，输出比较，PWM 生成(边缘或中间对齐模式)，单脉冲模式输出，正交编码中断
基本定时器6、7（定时器中断，无PWM等）
说明：可用于ADC触发，
*/
#define STSTEM_TIM4_ENABLE		1			/*模板基本功能，必须开启！使用通用定时器4，提供时基，默认10ms周期中断*/

#define STSTEM_TIM3PWM_ENABLE	1			/*模板基本功能，必须开启！使用通用定时器3，默认不开启中断，提供四路PWM输出*/
											/*其中一路默认给蜂鸣器，蜂鸣器低电平有效！蜂鸣器控制需要用到TIM4！蜂鸣器连接在通道4上！
											改变占空比可调响度，改变频率可调声调*/
											/*默认：引脚上拉，输出极性为LOW，PWM1模式			(CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_PARTIAL	0	/*定时器3部分引脚重映射，Partial remap  	(CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_ENABLE		0	/*定时器3全部引脚重映射，Full remap     	(CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)*/
		#define STSTEM_TIM3PWM_CHANNEL_ENABLE	B0000_1000 /*输出通道选择，共四个通道，通道四必打开*/
													/*可选  B0000_0001|B0000_0010|B0000_0100|B0000_1000	*/
		#define arr 720-1
		#define prsc STSTEM_TIM3PWM_Period_5K		/*选择定时器输出频率*/
		#define STSTEM_TIM3PWM_Period_2K	(50-1)
		#define STSTEM_TIM3PWM_Period_5K	(20-1)
		#define STSTEM_TIM3PWM_Period_10K	(10-1)
		#define STSTEM_TIM3PWM_Period_20K	(5-1)
		#define STSTEM_TIM3PWM_Period_50K	(2-1)
		/*可用API：
					HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);		开启TIM3的PWM通道2
					HAL_TIM_PWM_Stop(&TIM3_Handler,TIM_CHANNEL_2);		关闭TIM3的PWM通道2，但看源码好像使所有通道都关闭了，待实验
					
					设置TIM3的PWM通道2的占空比百分数为88.8%，值需在0~100.0之间。默认向上计数，默认设置为当计数值小于此值时输出低电平。
					TIM3_set_Channel_Pulse(TIM3PWM_Channel_2,88.8);
		*/
		
#define SYSTEM_IWDG_ENABLE		1			/*开启独立看门狗，默认1S的喂狗周期，默认在TIM4定时中断里喂狗，用IWDG必开TIM4*/

/*开启串口，x8/xB系列有三个串口，最好不超过2M位每秒。默认均为：8位数据，1位停止，无校验，收发模式，开启接受中断*/
#define SYSTEM_UART1_ENABLE			1		/*使能串口1	       TX/PA9, RX/PA10		*/
#define SYSTEM_UART1_REMAP_ENABLE	0		/*串口1引脚重映射：TX/PB6, RX/PB7		*/
#define SYSTEM_UART1_BOUND			115200	/*串口1波特率*/

#define SYSTEM_UART2_ENABLE			0		/*使能串口2	       TX/PA2, RX/PA3		*/
#define SYSTEM_UART2_REMAP_ENABLE	0		/*串口2引脚重映射：TX/PD5, RX/PD6，可以设置，但对于C8T6无此引脚*/
#define SYSTEM_UART2_BOUND			115200	/*串口2波特率*/

#define SYSTEM_UART3_ENABLE			0		/*使能串口3	       TX/PB10, RX/PB11		*/
#define SYSTEM_UART3_REMAP_ENABLE	0		/*串口3引脚重映射：TX/PD8,  RX/PD9，可以设置，但对于C8T6无此引脚*/
#define SYSTEM_UART3_BOUND			115200	/*串口3波特率*/

/*开启硬件SPI，x8/xB系列有两个SPI，最高18M位每秒*/
#define SYSTEM_SPI_ENABLE		1

/*_____________用户函数_______________*/

/*_____________系统函数_______________*/
//extern static uint8_t Init_OK_Num;

extern uint32_t UIDw[3]; /*保存STM32内部UID识别码，全球唯一识别码*/
extern uint32_t sysCoreClock; /*获取HCLK频率，外设时钟均来自此再分频*/

void sys_MCU_Init_Seq(void);				/*MCU外设初始化序列，所有初始化写到这里面*/
void sys_Device_Init_Seq(void);				/*器件外设初始化，并开机自检*/

extern uint8_t is_quitFault;
void FaultASSERT(uint8_t errNum,char* message,uint8_t* is_quit);				/*表示初始化有问题，串口提示，指示灯或者蜂鸣器进行提示，并进入死循环*/
uint8_t Stm32_Clock_Init(uint32_t PLL);		/*时钟系统配置*/

#if SYSTEM_UART1_ENABLE||SYSTEM_UART2_ENABLE||SYSTEM_UART3_ENABLE
	#define UART1	1
	#define UART2	2
	#define UART3	3
	#define RXBUFFERSIZE   1 					//缓存大小
	extern u8 aRxBuffer1[RXBUFFERSIZE],aRxBuffer2[RXBUFFERSIZE],aRxBuffer3[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
	extern UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART句柄
	void sys_USART1_ENABLE(void);
	void sys_USART2_ENABLE(void);
	void sys_USART3_ENABLE(void);
	
	#define USART1_RX_BUF_MaxNum 200
	extern char USART1_RX_BUF[USART1_RX_BUF_MaxNum]; /*串口1的模式1数据接受区*/
	extern u16 USART1_RX_CONFIG;	/*    x            x           x        x xxxx xxxx xxxx      */
									/*(接受完成) (协议模式0/1) (接收到\r)   串口1接受数据字节计数 */
									/*串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出实现接受区，无协议*/
#endif

#if SYSTEM_MCO_PA8_OUT
	void sys_MCO_Out_Enable(void);				/*MCO在PA8上输出启动，默认时钟源HSE*/
#endif

#if STSTEM_TIM4_ENABLE

	__packed struct TIM4_IT_FLAGS
	{
		u8 _10msec_flag;	//10毫秒标志和计数
		u16 _10msec;
		
		u8 _100msec_flag;	//100毫秒标志和计数
		u16 _100msec;
		
		u8 _300msec_flag;	//300毫秒标志
		
		u8 _1sec_flag;		//1秒标志和计数
		u16 _1sec;
		
		u8 _1min_flag;		//1分钟标志和计数
		u16 _1min;
	};
	extern struct TIM4_IT_FLAGS Timer_IT_flags;
	extern TIM_HandleTypeDef TIM4_Handler;      //通用定时器4句柄 
	void sys_TIM4_ENABLE(void);
#endif
	
#if STSTEM_TIM3PWM_ENABLE
	/*蜂鸣器连接在通道4上*/
	extern u8 is_buzzer_once;
	extern u8 is_buzzer_bibi;
	#define buzzer_bibi_once	is_buzzer_once = 1;  //用户使用，蜂鸣器叫唤一声
	#define buzzer_bibi_on		is_buzzer_bibi = 1;  //用户使用，蜂鸣器间歇叫唤开启
	#define buzzer_bibi_off		is_buzzer_bibi = 0;  //用户使用，蜂鸣器间歇叫唤关闭
	extern TIM_HandleTypeDef 	TIM3_Handler;      	//定时器3句柄
	extern TIM_OC_InitTypeDef 	TIM3_CH1Handler,TIM3_CH2Handler,TIM3_CH3Handler,TIM3_CH4Handler;	//定时器3通道1-4句柄
	#define TIM3PWM_Channel_1	1
	#define TIM3PWM_Channel_2	2
	#define TIM3PWM_Channel_3	3
	#define TIM3PWM_Channel_4	4
	void sys_TIM3PWM_ENABLE(void);
	void TIM3_set_Channel_Pulse(u8 channel,float percent);
#endif

#if SYSTEM_IWDG_ENABLE
	extern IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄
	void sys_IWDG_ENABLE(void);
	void IWDG_Feed(void);
#endif

#if SYSTEM_CRC_ENABLE
	#define BUFFER_SIZE    114
	extern CRC_HandleTypeDef hcrc;
	extern const uint32_t aDataBuffer[BUFFER_SIZE];
	extern uint32_t uwExpectedCRCValue;
	void sys_CRC_ENABLE(void);
	void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc);
#endif

#if SYSTEM_RTC_ENABLE
	extern RTC_HandleTypeDef RTC_Handler;  //RTC句柄
	//时间结构体
	typedef struct 
	{
		vu8 hour;
		vu8 min;
		vu8 sec;			
		//公历日月年周
		vu16 w_year;
		vu8  w_month;
		vu8  w_date;
		vu8  week;	
	}_calendar_obj;					 
	extern _calendar_obj calendar;				//用户用！RTC结构体，实际用时只需要读这个结构体获取时间即可
	uint8_t sys_RTC_Enable(void);						/*RTC配置*/
	u8 Is_Leap_Year(u16 year);					//平年,闰年判断
	u8 RTC_Get(void);         					//获取时间，在RTC秒中断中执行，自动更新calendar时间，用户不用管
	u8 RTC_Get_Week(u16 year,u8 month,u8 day);
	u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);		//用户用！设置时间	
	u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);	//用户用！设置闹钟
	//HAL_StatusTypeDef HAL_RTC_SetAlarm_IT (RTC_HandleTypeDef * hrtc, RTC_AlarmTypeDef * sAlarm,uint32_t Format);
#endif


void delay_init(void);
void delay_ms(uint16_t nms);
void delay_xms(uint16_t nms);
void delay_us(uint32_t nus);


/*Author:	正点原子@ALIENTEK*/
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

/*Author:		MaxwellXyao*/
#define BIT(n) 					(1 << n) 				//位mask
#define bit_SetTrue(x,mask) 	(x |= mask)				//该位设置为真
#define bit_SetFalse(x,mask)	(x &= ~mask)			//该位设置为假
#define bit_Toggle(x,mask) 		(x ^= mask)				//位切换
#define bit_IsTrue(x,mask) 		((x & mask) != 0)		//该位是否真
#define bit_IsFalse(x,mask) 	((x & mask) == 0)		//该位是否假

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址

#endif


