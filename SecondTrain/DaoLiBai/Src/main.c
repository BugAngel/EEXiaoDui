/**
******************************************************************************
* File Name          : main.c
* Description        : Main program body
******************************************************************************
*
* COPYRIGHT(c) 2017 STMicroelectronics
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "pid.h"
#include "Motor.h"
#include "Encoder.h"
#include "wifi.h"
#include "mymath.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

//电位器不均匀
//把PID函数输出的无符号改为有符号了
//电位器断点处AD大概为1200
//加一个基础速度
//杆太重了，PID调起来没有意义
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
uint8_t UART_TEMP;
uint8_t usart1_temp;        //USART1临时接收
PidTypeDef pid;
int16_t MotorEncoder = 0; //电机编码器值 正对电机 从上向下看  逆时针是正，顺时针是负
int32_t PWMSet = 0;
int flag = 0;
int PWM_temp=140; 
int SetSpeed=20;

//ADC
//11.37每度
//算法暂时只是用于摆杆装在这个角度
#define ADC_TIMES   30
uint32_t ADC_Value[ADC_TIMES];
uint8_t index_adc;
int ad1;
int adc_start = 0,adc_target = 0;
int abs_err = 0;

#define START_OFFSET   100   //补偿初始速度
//PID
PidTypeDef pid_degree;
/* USER CODE END 0 */

int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();

    /* USER CODE BEGIN 2 */
    Motor_init();
    Encoder_init();
    while (HAL_UART_Receive_IT(&huart1, &usart1_temp,
        sizeof(usart1_temp)) != HAL_OK);//开启串口1中断
    PID_init(&pid, 0.15, 0.5, 0.0);
    PID_init(&pid_degree, 2.5, 0, 0.0);
    MotorSpeed(0);

    //开启ADC+DMA
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Value, ADC_TIMES);

    //初始AD采样
    HAL_Delay(500);

    for(int i = 0;i < 7;i++)
    {
        for(index_adc = 0,ad1 =0; index_adc < ADC_TIMES;)
            ad1 += ADC_Value[index_adc++];

        ad1 /= ADC_TIMES;
        adc_start += ad1; 
    }
    adc_start /= 7;


    //AD不是均匀的不能直接加2048，暂时线直接手动定成这个值了
    adc_target = 1733;



    printf(" ADC value = %d\r\n", adc_start);

    HAL_Delay(500);




    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        //AD采样
        for(index_adc = 0,ad1 =0; index_adc < ADC_TIMES;)
            ad1 += ADC_Value[index_adc++];
        ad1 /= ADC_TIMES;


        if (ad1 > adc_start)     
            ad1 = ad1 - 4096;   //  减去断点影响

        if(flag)
        {





            PWMSet = (int32_t)PID_PositionRealize(&pid_degree,adc_target,ad1);    //tar  > ad1 应该逆时针 PWM为正

            if(PWMSet > 0)
                PWMSet += START_OFFSET;
            if(PWMSet < 0)
                PWMSet -= START_OFFSET;

            //限幅
            if(PWMSet > 999)
                PWMSet = 999;
            if(PWMSet < -999)
                PWMSet = -999;

            abs_err = ABS(adc_target - ad1);
            if (abs_err < 800)//先只有小角度才调节，方便调PID，断点处可能也会调节，跳动一下

                MotorSpeed(PWMSet); //正的时候是逆时针，负的时候是顺时针            
            else
                Stop ();


            //            printf(" ADC value = %d\r\n", ad1);
        }
        else
        {
            MotorEncoder = GetEncoderValue(htim3);
            Stop();
        }
        HAL_Delay (10);
        //        if (flag)
        //        {
        //            MotorEncoder = GetEncoderValue(htim3);
        //            PWMSet = (int32_t)PID_IncermentRealize(&pid, SetSpeed,MotorEncoder);
        //            MotorSpeed(PWMSet); //正的时候是逆时针，负的时候是顺时针

        //            //					MotorSpeed(PWM_temp); //匀速转测试的时候用

        //            UART_TEMP = 0x03;
        //            HAL_UART_Transmit(&huart1 , &UART_TEMP, 1, 0xff);
        //            UART_TEMP = 0xfc;
        //            HAL_UART_Transmit(&huart1 , &UART_TEMP, 1, 0xff);
        //            uint8_t low,high;
        //            high=(char)(MotorEncoder>>8);
        //            low=(char)(MotorEncoder&0xFF);
        //            HAL_UART_Transmit(&huart1 , &low, 1, 0xff);
        //            HAL_UART_Transmit(&huart1 , &high, 1, 0xff);
        //            UART_TEMP = 0xfc;
        //            HAL_UART_Transmit(&huart1 , &UART_TEMP, 1, 0xff);
        //            UART_TEMP = 0x03;
        //            HAL_UART_Transmit(&huart1 , &UART_TEMP, 1, 0xff);

        //            HAL_Delay(50);
        //        }
        //        else
        //        {
        //            MotorEncoder = GetEncoderValue(htim3);
        //            Stop();
        //        }
    }
    /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
        |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }

    /**Configure the Systick interrupt time 
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
* @brief Reports the name of the source file and the source line number
* where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */

}

#endif

/**
* @}
*/ 

/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
