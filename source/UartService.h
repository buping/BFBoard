/*


*/
#ifndef _UART_SERVICE_H
#define _UART_SERVICE_H

#include "Common.h"
#include "string.h"
#include "uart.h"

//UART�ն�ID����
#define UART1 	1
#define UART2	2
#define UART3 	3
#define UART4	4
#define UART5 	5
//UARTʹ��
#define UART1_EN	1	//ʹ��UARTA
#define UART2_EN	1	//ʹ��UARTA
#define UART3_EN	1	//ʹ��UARTA
#define UART4_EN	0	//ʹ��UARTA
#define UART5_EN	0	//ʹ��UARTA

#define	UARTRXBUFFLEN 60

//UART��������
#define UART1_TX_PERIOD 11
#define UART2_TX_PERIOD 3
#define UART3_TX_PERIOD 100
//���ϴ�Ӧ�ö��ԣ�UART���ճ���Ϊһ���ṹ��
typedef struct UartSource
{
	volatile INT8U	UartRxBuff[UARTRXBUFFLEN];  //���ջ�����
	volatile INT8U	UartRxLen;		   			//Uart�н��յ��ֽ���
	volatile INT8U	UartFlag;					//��ر�־
	volatile BOOL   UartTxEn;                   //������
    volatile INT8U	UartTxCnt;					//����ʱ�������
	INT32U	gUartTimeOutCnt;					//���ճ�ʱ������
	INT8U*  UartTx;	
	INT16U	UartTxTicks;						//����ʹ�õ�ʱ�ӽ���						
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
//Discription:UART��������
//Return Type:void
//Argument:INT8U UartId,BYTE* pData,INT8U Lenth
void UartSend(INT8U UartId,BYTE* pData,INT8U Lenth);

//FileName:UartRxISR
//Discription:UART��������
//Return Type:void
//Argument:INT8U UartId		
void UartRxISR(INT8U UartId);
#endif







