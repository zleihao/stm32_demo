/**
 * @Date:               2022.3.16
 * @Revision:           V1.0
 * @author:             ��ƴ�ʦ
 * @Affiliated unit��   �ƺӿƼ�ѧԺ
 * @Description:        ����STM32F103�Ĺ̼��⹤��ģ��
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

const char w_data[] = "����һ��FATFS���̲���\n";
char r_data[4096];
/**
 * @brief  ������
 * @param  ��
 * @retval ��
 */
int main(void)
{
	FRESULT res;
	UINT bw;
	// LED_GPIO_Config();

	// LED_BLUE;
	/* ���ڳ�ʼ�� */
	USART_Config();

	res = f_mount(&fs, "1:", 1);
	if (res == FR_OK) {
		printf("���̹��سɹ�\n");
	} else {
		/* code */
		printf("f_mount:res = %d\n", res);
	}

	if (res == FR_NO_FILESYSTEM) {
		printf("����δ��ʽ������ʼ��ʽ������...\n");
		res = f_mkfs("1:", 0, NULL, 4096);
		if (res == FR_OK) {
			printf("���̸�ʽ�����...\n");
		}
		res = f_mount(NULL, "1:", 1);
		res = f_mount(&fs, "1:", 1);
		if (res == FR_OK) {
			printf("���̹��سɹ�\n");
		}
	}

	res = f_open(&fp, "1:test.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
	if (res == FR_OK) {
		// res = f_write(&fp, w_data, sizeof(w_data), &bw);
		// f_lseek(&fp, 0);
		f_read(&fp, r_data, f_size(&fp), &bw);
		printf("��ȡ�������ݣ�%s\n", r_data);
	} else {
		printf("�ļ���ʧ��\n");
	}
	f_close(&fp);
	while (1) {
	}
}
