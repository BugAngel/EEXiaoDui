#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "ultrasonic.h"

//ALIENTEK 探索者STM32F407开发板 实验5
//外部中断实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司
//作者：正点原子 @ALIENTEK

int main(void)
{
    uint8_t high, low, UART_TEMP;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);    //初始化延时函数
    uart_init(115200); 	//串口初始化

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
						
						printf(" distance ： %d\r\n",ultra_distance);
            ultra_start_f = -1;
        }
        delay_ms(100);
        //Delay(100000);
    }

}
