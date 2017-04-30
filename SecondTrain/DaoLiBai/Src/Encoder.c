/**
******************************************************************************
    * @file     Encoder.c
    * @author   WangRong
    * @version  1.0
    * @date     2017/4/30
    * @brief    Encoder Driver
******************************************************************************
*/

#include "Encoder.h"

extern TIM_HandleTypeDef htim3;

/**
  * @brief  ��Ƭ��������ģʽ����
  * @param  NONE
  * @retval NONE
  */
void Encoder_init(void)
{
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2);
}

/**
  * @brief  ȡ����ֵ����ռ�����
  * @param  htim ��ʱ��
  * @retval MotorSpeed ����������ֵ
  */
int16_t GetEncoderValue(TIM_HandleTypeDef htim)
{
    int16_t MotorSpeed;
    MotorSpeed = (int16_t)(__HAL_TIM_GET_COUNTER(&htim));
    __HAL_TIM_SET_COUNTER(&htim, 0);
    return MotorSpeed;
}
