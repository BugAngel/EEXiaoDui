/********************************************************************
 * brd_cfg.h
 * .Description
 *     header file of main board configuration macros 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
#ifndef _BRG_CFG_H_
#define _BRG_CFG_H_

//#define MCU_IS_STM32F1XX
#define MCU_IS_STM32F4XX
//#define MCU_IS_STM32F7XX

//#define MCU_IS_NXP_LPC17XX

//#define MCU_IS_HT32F16XX

//#define M8266WIFI_CONFIG_VIA_USART
#define M8266WIFI_CONFIG_VIA_SPI

//#define M8266WIFI_SPI_ACCESS_USE_DMA

#if defined(MCU_IS_HT32F16XX)
#include "ht32f1655_56.h"
#endif

///////////////////////////////////////////////////////////////
// M8266WIFI Module Interface Board Usages Macros
//      -- nRESET, nCS
///////////////////////////////////////////////////////////////
#if   defined(MCU_IS_STM32F1XX)  // STM's STM32F1XX, using GPIOB14 as nRESET
#define  M8266WIFI_nRESET_GPIO_RCC_CLOCK_EN				  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define  M8266WIFI_nRESET_GPIO											GPIOB
#define  M8266WIFI_nRESET_PIN												GPIO_Pin_14
#elif defined(MCU_IS_STM32F4XX)  // STM's STM32F4XX, using GPIOB14 as nRESET
#define  M8266WIFI_nRESET_GPIO_RCC_CLOCK_EN				  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define  M8266WIFI_nRESET_GPIO											GPIOB
#define  M8266WIFI_nRESET_PIN												GPIO_Pin_14
#elif defined(MCU_IS_STM32F7XX)  // STM's STM32F7XX, using GPIOE3 as nRESET
#define  M8266WIFI_nRESET_GPIO_RCC_CLOCK_EN				  __HAL_RCC_GPIOE_CLK_ENABLE()
#define  M8266WIFI_nRESET_GPIO											GPIOE
#define  M8266WIFI_nRESET_PIN												GPIO_PIN_3
#define  M8266WIFI_nRESET_RCC_Periph_GPIO	  RCC_AHB1Periph_GPIOB
#elif defined(MCU_IS_NXP_LPC17XX) // NXP's LPC17XX, using GPIO[2,7] as nRESET
#define  M8266WIFI_nRESET_GPIO											LPC_GPIO2
#define  M8266WIFI_nRESET_PIN												7
#elif defined(MCU_IS_HT32F16XX)   // Holtek's HT32F16XX, using GPIOA2 as nRESET
#define  M8266WIFI_nRESET_GPIO_CLK_BIT							CKCUClock.Bit.PA
#define  M8266WIFI_nRESET_GPIO											HT_GPIOA
#define  M8266WIFI_nRESET_PIN												GPIO_PIN_2
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

#if   defined(MCU_IS_STM32F1XX)  // STM's STM32F1XX, using GPIOA4 as nCS
#define  M8266WIFI_SPI_nCS_GPIO_RCC_CLOCK_EN				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define  M8266WIFI_SPI_nCS_GPIO											GPIOA
#define  M8266WIFI_SPI_nCS_PIN											GPIO_Pin_4
#elif defined(MCU_IS_STM32F4XX)  // STM's STM32F4XX, using GPIOA4 as nCS
#define  M8266WIFI_SPI_nCS_GPIO_RCC_CLOCK_EN				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define  M8266WIFI_SPI_nCS_GPIO											GPIOE
#define  M8266WIFI_SPI_nCS_PIN											GPIO_Pin_1
#elif defined(MCU_IS_STM32F7XX)  // STM's STM32F7XX, using GPIOE4 as nCS
#define  M8266WIFI_SPI_nCS_GPIO_RCC_CLOCK_EN				__HAL_RCC_GPIOE_CLK_ENABLE()
#define  M8266WIFI_SPI_nCS_GPIO											GPIOE
#define  M8266WIFI_SPI_nCS_PIN											GPIO_PIN_4 
#elif defined(MCU_IS_NXP_LPC17XX) // NXP's LPC17XX, using GPIO[0,16] as nCS
#define  M8266WIFI_SPI_nCS_GPIO											LPC_GPIO0
#define  M8266WIFI_SPI_nCS_PIN											16
#elif defined(MCU_IS_HT32F16XX)   // Holtek's HT32F16XX, using GPIOA3 as nCS
#define  M8266WIFI_SPI_nCS_GPIO_CLK_BIT							CKCUClock.Bit.PA
#define  M8266WIFI_SPI_nCS_GPIO											HT_GPIOA
#define  M8266WIFI_SPI_nCS_PIN											GPIO_PIN_3
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// M8266WIFI Module Interface Board Usages Macros
//     - MOSI - MISO - CLK
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//      PIN   GND - nRST - nCS - MOSI - MISO - CLK   
//For STM32
//- If uses SPI1, SPI1_MOSI=GPIOA7,  SPI1_MISO=GPIOA6,  SPI1_SCK=GPIOA5   -- use this currently（为了和摄像头兼容，这个版本用的都是B4）
//            or  SPI1_MOSI=GPIOB5,  SPI1_MISO=GPIOB4,  SPI1_SCK=GPIOB3                 
//- If uses SPI3, SPI3_MOSI=GPIOC12, SPI3_MISO=GPIOC11, SPI3_SCK=GPIOC10  -- use this currently
//            or  SPI1_MOSI=GPIOB5,  SPI1_MISO=GPIOB4,  SPI1_SCK=GPIOB3,                  
//For LPX17xx
//- If uses SPI0, SPI0_MOSI=P0.18,  SPI0_MISO=P0.17,  SPI0_SCK=P0.15      -- use this currently
//For HT32F16xx
//- If uses SPI0, SPI0_MOSI=PA5,  SPI0_MISO=PA6,  SPI0_SCK=PA4            -- use this currently
//            or  SPI0_MOSI=PD11, SPI0_MISO=PD12, SPI0_SCK=PD10
//- If uses SPI1, SPI1_MOSI=PE10, SPI1_MISO=PE11, SPI1_SCK=PE9
//            or  SPI1_MOSI=PC11, SPI1_MISO=PC12, SPI1_SCK=PC10
#define  M8266WIFI_SPI_INTERFACE_NO							1

//- If uses USART1, USART1_TXD=PA9, USART1_RXD=PA10
//- If uses USART2, USART2_TXD=PA2, USART2_RXD=PA3   
#define  M8266WIFI_USART_INTERFACE_NO						1 // 1 for USART1, 2 for USART2
#define  M8266WIFI_USART_RX_BUF_SIZE						512

#define  USART_FOR_PRINTF												USART1

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// M8266WIFI Module Interface Board Usages Macros
//     - SPI TX DMA   if DMA used 
//     - SPI RX DMA   if DMA used
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
#ifdef M8266WIFI_SPI_ACCESS_USE_DMA
#define M8266WIFI_INTERFACE_SPI_TX_DMA										DMA2
#define M8266WIFI_INTERFACE_SPI_TX_RCC_Periph_DMA 				RCC_AHB1Periph_DMA2
#define M8266WIFI_INTERFACE_SPI_TX_DMA_CHANNEL 						DMA_Channel_3
#define M8266WIFI_INTERFACE_SPI_TX_DMA_STREAM							DMA2_Stream3
#define M8266WIFI_INTERFACE_SPI_RX_DMA										DMA2
#define M8266WIFI_INTERFACE_SPI_RX_RCC_Periph_DMA 				RCC_AHB1Periph_DMA2
#define M8266WIFI_INTERFACE_SPI_RX_DMA_CHANNEL 						DMA_Channel_3
#define M8266WIFI_INTERFACE_SPI_RX_DMA_STREAM							DMA2_Stream2
#endif

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// PLEASE DO NOT MODIFY BELOW UNLESS NECESSITY
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#if defined(MCU_IS_STM32F1XX) || defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32F7XX)
#define MCU_IS_STM32
#endif

//////////////////////////////////////////////////////////////////////////////////
#if   defined(MCU_IS_STM32F1XX)  // STM's STM32F1XX, 
#define GPIO_InitStructure_AS_GPIO_OUTPUT(gpio,pin)						\
	GPIO_InitStructure.GPIO_Pin 	= pin;												\
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						\
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						\
	GPIO_Init(gpio, &GPIO_InitStructure)	
#elif defined(MCU_IS_STM32F4XX)  // STM's STM32F7XX,  
#define GPIO_InitStructure_AS_GPIO_OUTPUT(gpio,pin)						\
	GPIO_InitStructure.GPIO_Pin 	= pin;												\
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;							\
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							\
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					\
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;								\
	GPIO_Init(gpio, &GPIO_InitStructure)	
#elif defined(MCU_IS_STM32F7XX)  // STM's STM32F7XX,  
#define GPIO_InitStructure_AS_GPIO_OUTPUT(gpio,pin)						\
	GPIO_InitStructure.Pin 				= pin;												\
	GPIO_InitStructure.Mode  			= GPIO_MODE_OUTPUT_PP;				\
	GPIO_InitStructure.Speed 			= GPIO_SPEED_HIGH;						\
	GPIO_InitStructure.Pull  			= GPIO_NOPULL;								\
	HAL_GPIO_Init(gpio,&GPIO_InitStructure)
#elif defined(MCU_IS_NXP_LPC17XX)//NXP's LPC17XX
#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

#if   defined(MCU_IS_STM32F1XX)  // STM's STM32F1XX, 
#define GPIO_OUTPUT_HIGH(gpio,pin)	(gpio)->BSRR=pin
#define GPIO_OUTPUT_LOW(gpio,pin)		(gpio)->BRR=pin
#elif   defined(MCU_IS_STM32F4XX)  // STM's STM32F4XX, 
#define GPIO_OUTPUT_HIGH(gpio,pin)	(gpio)->BSRRL=pin
#define GPIO_OUTPUT_LOW(gpio,pin)		(gpio)->BSRRH=pin
#elif   defined(MCU_IS_STM32F7XX)  // STM's STM32F7XX, 
#define GPIO_OUTPUT_HIGH(gpio,pin)	(gpio)->BSRR=pin  				// HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET)
#define GPIO_OUTPUT_LOW(gpio,pin)		(gpio)->BSRR=(pin<<16)  	// HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET)
#elif defined(MCU_IS_NXP_LPC17XX)//NXP's LPC17XX
#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

#if   defined(MCU_IS_STM32F1XX)
#define GPIO_InitStructure_AS_AF(gpio,pins)										\
	GPIO_InitStructure.GPIO_Pin 	= pins;												\
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;						\
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						\
	GPIO_Init(gpio, &GPIO_InitStructure)	
#elif   defined(MCU_IS_STM32F4XX)
#define GPIO_InitStructure_AS_AF(gpio,pins)										\
	GPIO_InitStructure.GPIO_Pin 	= pins;												\
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;								\
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							\
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					\
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;								\
	GPIO_Init(gpio, &GPIO_InitStructure)	
#elif   defined(MCU_IS_STM32F7XX)
#define GPIO_InitStructure_AS_AF(gpio,pins)										\
	GPIO_InitStructure.Pin       = pins;												\
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;							\
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;							\
	GPIO_InitStructure.Pull      = GPIO_PULLUP;									\
  GPIO_InitStructure.Alternate = 0x05U;												\
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure)
#elif defined(MCU_IS_NXP_LPC17XX)//NXP's LPC17XX
#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

//To define GPIO_PinRemapSPI0
#if   defined(MCU_IS_STM32F1XX)
#elif defined(MCU_IS_STM32F4XX)
#elif defined(MCU_IS_STM32F7XX)
#elif defined(MCU_IS_NXP_LPC17XX)
#define GPIO_PinRemapSPI0																																										\
						LPC_PINCON->PINSEL0 &= ~(0x03UL << 30); LPC_PINCON->PINSEL0 |=  (0x02UL << 30);									\
						LPC_PINCON->PINSEL1 &= ~(0x03UL <<  2); LPC_PINCON->PINSEL1 |=  (0x02UL <<  2);									\
						LPC_PINCON->PINSEL1 &= ~(0x03UL <<  4); LPC_PINCON->PINSEL1 |=  (0x02UL <<  4);									\
						LPC_PINCON->PINSEL1 &= ~(0x03UL << 0);	
#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

//To define GPIO_PinRemapSPI1
#if   defined(MCU_IS_STM32F1XX)
#define GPIO_PinRemapSPI1																																	\
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
#elif defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32F7XX)
#define GPIO_PinRemapSPI1																																	\
  GPIO_PinAFConfig(M8266WIFI_SPI_GPIO,M8266WIFI_SPI_MOSI_GPIO_PinSource,GPIO_AF_SPI1);		\
	GPIO_PinAFConfig(M8266WIFI_SPI_GPIO,M8266WIFI_SPI_MISO_GPIO_PinSource,GPIO_AF_SPI1); 		\
	GPIO_PinAFConfig(M8266WIFI_SPI_GPIO,M8266WIFI_SPI_CLK_GPIO_PinSource,GPIO_AF_SPI1); 
#elif defined(MCU_IS_NXP_LPC17XX)//NXP LPC17XX
#define GPIO_PinRemapSPI1																																		\
						LPC_PINCON->PINSEL0 &= ~(0x03UL << 14); LPC_PINCON->PINSEL0 |=  (0x02UL << 14);	\
						LPC_PINCON->PINSEL0 &= ~(0x03UL << 16); LPC_PINCON->PINSEL0 |=  (0x02UL << 16);	\
						LPC_PINCON->PINSEL0 &= ~(0x03UL << 18); LPC_PINCON->PINSEL0 |=  (0x02UL << 18);	\
						LPC_PINCON->PINSEL0 &= ~(0x03UL << 12);	
#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

//To define GPIO_PinRemapSPI3
#if   defined(MCU_IS_STM32F1XX)
#define GPIO_PinRemapSPI3																				\
	GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);
#elif defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32F7XX)
#define GPIO_PinRemapSPI3																				\
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI3);					\
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI3); 				\
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI3);
#elif defined(MCU_IS_NXP_LPC17XX)//NXP's LPC17XX
#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

#if   defined(MCU_IS_STM32F1XX)
#define GPIO_PinRemapUSART1																				\
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
#elif defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32F7XX)
#define GPIO_PinRemapUSART1																				\
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1); 			\
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
#elif defined(MCU_IS_NXP_LPC17XX)//NXP's LPC17XX
#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

#if   defined(MCU_IS_STM32F1XX)
#define GPIO_PinRemapUSART2																				\
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#elif defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32F7XX)
#define GPIO_PinRemapUSART2																				\
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2); 			\
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_USART2);
#elif defined(MCU_IS_NXP_LPC17XX)//NXP's LPC17XX
#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif


#if   defined(MCU_IS_STM32)  // for STM32
#if   (M8266WIFI_SPI_INTERFACE_NO == 1)
  #define  M8266WIFI_INTERFACE_SPI								SPI1
#elif (M8266WIFI_SPI_INTERFACE_NO == 2)
  #define  M8266WIFI_INTERFACE_SPI								SPI2
#elif (M8266WIFI_SPI_INTERFACE_NO == 3)
  #define  M8266WIFI_INTERFACE_SPI								SPI3
#elif (M8266WIFI_SPI_INTERFACE_NO == 4)
  #define  M8266WIFI_INTERFACE_SPI								SPI4
#else
#error Wrong M8266WIFI_SPI_INTERFACE_NO defined near line 104 for STM32 in brd_cfg.h
#endif

#elif defined(MCU_IS_NXP_LPC17XX) // for LPC17XX
#if   (M8266WIFI_SPI_INTERFACE_NO == 0)
  #define  M8266WIFI_INTERFACE_SPI								LPC_SSP0
	#define  LPC_SC_PCON_SSP_BIT										21				// Power Enable bit of SSP0
	#define  LPC_SC_PCLKSELx_FOR_SSP								PCLKSEL1	// PCLK Selection for SSP0
	#define  LPC_SC_PCLKSELx_FOR_SSP_MASK_BIT				10
#elif (M8266WIFI_SPI_INTERFACE_NO == 1)
  #define  M8266WIFI_INTERFACE_SPI								LPC_SSP1
	#define  LPC_SC_PCON_SSP_BIT										10				// Power Enable bit of SSP1
	#define  LPC_SC_PCLKSELx_FOR_SSP								PCLKSEL0	// PCLK Selection for SSP1
	#define  LPC_SC_PCLKSELx_FOR_SSP_MASK_BIT				20
#else
#error Wrong M8266WIFI_SPI_INTERFACE_NO defined near line 104 for NXP_LPC17XX in brd_cfg.h
#endif

#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX
#if   (M8266WIFI_SPI_INTERFACE_NO == 0)
  #define  M8266WIFI_INTERFACE_SPI								HT_SPI0
#elif (M8266WIFI_SPI_INTERFACE_NO == 1)
  #define  M8266WIFI_INTERFACE_SPI								HT_SPI1
#else
#error Wrong M8266WIFI_SPI_INTERFACE_NO defined near line 104 for Holtek's HT32F16XX in brd_cfg.h
#endif

#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif

#if   defined(MCU_IS_STM32)  // for STM32
#if   (M8266WIFI_USART_INTERFACE_NO == 1)
#define  M8266WIFI_INTERFACE_USART							USART1
#define  M8266WIFI_INTERFACE_USART_IRQn					USART1_IRQn
#elif (M8266WIFI_USART_INTERFACE_NO == 2)
#define  M8266WIFI_INTERFACE_USART							USART2
#define  M8266WIFI_INTERFACE_USART_IRQn					USART2_IRQn
#else
#error Wrong M8266WIFI_SPI_INTERFACE_NO defined near line 90 for STM32 in brd_cfg.h
#endif

#elif defined(MCU_IS_NXP_LPC17XX) // for LPC17XX

#elif defined(MCU_IS_HT32F16XX)  //Holtek's HT32F16XX

#else
#error UNSUPPORTED USARTx defined in brd_cfg.h
#endif

//Other board related Macros

#if defined(MCU_IS_STM32F4XX)
#if 1  // for DISCOVERY STM32F4 Board
#define RCC_AHB1Periph_GPIO_LEDS								RCC_AHB1Periph_GPIOF
#define LED_GPIOS															  GPIOF
#define LED0_GPIO_PIN													  GPIO_Pin_9
#define LED1_GPIO_PIN													  GPIO_Pin_10
#else // for a customer's Board
#define RCC_AHB1Periph_GPIO_LEDS								RCC_AHB1Periph_GPIOC
#define LED_GPIOS															  GPIOC
#define LED0_GPIO_PIN													  GPIO_Pin_0
#define LED1_GPIO_PIN													  GPIO_Pin_1
//#define LED2_GPIO_PIN													  GPIO_Pin_2
//#define LED3_GPIO_PIN													  GPIO_Pin_3
#endif
#if 1 // for DISCOVERY STM32F4 Board
#define RCC_AHB1Periph_GPIO_KEYS								RCC_AHB1Periph_GPIOE
#define KEY_GPIOS															  GPIOE
#define KEY0_GPIO_PIN														GPIO_Pin_4
#define KEY1_GPIO_PIN 													GPIO_Pin_3
#define KEY2_GPIO_PIN														GPIO_Pin_2
#else // for a customer's Board
#define RCC_AHB1Periph_GPIO_KEYS								RCC_AHB1Periph_GPIOB
#define KEY_GPIOS																GPIOB
#define KEY0_GPIO_PIN														GPIO_Pin_9
#define KEY1_GPIO_PIN														GPIO_Pin_9
#define KEY2_GPIO_PIN														GPIO_Pin_9
#endif
#endif

#endif
