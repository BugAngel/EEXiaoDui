#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "ultrasonic.h"

//ALIENTEK ̽����STM32F407������ ʵ��5
//�ⲿ�ж�ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾
//���ߣ�����ԭ�� @ALIENTEK

int main(void)
{
    uint8_t high, low, UART_TEMP;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
    delay_init(168);    //��ʼ����ʱ����
    uart_init(115200); 	//���ڳ�ʼ��

    Ultrasonic_Init();

    while (1)
    {
        Ultra_Duty();

        if (ultra_start_f == 0)
        {
//            UART_TEMP = 0x03;
//            USART1->DR = UART_TEMP;
//					while((USART1->SR&0X40)==0);
//					
////        USART_SendData(USART1, UART_TEMP);
//            UART_TEMP = 0xfc;
////        USART_SendData(USART1, UART_TEMP);
//            USART1->DR = UART_TEMP;
//					while((USART1->SR&0X40)==0);
//					
//            high = (uint8_t)(ultra_distance >> 8);
//            low = (uint8_t)(ultra_distance & 0xFF);
////        USART_SendData(USART1, low);
////        USART_SendData(USART1, high);
//            USART1->DR = low;
//					while((USART1->SR&0X40)==0);
//					
//            USART1->DR = high;
//					while((USART1->SR&0X40)==0);
//					
//            UART_TEMP = 0xfc;
////        USART_SendData(USART1, UART_TEMP);
//            USART1->DR = UART_TEMP;
//					while((USART1->SR&0X40)==0);
//					
//            UART_TEMP = 0x03;
////        USART_SendData(USART1, UART_TEMP);
//            USART1->DR = UART_TEMP;
//						while((USART1->SR&0X40)==0);
						
						printf(" distance �� %d\r\n",ultra_distance);
            ultra_start_f = -1;
        }
        delay_ms(100);
        //Delay(100000);
    }

}
