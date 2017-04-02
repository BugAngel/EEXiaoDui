/**
  ******************************************************************************
  * @file    key.c
  * @author  WangRong
  * @version V1.0
  * @date    2017.4.1
  * @note    扫描型独立按键
  ******************************************************************************
  */
#include "key.h"

/**
  * @brief  按键处理函数 0是按下
  * @param  mode: 0,不支持连续按;1,支持连续按;
  * @retval
            @arg  0:没有任何按键按下
            @arg  KEY0_PRES:KEY0按下
            @arg  KEY1_PRES:KEY1按下
        @attention  注意此函数有响应优先级,KEY0>KEY1!!
*/
uint8_t KEY_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0))
	{
		HAL_Delay(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
	}else if(KEY0==1&&KEY1==1)key_up=1; 	     
	return 0;// 无按键按下
}
