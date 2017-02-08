/*


*/
#ifndef _UART_SERVICE_H
#define _UART_SERVICE_H

#include "Common.h"
#include "string.h"
#include "uart.h"

//UART终端ID定义
#define UART1 	1
#define UART2	2
#define UART3 	3
#define UART4	4
#define UART5 	5
//UART使能
#define UART1_EN	1	//使能UARTA
#define UART2_EN	1	//使能UARTA
#define UART3_EN	1	//使能UARTA
#define UART4_EN	0	//使能UARTA
#define UART5_EN	0	//使能UARTA

#define	UARTRXBUFFLEN 60

//UART发送周期
#define UART1_TX_PERIOD 11
#define UART2_TX_PERIOD 3
#define UART3_TX_PERIOD 100
//对上次应用而言，UART接收抽象为一个结构体
typedef struct UartSource
{
	volatile INT8U	UartRxBuff[UARTRXBUFFLEN];  //接收缓冲区
	volatile INT8U	UartRxLen;		   			//Uart中接收的字节数
	volatile INT8U	UartFlag;					//相关标志
	volatile BOOL   UartTxEn;                   //允许发送
    volatile INT8U	UartTxCnt;					//发送时间计数器
	INT32U	gUartTimeOutCnt;					//接收超时计数器
	INT8U*  UartTx;	
	INT16U	UartTxTicks;						//发送使用的时钟节拍						
}UART_SOURCE;

#if	UART1_EN
  extern UART_SOURCE gUart1;
#endif		

#if	UART2_EN
  extern UART_SOURCE gUart2;
#endif

#if	UART3_EN
  extern UART_SOURCE gUart3;
#endif

#if	UART4_EN
  extern UART_SOURCE gUart4;
#endif

#if	UART5_EN
  extern UART_SOURCE gUart5;
#endif



//#define SCITXBUF_A  SciaRegs.SCIRXBUF.all
//#define SCIRXBUF_A  SciaRegs.SCITXBUF.all
//#define SCIFFTX_A	SciaRegs.SCIFFTX.all
//#define SCIFFRX_A	SciaRegs.SCIFFRX.all

//FileName:UartSend
//Discription:UART发送数据
//Return Type:void
//Argument:INT8U UartId,BYTE* pData,INT8U Lenth
void UartSend(INT8U UartId,BYTE* pData,INT8U Lenth);

//FileName:UartRxISR
//Discription:UART接收数据
//Return Type:void
//Argument:INT8U UartId		
void UartRxISR(INT8U UartId);
#endif







