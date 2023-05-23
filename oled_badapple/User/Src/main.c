#include "stm32f10x.h"
#include "bsp_sdio_sdcard.h"
#include "sdio_test.h"
#include "bsp_debug_usart.h"
#include "oled.h"
#include "stdio.h"
#include "ff.h"
#include "bmp.h"

void badapple_play(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	FATFS fs;	/* FatFs文件系统对象 */
	FRESULT res_sd;
	/* 初始化调试串口，一般为串口1 */
	USART_Config();
	OLED_Init();

	// 在外部SPI Flash 挂载文件系统，文件系统挂载时会对SPI设备初始化
	res_sd = f_mount(&fs, "0:", 1);
	if (res_sd == FR_OK) {
		printf("》文件系统挂载成功，可以进行读写测试\r\n");
	} else {
		printf("》文件系统挂载失败\n");
		return 0;
	}

	/* 操作完成，停机 */
	while (1) {
		badapple_play();
	}
}

void badapple_play(void)
{
	unsigned char G_Bin[1024];
	/*BIN图形显存 */
	FRESULT res;
	/*用来指示运行情况 */
	FIL fsrc;
	/* 文件对象指针 */
	uint32_t ls_move = 0;
	/* 存储文件指针移动 */
	UINT br;
	/*成功读取的字节个数 */
	res = f_open(&fsrc, "0:/badapple.bin", FA_READ);
	if (res == FR_OK) {
		//文件的路径要对应，即前面把badapple.bin放在SD根目录下
		while (1) {
			res = f_lseek(&fsrc, ls_move);
			res = f_read(&fsrc, G_Bin, sizeof(G_Bin), &br);
			if (br != 1024) {
				/* 已经把数据读取完成 */
				ls_move = 0;
				break;
			}
			OLED_ShowPicture(0, 0, 128, 64, G_Bin);/*显示帧数据 */

			SysTick_Delay_ms(70);  /*加点延迟，看起来不会太鬼畜*/
			ls_move += 1024;   /*bin件数据位置偏移 */
		}
	}
}


/***************************END OF FILE*******************************/