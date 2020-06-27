#include "sys.h" /*所有头文件均放在这里面*/

int main(void)
{
	/*选择是否用FreeRTOS和写初始化函数均在sys.c里*/
	/*FreeRTOS任务编写在TaskConfig.c里*/
	/*裸机程序任务编写在BareConfig.c里*/
	HAL_Init();                    	/*初始化HAL库*/
	sys_MCU_Init_Seq();					/*MCU外设初始化序列，所有初始化写到这里面——————！按需要进行修改！*/
	delay_ms(66);
	sys_Device_Init_Seq();				/*器件外设初始化，并开机自检——————！按需要进行修改！*/
	delay_ms(234);
	while(1)
	{
		#if SYSTEM_SUPPORT_OS
			Task_Begin(); 			/*开始跑FreeRTOS*/
		#else
			Bare_Begin();			/*开始跑裸机*/
		#endif
	}
}


