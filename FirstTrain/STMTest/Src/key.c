/**
  ******************************************************************************
  * @file    key.c
  * @author  WangRong
  * @version V1.0
  * @date    2017.4.1
  * @note    ɨ���Ͷ�������
  ******************************************************************************
  */
#include "key.h"

/**
  * @brief  ���������� 0�ǰ���
  * @param  mode: 0,��֧��������;1,֧��������;
  * @retval
            @arg  0:û���κΰ�������
            @arg  KEY0_PRES:KEY0����
            @arg  KEY1_PRES:KEY1����
        @attention  ע��˺�������Ӧ���ȼ�,KEY0>KEY1!!
*/
uint8_t KEY_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0))
	{
		HAL_Delay(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
	}else if(KEY0==1&&KEY1==1)key_up=1; 	     
	return 0;// �ް�������
}
