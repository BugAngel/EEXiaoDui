/**
  ******************************************************************************
  * @file    key.c
  * @author  WangRong
  * @version V1.0
  * @date    2017.4.1
  * @note    扫描型独立按键
  ******************************************************************************
  */
#ifndef __KEY_H
#define __KEY_H	 

#include "stm32f1xx_hal.h"		

#define KEY0  HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)//读取按键0
#define KEY1  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)//读取按键1
 
#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 

uint8_t KEY_Scan(uint8_t mode);  	//按键扫描函数					    
#endif
