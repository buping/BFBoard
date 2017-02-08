/************************************
FileName:spi.c//spi驱动函数
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/
#ifndef _UART_H
 #define _UART_H

#include "Common.h"
#define  RATE_115200   (115200)
#define  RATE_57600    ( 57600)
#define  RATE_38400    ( 38400)
#define  RATE_19200    ( 19200)

//
//USART1 DMA 声明
//
#define	USART1_DMA_TX_BUF_LEN	(25)
#define	USART1_DMA_RX_BUF_LEN	(25)

#define USART1_Tx_DMA_Channel      DMA1_Channel4
#define USART1_Tx_DMA_FLAG           ( DMA1_FLAG_GL4|DMA1_FLAG_TC4 | DMA1_FLAG_TE4 )
#define USART1_Tx_DMA_IRQ             DMA1_Channel4_IRQn
 
#define USART1_Rx_DMA_Channel      DMA1_Channel5
#define USART1_Rx_DMA_FLAG           (DMA1_FLAG_GL5|DMA1_FLAG_TC5 | DMA1_FLAG_TE5) 
#define USART1_Rx_DMA_IRQ             DMA1_Channel5_IRQn

#define GET_UART1_DMA_RX_LEN (USART1_Rx_DMA_Channel->CNDTR)

extern INT8U  USART1_DMA_TX_Buf[USART1_DMA_TX_BUF_LEN];
extern INT8U  USART1_DMA_RX_Buf[USART1_DMA_RX_BUF_LEN];
//
//USART2 DMA 声明
//

#define	USART2_DMA_TX_BUF_LEN	(25)
#define	USART2_DMA_RX_BUF_LEN	(25)

#define USART2_Tx_DMA_Channel      DMA1_Channel7
#define USART2_Tx_DMA_FLAG           ( DMA1_FLAG_GL7 | DMA1_FLAG_TC7 | DMA1_FLAG_TE7 )
#define USART2_Tx_DMA_IRQ             DMA1_Channel7_IRQn
 
#define USART2_Rx_DMA_Channel      DMA1_Channel6
#define USART2_Rx_DMA_FLAG           (DMA1_FLAG_GL6|DMA1_FLAG_TC6 | DMA1_FLAG_TE6) 
#define USART2_Rx_DMA_IRQ             DMA1_Channel6_IRQn
extern INT8U  USART2_DMA_TX_Buf[USART2_DMA_TX_BUF_LEN];
extern INT8U  USART2_DMA_RX_Buf[USART2_DMA_RX_BUF_LEN];
//
//
//


void initUart(void);
void initUartDMA(void);
void DMA_Uart_Init(INT8U usartNo);
void USART1_DMA_Tx(INT16U size);
void USART1_DMA_Tx_Over(void);
void USART1_DMA_Rx_Over(void);

void USART2_DMA_Tx(INT16U size);
void USART2_DMA_Tx_Over(void);
void USART2_DMA_Rx_Over(void);
#endif
