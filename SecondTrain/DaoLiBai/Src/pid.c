#include "pid.h"

void PID_init(PidTypeDef *pid,float Kp,float Ki,float Kd)
{
    pid->SetSpeed=0.0;
    pid->ActualSpeed=0.0;
    pid->err=0.0;
    pid->err_last=0.0;
    pid->err_next=0.0;
    pid->Kp=Kp;
    pid->Ki=Ki;
    pid->Kd=Kd;
}
//位置式PID
float PID_PositionRealize(PidTypeDef *pid,float speed,int16_t ActualSpeed)
{
    pid->SetSpeed=speed;
    pid->ActualSpeed=ActualSpeed;
    pid->err=pid->SetSpeed-pid->ActualSpeed;
    pid->integral+=pid->err;
    pid->output=pid->Kp*pid->err+pid->Ki*pid->integral+pid->Kd*(pid->err-pid->err_next);
    pid->err_next=pid->err;
    return pid->output;
}
//增量式PID
float PID_IncermentRealize(PidTypeDef *pid,float speed,int16_t ActualSpeed)
{
    pid->SetSpeed=speed;
    pid->ActualSpeed=ActualSpeed;
    pid->err=pid->SetSpeed-pid->ActualSpeed;
    float  incrementSpeed=pid->Kp*(pid->err-pid->err_next)+pid->Ki*pid->err+pid->Kd*(pid->err-2*pid->err_next+pid->err_last);
    pid->output+=incrementSpeed;
    pid->err_last=pid->err_next;
    pid->err_next=pid->err;
    return pid->output;
}
