/**
  ******************************************************************************
  * @file    OLED.c
  * @author  WangRong
  * @version V1.0
  * @date    2017.4.1
  * @note    spi����OLED
  ******************************************************************************
  */
#ifndef __OLED_H
#define __OLED_H

#include "stm32f1xx_hal.h"			

//OLEDģʽ����
//0:SPIͨѶ
//1:IICͨѶ
//HAL��IICģʽ��������
#define OLED_MODE 0

//-----------------OLED�˿ڶ���----------------  		
#define OLED_RST_1  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET)
#define OLED_RST_0  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET)

#define OLED_DC_1   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)
#define OLED_DC_0   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET) 
 
//ʹ��4�ߴ��нӿ�ʱʹ�� 
#define OLED_SCLK_1 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET)  //D0
#define OLED_SCLK_0 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET)

#define OLED_SDIN_1   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET) //D1
#define OLED_SDIN_0   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET)

#define OLED_CMD  	0		//д����
#define OLED_DATA 	1		//д����

//OLED�����ú���
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);
void myOLED_Decimals(uint8_t x,uint8_t y, float als);
#endif
