/**
  ******************************************************************************
  * @file    wifi.c
  * @author  WangRong
  * @version V1.1
  * @date    2016.10.1
  * @note    ����WIFI��������
  ******************************************************************************
  */
#include "wifi.h"
#include "Motor.h"
#include "pid.h"

extern UART_HandleTypeDef huart1;

uint8_t WIFI_RX_BUF[WIFI_REC_LEN] = {0};   //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��   ������ɱ�־
//bit14��   ���յ�0x0d
//bit13~0�� ���յ�����Ч�ֽ���Ŀ
uint16_t WIFI_RX_STA = 0;     //����״̬���

//extern uint8_t MotorFlag;       //С�������ź�
//extern int32_t UpperSetSpeed;
//extern PidTypeDef RFMotorPID;
//extern PidTypeDef LBMotorPID;
//extern PidTypeDef LFMotorPID;
//extern PidTypeDef RBMotorPID;
//extern PidTypeDef YawPID;
//extern float UpperSetYaw;//�趨�Ƕ�

/**
  * @brief  ������λ������
  * @param  usart_receive ���մ�������
      * @arg  ��һλ  0xff  ����
        * @arg  �ڶ�λ  ��Ӧ����
        * @arg  ����λ  ������������
        * @arg  ����λ  ���ݵ�һ��λС��
        * @arg  ����λ  0x0d  ����
        * @arg  ����λ  0x0a  ����
    * @retval NONE
  * @attention
    @arg
  */
void UpperComputer(uint8_t *usart_receive)
{
    float usart_temp;
    uint8_t len;
    if ((WIFI_RX_STA & 0x8000) == 0) //����δ���
    {
        if (WIFI_RX_STA & 0x4000) //���յ���0x0d
        {
            if (*usart_receive != 0x0a)
            {
                WIFI_RX_STA = 0; //���մ���,���¿�ʼ
            }
            else
            {
                WIFI_RX_STA |= 0x8000;  //���������
            }
        }
        else //��û�յ�0X0D
        {
            if (*usart_receive == 0x0d)
            {
                WIFI_RX_STA |= 0x4000;
            }
            else
            {
                WIFI_RX_BUF[WIFI_RX_STA & 0X3FFF] = *usart_receive ;
                WIFI_RX_STA++;
                if (WIFI_RX_STA > (WIFI_REC_LEN - 1))
                {
                    WIFI_RX_STA =  0; //�������ݴ���,���¿�ʼ����
                }
            }
        }
    }

    if (WIFI_RX_STA & 0x8000)
    {
        len = WIFI_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
        //�����һλ  ȷ��������ȷ
        if (WIFI_RX_BUF[0] == 0xff)
        {
            if (len == 5)
            {
                usart_temp = (float)(WIFI_RX_BUF[3]) + ((float)WIFI_RX_BUF[4]) / 100;
            }
            if (WIFI_RX_BUF[2] == 0x01)
            {
                usart_temp = -usart_temp;
            }
            switch (WIFI_RX_BUF[1])
            {
                case Rec_Balance_P:

                    break;
                case Rec_Balance_I:

                    break;
                case Rec_Balance_D:

                    break;
//                case Rec_Position_P:
//                    YawPID.Kp = usart_temp;
//                    break;
//                case Rec_Position_I:
//                    YawPID.Ki = usart_temp;
//                    break;
//                case Rec_Position_D:
//                    YawPID.Kd = usart_temp;
//                    break;
//                case Rec_Speed_P:
//                    RFMotorPID.Kp = usart_temp;
//                    LBMotorPID.Kp = usart_temp;
//                    LFMotorPID.Kp = usart_temp;
//                    RBMotorPID.Kp = usart_temp;
//                    break;
//                case Rec_Speed_I:
//                    RFMotorPID.Ki = usart_temp;
//                    LBMotorPID.Ki = usart_temp;
//                    LFMotorPID.Ki = usart_temp;
//                    RBMotorPID.Ki = usart_temp;
//                    break;
//                case Rec_Speed_D:
//                    RFMotorPID.Kd = usart_temp;
//                    LBMotorPID.Kd = usart_temp;
//                    LFMotorPID.Kd = usart_temp;
//                    RBMotorPID.Kd = usart_temp;
//                    break;
                case Rec_Steer_P:

                    break;
                case Rec_Steer_I:

                    break;
                case Rec_Steer_D:

                    break;
                case Rec_Forward:
//                    MotorSpeedLB(500);
//                    MotorSpeedLF(500);
//                    MotorSpeedRF(500);
//                    MotorSpeedRB(500);
                    break;
                case Rec_Backward:
//                    MotorSpeedLB(-500);
//                    MotorSpeedLF(-500);
//                    MotorSpeedRF(-500);
//                    MotorSpeedRB(-500);
                    break;
                case Rec_TurnLeft:

                    break;
                case Rec_TurnRight:

                    break;
//								case Rec_Firing:
//										MotorFlag = 1;
//										break;
//								case Rec_Misfire:
//										Stop();
//										break;
//								case Rec_EncoderForward:
//										UpperSetSpeed = 20;
//										break;
//								case Rec_EncoderBackward:
//										UpperSetSpeed = -20;
//										break;
                case Rec_EncoderTurnLeft:

                    break;

                case Rec_EncoderTurnRight:

                    break;
//                case Rec_EncoderFiring:
//                    MotorFlag = 3;
//                    break;
//                case Rec_EncoderMisfire:
//                    Stop();
//                    break;
//                case Rec_YAW:
//                    UpperSetYaw = usart_temp;
//                    break;
//                case Rec_SetSpeed:
//                    UpperSetSpeed = usart_temp;
//                    break;
            }
        }

        WIFI_RX_STA = 0;
    }



}
/**
  * @brief  ��uint8_t�������ݾ�����λ��
  * @param
    * @arg  data  Ҫ���͵�����
    * @arg  type  ������������
    * @retval NONE
  * @attention
        ͨѶЭ��
    * @arg  ��һλ  0xff  ����
        * @arg  �ڶ�λ  ��Ӧ��������
        * @arg  ����λ  Ҫ���͵�����
        * @arg  ����λ  0x0d  ����
        * @arg  ����λ  0x0a  ����
  */
void Send_Uint8_Data(uint8_t data, uint8_t type)
{
    uint8_t SendData[5] = {0xff, type, data, 0x0d, 0x0a};
    HAL_UART_Transmit(&huart1 , SendData, 5, 0xff);
}
/**
  * @brief  ��float�������ݾ�����λ��
  * @param
    * @arg  data  Ҫ���͵�����
    * @arg  type  ������������
    * @retval NONE
  * @attention
        ͨѶЭ��
    * @arg  ��һλ  0xff  ����
        * @arg  �ڶ�λ  ��Ӧ��������
        * @arg  ����λ  �ж�����
        * @arg  ����λ  Ҫ���͵���������λ
        * @arg  ����λ  Ҫ���͵�����С��λ
        * @arg  ����λ  0x0d  ����
        * @arg  ����λ  0x0a  ����
  */
void Send_Float_Data(float data, uint8_t type)
{
    uint8_t SendData[7] = {0xff, type, 0x00, 0x00, 0x00, 0x0d, 0x0a};
    if (data < 0)
    {
        SendData[2] = 0x01;
        data = -data;
    }
    //temp_1��������  temp_2��λС��
    uint8_t temp_1, temp_2;
    temp_1 = (uint8_t)data;
    SendData[3] = temp_1;
    temp_2 = (uint8_t)(100 * (data - (float)temp_1));
    SendData[4] = temp_2;

    HAL_UART_Transmit(&huart1 , SendData, 7, 0xff);
}

/**
  * @brief  ��CCD���ݾ�����λ��
  * @param
    * @arg  data  Ҫ���͵�����  128λ����
    * @arg  type  ������������
    * @retval NONE
  * @attention
        ͨѶЭ��
    * @arg  ��һλ  0x0f  ����
    * @arg  �ڶ�λ  type  ��������
        * @arg  �����ڶ�λ  0x0d  ����
        * @arg  ������һλ  0x0a  ����
        * @arg  ����128λ����λ
  */
void Send_CCD_Data(uint8_t data[], uint8_t type)
{
    uint8_t temp_1 = 0x0f;
    uint8_t temp_2[2] = {0x0d, 0x0a};
    HAL_UART_Transmit(&huart1 , &temp_1, 1, 0xff);
    HAL_UART_Transmit(&huart1 , &type, 1, 0xff);
    HAL_UART_Transmit(&huart1 , data, 128, 0xffff);
    HAL_UART_Transmit(&huart1 , temp_2, 2, 0xff);
}

void Send_Data(uint8_t data)
{
    uint8_t SendData[5] = {0x03,0xfc, data, 0xfc, 0x03};
    HAL_UART_Transmit(&huart1 , SendData, 5, 0xff);
}
