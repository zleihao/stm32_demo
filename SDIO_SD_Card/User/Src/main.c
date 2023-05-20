#include "stm32f10x.h"
#include "bsp_sdio_sdcard.h"
#include "sdio_test.h"
#include "bsp_debug_usart.h"	
#include "bsp_led.h"
#include "bsp_key.h" 


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* 初始化LED灯 */
	LED_GPIO_Config();
	LED_BLUE;
	/* 初始化独立按键 */
	Key_GPIO_Config();

	/*初始化USART1*/
	USART_Config();

	printf("\r\n欢迎使用野火  STM32 开发板。\r\n");

	printf("在开始进行SD卡基本测试前，请给开发板插入32G以内的SD卡\r\n");
	printf("本程序会对SD卡进行 非文件系统 方式读写，会删除SD卡的文件系统\r\n");
	printf("实验后可通过电脑格式化或使用SD卡文件系统的例程恢复SD卡文件系统\r\n");
	printf("\r\n 但sd卡内的原文件不可恢复，实验前务必备份SD卡内的原文件！！！\r\n");

	printf("\r\n 若已确认，请按开发板的KEY1按键，开始SD卡测试实验....\r\n");

	/* Infinite loop */
	while (1) {
		/*按下按键开始进行SD卡读写实验，会损坏SD卡原文件*/
		if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON) {
			printf("\r\n开始进行SD卡读写实验\r\n");
			SD_Test();
		}
	}
}



/***************************END OF FILE*******************************/