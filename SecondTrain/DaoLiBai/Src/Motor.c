/**
******************************************************************************
    * @file     Motor.c
    * @author   WangRong
    * @version  1.0
    * @date     2017/4/30
    * @brief    Motor Driver
            @arg    TIM1 CH1 2���������
                        @arg    TIM1 CH3 4������ǰ���
                        @arg    TIM8 CH1 3������ǰ���
                        @arg    TIM8 CH2 4�����Һ���
******************************************************************************
*/

#include "Motor.h"

extern TIM_HandleTypeDef htim4;

/**
  * @brief  ��Ƭ��PWM���ܿ���
  * @param  NONE
  * @retval NONE
  */
void Motor_init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}

/**
  * @brief  �������
  * @param  speed_value:Ԥ��PWM CCR�Ĵ���ֵ
  * @retval NONE
  */
void MotorSpeed(int32_t speed_value)
{

    if (speed_value > 0)     //��ת
    {
        if (speed_value > MOTOR_SPEED_MAX)
        {
            speed_value = MOTOR_SPEED_MAX;
        }
        if (speed_value < MOTOR_SPEED_MIN)
        {
            speed_value = MOTOR_SPEED_MIN;
        }
        TIM4->CCR1 = speed_value;
        TIM4->CCR2 = 0;
    }
    else                       //��ת
    {
        if (speed_value < -MOTOR_SPEED_MAX)
        {
            speed_value = -MOTOR_SPEED_MAX;
        }
        if (speed_value > -MOTOR_SPEED_MIN)
        {
            speed_value = -MOTOR_SPEED_MIN;
        }
        TIM4->CCR1 = 0;
        TIM4->CCR2 = -speed_value;
    }
}


/**
  * @brief  ͣ��
  * @param  NONE
  * @retval NONE
  */
void Stop(void)
{
    TIM4->CCR1 = 0;
    TIM4->CCR2 = 0;
}



