/**
******************************************************************************
    * @file     Motor.c
    * @author   WangRong
    * @version  1.0
    * @date     2017/4/30
    * @brief    Motor Driver
            @arg    TIM1 CH1 2控制左后电机
                        @arg    TIM1 CH3 4控制左前电机
                        @arg    TIM8 CH1 3控制右前电机
                        @arg    TIM8 CH2 4控制右后电机
******************************************************************************
*/

#include "Motor.h"

extern TIM_HandleTypeDef htim4;

/**
  * @brief  单片机PWM功能开启
  * @param  NONE
  * @retval NONE
  */
void Motor_init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}

/**
  * @brief  电机控制
  * @param  speed_value:预期PWM CCR寄存器值
  * @retval NONE
  */
void MotorSpeed(int32_t speed_value)
{

    if (speed_value > 0)     //正转
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
    else                       //反转
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
  * @brief  停车
  * @param  NONE
  * @retval NONE
  */
void Stop(void)
{
    TIM4->CCR1 = 0;
    TIM4->CCR2 = 0;
}



