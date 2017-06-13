#ifndef _USART5_H
#define _USART5_H

#include "stm32f4xx.h"
#include "stdio.h"	
#include "stm32f4xx_conf.h"


void Uart5_Init(u32 br_num);
void Uart5_IRQ(void);
void Uart5_Send(unsigned char *DataToSend ,u8 data_num);
#endif
