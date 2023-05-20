/**
 * @Date:               2022.3.16
 * @Revision:           V1.0
 * @author:             点灯大师
 * @Affiliated unit：   黄河科技学院
 * @Description:        基于STM32F103的固件库工程模板
 * @Email:              im_leihao@163.com
 * @github:             https://github.com/zleihao
 */
#include "bsp_debug_usart.h"

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_spi_flash.h"
#include <string.h>
#include "ff.h"

FATFS fs;
FIL fp;

const char w_data[] = "这是一个FATFS工程测试\n";
char r_data[4096];
/**
 * @brief  主函数
 * @param  无
 * @retval 无
 */
int main(void)
{
	FRESULT res;
	UINT bw;
	// LED_GPIO_Config();

	// LED_BLUE;
	/* 串口初始化 */
	USART_Config();

	res = f_mount(&fs, "1:", 1);
	if (res == FR_OK) {
		printf("磁盘挂载成功\n");
	} else {
		/* code */
		printf("f_mount:res = %d\n", res);
	}

	if (res == FR_NO_FILESYSTEM) {
		printf("磁盘未格式化，开始格式化磁盘...\n");
		res = f_mkfs("1:", 0, NULL, 4096);
		if (res == FR_OK) {
			printf("磁盘格式化完成...\n");
		}
		res = f_mount(NULL, "1:", 1);
		res = f_mount(&fs, "1:", 1);
		if (res == FR_OK) {
			printf("磁盘挂载成功\n");
		}
	}

	res = f_open(&fp, "1:test.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
	if (res == FR_OK) {
		// res = f_write(&fp, w_data, sizeof(w_data), &bw);
		// f_lseek(&fp, 0);
		f_read(&fp, r_data, f_size(&fp), &bw);
		printf("读取到的内容：%s\n", r_data);
	} else {
		printf("文件打开失败\n");
	}
	f_close(&fp);
	while (1) {
	}
}
