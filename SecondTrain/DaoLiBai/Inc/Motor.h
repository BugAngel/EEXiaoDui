/**
******************************************************************************
    * @file     Motor.h
    * @author   WangRong
    * @version  1.0
    * @date     2017/4/30
    * @brief    Motor Driver
******************************************************************************
*/


#ifndef __Motor_H
#define __Motor_H

#include "stm32f4xx_hal.h"

#define MOTOR_SPEED_MAX     1000
#define MOTOR_SPEED_MIN     0

void Motor_init(void);

void MotorSpeed(int32_t speed_value);

void Stop(void);
#endif  //__Motor_H

/* End of "Motor.h" */
