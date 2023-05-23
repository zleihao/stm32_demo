#ifndef __OLED_H
#define __OLED_H 

#include "bsp_SysTick.h"
#include "stdlib.h"	
#include "stm32f10x.h"

/*************************************************************
              GND  电源地
              VCC  5v电源
              D0   PB12（SCL）
              D1   PB13（SDA）
              RES  PB14
              DC   PB15
              CS   PB11
*************************************************************/

#define DC_Pin GPIO_Pin_12
#define DC_GPIO_Port GPIOB
#define CS_Pin GPIO_Pin_13
#define CS_GPIO_Port GPIOB
#define RST_Pin GPIO_Pin_15
#define RST_GPIO_Port GPIOB
#define SCLK_Pin GPIO_Pin_8
#define SCLK_GPIO_Port GPIOB
#define SDIN_Pin GPIO_Pin_9
#define SDIN_GPIO_Port GPIOB

//-----------------OLED端口定义---------------- 

#define OLED_SCLK_Clr() GPIO_ResetBits(SCLK_GPIO_Port,SCLK_Pin)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(SCLK_GPIO_Port,SCLK_Pin)

#define OLED_SDIN_Clr() GPIO_ResetBits(SDIN_GPIO_Port,SDIN_Pin)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(SDIN_GPIO_Port,SDIN_Pin)

#define OLED_RST_Clr() GPIO_ResetBits(RST_GPIO_Port,RST_Pin)//RES
#define OLED_RST_Set() GPIO_SetBits(RST_GPIO_Port,RST_Pin)

#define OLED_DC_Clr() GPIO_ResetBits(DC_GPIO_Port,DC_Pin)//DC
#define OLED_DC_Set() GPIO_SetBits(DC_GPIO_Port,DC_Pin)

#define OLED_CS_Clr()  GPIO_ResetBits(CS_GPIO_Port,CS_Pin)//CS
#define OLED_CS_Set()  GPIO_SetBits(CS_GPIO_Port,CS_Pin)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define u8 unsigned char
#define u32 unsigned int

void OLED_ClearPoint(u8 x, u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat, u8 cmd);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y);
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2);
void OLED_DrawCircle(u8 x, u8 y, u8 r);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1);
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1);
void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1);
void OLED_ScrollDisplay(u8 num, u8 space);
void OLED_WR_BP(u8 x, u8 y);
void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t *pic);
void OLED_Init(void);

#endif
