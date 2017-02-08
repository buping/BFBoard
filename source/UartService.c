/***********************************
File Name: UartService.c: Uart Receive and Send Realize
Revision By: Sunpg
Revised on 2013-04-25 
Comments: Create
Innover Confidential
************************************/
#include	"UartService.h"


#if	UART1_EN
UART_SOURCE gUart1;
#endif

#if	UART2_EN
UART_SOURCE gUart2;
#endif

#if	UART3_EN
UART_SOURCE gUart3;	
#endif


//FileName:UartSend
//Discription:UART发送数据
//Return Type:void
//Argument:INT8U UartId,BYTE* pData,INT8U Lenth
void UartSend(INT8U UartId,BYTE* pData,INT8U Lenth)
{
	volatile INT16U* UartTxBuff;		
	volatile INT16U* UartTxFull;
		
	switch(UartId)
	{
		case UART1: 
		{
#if UART1_EN
			UartTxBuff = UART_TX_BUFF1;
			UartTxFull = UART_TX_FULL1;
#endif
			break;
		}

		case UART2: 
		{
#if	UART2_EN
			UartTxBuff = UART_TX_BUFF2;
			UartTxFull = UART_TX_FULL2;
#endif
			break;
		}

		case UART3: 
		{
#if	UART3_EN
			UartTxBuff = UART_TX_BUFF3;
			UartTxFull = UART_TX_FULL3;
#endif
			break;
		}

		case UART4: 
		{
#if	UART4_EN
			UartTxBuff = UART_TX_BUFF4;
			UartTxFull = UART_TX_FULL4;
#endif
			break;
		}

		case UART5: 
		{
#if	UART4_EN
			UartTxBuff = UART_TX_BUFF5;
			UartTxFull = UART_TX_FULL5;
#endif
			break;
		}

		default: break;
	}

	
#if(URAT_DMA_EN)
		switch(UartId)
		{
			case UART1:
			{
			    memcpy((void*)USART1_DMA_TX_Buf,(void*)pData,Lenth);
				USART1_DMA_Tx(Lenth); break;
			}
			case UART2:
			{
			    memcpy((void*)USART2_DMA_TX_Buf,(void*)pData,Lenth);
				USART2_DMA_Tx(Lenth); break;
			}
			case UART3:
			{
			    //memcpy((void*)USART3_DMA_TX_Buf,(void*)pData,Lenth);
				//USART3_DMA_Tx(Lenth); break;
			}
			default: break;
        }
#else
		while(Lenth)  //数据是否发完
		{
	        while(!((*UartTxFull) & 0x80))	//发送BUFF或FIFO是否满
			{
				;
			}

			*UartTxBuff = *pData++;
			Lenth--;
		}
#endif

}

//FileName:UartRxISR
//Discription:UART接收数据
//Return Type:void
//Argument:INT8U UartId		
void UartRxISR(INT8U UartId)
{
	volatile INT16U* UartRxBuff;		
	volatile INT16U* UartRxEmpt;
	volatile BYTE*	UartReceive;
	volatile INT8U*	UartFlag;
	volatile INT8U*	Lenth;

	switch(UartId)
	{
		case UART1: 
		{
#if UART1_EN
			UartRxBuff = UART_RX_BUFF1;
			UartRxEmpt = UART_RX_EMPT1;
			UartReceive = &gUart1.UartRxBuff[0];
			UartFlag = &gUart1.UartFlag;
			Lenth = &gUart1.UartRxLen;
#endif
			break;
		}

		case UART2: 
		{
#if	UART2_EN
			UartRxBuff = UART_RX_BUFF2;
			UartRxEmpt = UART_RX_EMPT2;
			UartReceive = &gUart2.UartRxBuff[0];
			UartFlag = &gUart2.UartFlag;
			Lenth = &gUart2.UartRxLen;
#endif
			break;
		}

		case UART3: 
		{
#if	UART3_EN
			UartRxBuff = UART_RX_BUFF3;
			UartRxEmpt = UART_RX_EMPT3;
			UartReceive = &gUart3.UartRxBuff[0];
			UartFlag = &gUart3.UartFlag;
			Lenth = &gUart3.UartRxLen;
#endif
			break;
		}

		case UART4: 
		{
#if	UART4_EN
			UartRxBuff = UART_RX_BUFF4;
			UartRxEmpt = UART_RX_EMPT4;
			UartReceive = gUartRxBuff4;
			UartFlag = &gUartFlag4;
			Lenth = gUartRxLen4;
#endif
			break;
		}

		case UART5: 
		{
#if	UART5_EN
			UartRxBuff = UART_RX_BUFF5;
			UartRxEmpt = UART_RX_EMPT5; 
			UartReceive = gUartRxBuff5;
			UartFlag = &gUartFlag5;
			Lenth = gUartRxLen5;
#endif
			break;
		}

		default: break;
	}
#if(URAT_DMA_EN)
 
			 
      
#else
	//while(1)
	while((*UartRxEmpt & 0x1F00) >> 8)	//如果BUFF或FIFO非空，这个条件可能因硬件而已
	{
		if(*Lenth < UARTRXBUFFLEN)	//如果接收缓冲区未满
		{
			*(UartReceive + *Lenth) = *UartRxBuff;
			(*Lenth)++;
		}
		else
		{
			*UartFlag = TRUE;
			break;
		}
	}
#endif
}












