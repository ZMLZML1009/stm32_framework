#ifndef __SYS_H
#define __SYS_H	

/*
�����淶������ʱ��
	1��ϵͳʹ�����蹦�ܵ����������ú궨��SYSTEM_SUPPORT_XX
	2��RTOS��������ʹ��xx_task()����
	3������������������һ�£����������뺯��һ�£�ֻ��task���ּ�дΪt
	4������ע�͹淶���£�*/
	
	/*__________��������_____________*/
	/********************************
	*������������ϸ����������������������Ҫ�����޸ģ�
	*������		1��...
				2��...
	*����ֵ��	1��NULL
	********************************/
	
	/*
	5����������ֵ0��ʾ�ɹ�����0��ʾʧ�ܣ��˷��������Ա�ʾ�������
	6��һ�����һ��ģ���ڵı��������ýṹ�������������ݾ��ýṹ������
	*/
	
/*Ӧ�ù淶������ʱ����������
	1��IO�������Ϊ�͵�ƽ��Ч���ߵ�ƽ��ֹ��
	2��ʹ��C99ģʽ���룡
	3��I2Cʹ������ʵ��
	4��FreeRTOS�������ʼ��ʹ�ö�̬�����ڴ棻FreeRTOS�������ļ������û�����������򣬾�����Ҫ����һ�㲻ʹ��������ʱ������Ӳ����ʱ������������ʱ�������
	5�������������ݴ�������еĿ���������ָ�롢�ṹ��ָ�뵱��������
	6�����ú�����������NULLʵ�Σ�������һ�������ֵ
*/

#define SYSTEM_SUPPORT_OS		1				/*�����Ƿ�ʹ��FreeRTOS������0����1������������������Ҫ�����޸ģ�
													FreeRTOS�汾��v10.3.1
													Ĭ�����������RAM��ջ��СΪ5KB�������޸ģ�
												*/
#define SYSTEM_SUPPORT_Menu		1				/*�ṩһ���˵�ģ�壬��ϵͳ�����롢�����ִ�й��ܵı�־λ����ȫ�������ע�⣬menu�������printf.h���MyString.h�⣡*/
#define SYSTEM_SUPPORT_MyString	1				/*�ṩһ��ʵ����string.h�󲿷��ַ����������Ŀ�*/
#define SYSTEM_SUPPORT_sprintf	1				/*�����ұ���printf.h��github��Դ�ļ��������������ܱȽ�ȫ��
													Լռ6KB������stm32������������С����MCU�򿴡���������sprintfʵ�֡��ļ��������,��Ҫ�����ˡ�
													https://github.com/mpaland/printf
													�ṩAPI��
													int printf(const char* format, ...); 								����
													int printf_uart(unsigned char uart,const char* format, ...); 		�ã���ѡ���ʹ���
																		����ѡ���ѡ��UART1/UART2/UART3����Ҫ��ʹ����ش���
													int sprintf(char* buffer, const char* format, ...); 				�����ֽ���������
													int snprintf(char* buffer, size_t count, const char* format, ...); 	���ֽ��������ƣ�����ȫ
												ע�⣺����SYSTEM�ļ������ṩstrprintf.h�����ܱȽϵ�һ������С��
												*/
int myatoi(const char *str);					/*�ṩһ���ַ���ת���ε�ʵ��*/
#define SYSTEM_SUPPORT_pid		0				/*�ṩһ��pid�㷨ʵ�ֿ⣬�����˻��ַ���ͱ��޻��֣��Լ���ѡ�Ĳ���ȫ΢�ֺ�΢�����У������÷���pid.h����*/

/*����main��ͷ�ļ�����������*/
#if SYSTEM_SUPPORT_OS
	#include "FreeRTOS.h"	//FreeRTOSʹ��,���ﲻҪ�Ҷ�	  
	#include "task.h"
	#include "TaskConfig.h"
#else
	#include "BareConfig.h"
#endif

#include "sysVar.h"								/*����ϵͳ�����õı������������ͺͶ����Ʊ�ʾ��*/
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"						/*�������stm32f1xx_hal_conf.h����ѡ������Щ�����HAL�⡪����������������Ҫ�����޸ģ�*/
#include "ringbuf.h"							/*ʵ�ֵ�һ�����λ��棬���ڴ��ڽ���ģʽ*/

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

/*_____MCU��������ѡ����_____*/
#define SYSTEM_MCO_PA8_OUT		0				/*����PA8ΪMCO�����Ĭ��ʱ��ԴΪHSE*/

/*����ʹ��RTC��ȷ��LSE������36.768kHz�ľ���ȷ��RTC��VBAT���õ�Դ
˵��������Ҫ�����ú��������ã�ʱ�䣬���ӣ�����BKP
������ϵͳ�ڴ���ģʽ�¿�ѡ�����ӻ���*/
#define SYSTEM_RTC_ENABLE		0				
/*����ʹ��CRCѭ������У��
���CRC����ģ��ʹ�ó����ġ�����̫����ʹ�õļ������ʽ��
X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 +X8 + X7 + X5 + X4 + X2 + X + 1
д��16���ƾ��ǣ�0x04C11DB7*/
#define SYSTEM_CRC_ENABLE		0
/*
F103ϵ��������8����ʱ��������x8/xBϵ�н���1��2��3��4��ʱ����xE��������ȫ�˸���
�߼���ʱ��1��8����ʱ���жϣ�
	�߼�PWM��7·��������xxx���ţ���ӳ����xxx���ţ���ʽ��HAL F1�ֲ��225ҳ�ı�������
ͨ�ö�ʱ��2-5��2��5Ϊ32λ��
	��4·��������xxx���ţ���ӳ����xxx���ţ�����ʱ�жϣ����벶������Ƚϣ�PWM ����(��Ե���м����ģʽ)��������ģʽ��������������ж�
������ʱ��6��7����ʱ���жϣ���PWM�ȣ�
˵����������ADC������
*/
#define STSTEM_TIM4_ENABLE		1			/*ģ��������ܣ����뿪����ʹ��ͨ�ö�ʱ��4���ṩʱ����Ĭ��10ms�����ж�*/

#define STSTEM_TIM3PWM_ENABLE	1			/*ģ��������ܣ����뿪����ʹ��ͨ�ö�ʱ��3��Ĭ�ϲ������жϣ��ṩ��·PWM���*/
											/*����һ·Ĭ�ϸ����������������͵�ƽ��Ч��������������Ҫ�õ�TIM4��������������ͨ��4�ϣ�
											�ı�ռ�ձȿɵ���ȣ��ı�Ƶ�ʿɵ�����*/
											/*Ĭ�ϣ������������������ΪLOW��PWM1ģʽ			(CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_PARTIAL	0	/*��ʱ��3����������ӳ�䣬Partial remap  	(CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_ENABLE		0	/*��ʱ��3ȫ��������ӳ�䣬Full remap     	(CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)*/
		#define STSTEM_TIM3PWM_CHANNEL_ENABLE	B0000_1000 /*���ͨ��ѡ�񣬹��ĸ�ͨ����ͨ���ıش�*/
													/*��ѡ  B0000_0001|B0000_0010|B0000_0100|B0000_1000	*/
		#define arr 720-1
		#define prsc STSTEM_TIM3PWM_Period_5K		/*ѡ��ʱ�����Ƶ��*/
		#define STSTEM_TIM3PWM_Period_2K	(50-1)
		#define STSTEM_TIM3PWM_Period_5K	(20-1)
		#define STSTEM_TIM3PWM_Period_10K	(10-1)
		#define STSTEM_TIM3PWM_Period_20K	(5-1)
		#define STSTEM_TIM3PWM_Period_50K	(2-1)
		/*����API��
					HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);		����TIM3��PWMͨ��2
					HAL_TIM_PWM_Stop(&TIM3_Handler,TIM_CHANNEL_2);		�ر�TIM3��PWMͨ��2������Դ�����ʹ����ͨ�����ر��ˣ���ʵ��
					
					����TIM3��PWMͨ��2��ռ�ձȰٷ���Ϊ88.8%��ֵ����0~100.0֮�䡣Ĭ�����ϼ�����Ĭ������Ϊ������ֵС�ڴ�ֵʱ����͵�ƽ��
					TIM3_set_Channel_Pulse(TIM3PWM_Channel_2,88.8);
		*/
		
#define SYSTEM_IWDG_ENABLE		1			/*�����������Ź���Ĭ��1S��ι�����ڣ�Ĭ����TIM4��ʱ�ж���ι������IWDG�ؿ�TIM4*/

/*�������ڣ�x8/xBϵ�����������ڣ���ò�����2Mλÿ�롣Ĭ�Ͼ�Ϊ��8λ���ݣ�1λֹͣ����У�飬�շ�ģʽ�����������ж�*/
#define SYSTEM_UART1_ENABLE			1		/*ʹ�ܴ���1	       TX/PA9, RX/PA10		*/
#define SYSTEM_UART1_REMAP_ENABLE	0		/*����1������ӳ�䣺TX/PB6, RX/PB7		*/
#define SYSTEM_UART1_BOUND			115200	/*����1������*/

#define SYSTEM_UART2_ENABLE			0		/*ʹ�ܴ���2	       TX/PA2, RX/PA3		*/
#define SYSTEM_UART2_REMAP_ENABLE	0		/*����2������ӳ�䣺TX/PD5, RX/PD6���������ã�������C8T6�޴�����*/
#define SYSTEM_UART2_BOUND			115200	/*����2������*/

#define SYSTEM_UART3_ENABLE			0		/*ʹ�ܴ���3	       TX/PB10, RX/PB11		*/
#define SYSTEM_UART3_REMAP_ENABLE	0		/*����3������ӳ�䣺TX/PD8,  RX/PD9���������ã�������C8T6�޴�����*/
#define SYSTEM_UART3_BOUND			115200	/*����3������*/

/*����Ӳ��SPI��x8/xBϵ��������SPI�����18Mλÿ��*/
#define SYSTEM_SPI_ENABLE		1

/*_____________�û�����_______________*/

/*_____________ϵͳ����_______________*/
//extern static uint8_t Init_OK_Num;

extern uint32_t UIDw[3]; /*����STM32�ڲ�UIDʶ���룬ȫ��Ψһʶ����*/
extern uint32_t sysCoreClock; /*��ȡHCLKƵ�ʣ�����ʱ�Ӿ����Դ��ٷ�Ƶ*/

void sys_MCU_Init_Seq(void);				/*MCU�����ʼ�����У����г�ʼ��д��������*/
void sys_Device_Init_Seq(void);				/*���������ʼ�����������Լ�*/

extern uint8_t is_quitFault;
void FaultASSERT(uint8_t errNum,char* message,uint8_t* is_quit);				/*��ʾ��ʼ�������⣬������ʾ��ָʾ�ƻ��߷�����������ʾ����������ѭ��*/
uint8_t Stm32_Clock_Init(uint32_t PLL);		/*ʱ��ϵͳ����*/

#if SYSTEM_UART1_ENABLE||SYSTEM_UART2_ENABLE||SYSTEM_UART3_ENABLE
	#define UART1	1
	#define UART2	2
	#define UART3	3
	#define RXBUFFERSIZE   1 					//�����С
	extern u8 aRxBuffer1[RXBUFFERSIZE],aRxBuffer2[RXBUFFERSIZE],aRxBuffer3[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
	extern UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART���
	void sys_USART1_ENABLE(void);
	void sys_USART2_ENABLE(void);
	void sys_USART3_ENABLE(void);
	
	#define USART1_RX_BUF_MaxNum 200
	extern char USART1_RX_BUF[USART1_RX_BUF_MaxNum]; /*����1��ģʽ1���ݽ�����*/
	extern u16 USART1_RX_CONFIG;	/*    x            x           x        x xxxx xxxx xxxx      */
									/*(�������) (Э��ģʽ0/1) (���յ�\r)   ����1���������ֽڼ��� */
									/*����1����Э�飺0Ϊֻ������'\r\n'��β�����ݣ�1Ϊ��FIFO�Ƚ��ȳ�ʵ�ֽ���������Э��*/
#endif

#if SYSTEM_MCO_PA8_OUT
	void sys_MCO_Out_Enable(void);				/*MCO��PA8�����������Ĭ��ʱ��ԴHSE*/
#endif

#if STSTEM_TIM4_ENABLE

	__packed struct TIM4_IT_FLAGS
	{
		u8 _10msec_flag;	//10�����־�ͼ���
		u16 _10msec;
		
		u8 _100msec_flag;	//100�����־�ͼ���
		u16 _100msec;
		
		u8 _300msec_flag;	//300�����־
		
		u8 _1sec_flag;		//1���־�ͼ���
		u16 _1sec;
		
		u8 _1min_flag;		//1���ӱ�־�ͼ���
		u16 _1min;
	};
	extern struct TIM4_IT_FLAGS Timer_IT_flags;
	extern TIM_HandleTypeDef TIM4_Handler;      //ͨ�ö�ʱ��4��� 
	void sys_TIM4_ENABLE(void);
#endif
	
#if STSTEM_TIM3PWM_ENABLE
	/*������������ͨ��4��*/
	extern u8 is_buzzer_once;
	extern u8 is_buzzer_bibi;
	#define buzzer_bibi_once	is_buzzer_once = 1;  //�û�ʹ�ã��������л�һ��
	#define buzzer_bibi_on		is_buzzer_bibi = 1;  //�û�ʹ�ã���������Ъ�л�����
	#define buzzer_bibi_off		is_buzzer_bibi = 0;  //�û�ʹ�ã���������Ъ�л��ر�
	extern TIM_HandleTypeDef 	TIM3_Handler;      	//��ʱ��3���
	extern TIM_OC_InitTypeDef 	TIM3_CH1Handler,TIM3_CH2Handler,TIM3_CH3Handler,TIM3_CH4Handler;	//��ʱ��3ͨ��1-4���
	#define TIM3PWM_Channel_1	1
	#define TIM3PWM_Channel_2	2
	#define TIM3PWM_Channel_3	3
	#define TIM3PWM_Channel_4	4
	void sys_TIM3PWM_ENABLE(void);
	void TIM3_set_Channel_Pulse(u8 channel,float percent);
#endif

#if SYSTEM_IWDG_ENABLE
	extern IWDG_HandleTypeDef IWDG_Handler; //�������Ź����
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
	extern RTC_HandleTypeDef RTC_Handler;  //RTC���
	//ʱ��ṹ��
	typedef struct 
	{
		vu8 hour;
		vu8 min;
		vu8 sec;			
		//������������
		vu16 w_year;
		vu8  w_month;
		vu8  w_date;
		vu8  week;	
	}_calendar_obj;					 
	extern _calendar_obj calendar;				//�û��ã�RTC�ṹ�壬ʵ����ʱֻ��Ҫ������ṹ���ȡʱ�伴��
	uint8_t sys_RTC_Enable(void);						/*RTC����*/
	u8 Is_Leap_Year(u16 year);					//ƽ��,�����ж�
	u8 RTC_Get(void);         					//��ȡʱ�䣬��RTC���ж���ִ�У��Զ�����calendarʱ�䣬�û����ù�
	u8 RTC_Get_Week(u16 year,u8 month,u8 day);
	u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);		//�û��ã�����ʱ��	
	u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);	//�û��ã���������
	//HAL_StatusTypeDef HAL_RTC_SetAlarm_IT (RTC_HandleTypeDef * hrtc, RTC_AlarmTypeDef * sAlarm,uint32_t Format);
#endif


void delay_init(void);
void delay_ms(uint16_t nms);
void delay_xms(uint16_t nms);
void delay_us(uint32_t nus);


/*Author:	����ԭ��@ALIENTEK*/
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
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
 
//IO�ڲ���,ֻ�Ե�һ��IO��!ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

/*Author:		MaxwellXyao*/
#define BIT(n) 					(1 << n) 				//λmask
#define bit_SetTrue(x,mask) 	(x |= mask)				//��λ����Ϊ��
#define bit_SetFalse(x,mask)	(x &= ~mask)			//��λ����Ϊ��
#define bit_Toggle(x,mask) 		(x ^= mask)				//λ�л�
#define bit_IsTrue(x,mask) 		((x & mask) != 0)		//��λ�Ƿ���
#define bit_IsFalse(x,mask) 	((x & mask) == 0)		//��λ�Ƿ��

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(uint32_t addr);	//���ö�ջ��ַ

#endif

