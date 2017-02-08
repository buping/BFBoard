/************************************
FileName: MyPrintf.h
Revision By: spg
Revised on 2012-08-01
Comments: Create
Innover   Confidential
************************************/

#ifndef _MY_PRINTF_H
#define _MY_PRINTF_H

//#include "sysConfig.h"
#include "Utility.h"
#include "UartService.h"

#define MY_DEBUG_UART UART0
#define MY_PRINT printString
typedef enum _PRINT_DATA_TYPE   //��ӡ���ݸ�ʽ
{
    PRINT_CHAR = 0,
    PRINT_INT16,
    PRINT_INT16U,
    PRINT_INT32,
    PRINT_INT32U,
    PRINT_X              //��16������ʾ
}PRINT_DATA_TYPE;

/*
#if(MY_DEBUG)
#define PRINT_ENTER() UartTx(MY_DEBUG_UART,"\n",1)    //��ӡ���з�
#define PRINT_STRING printString
#define PRINT_NUM printNum
#else
#define PRINT_ENTER()     //��ӡ���з�
#define PRINT_STRING 
#define PRINT_NUM 
#endif
*/

// FunctionName	: printString
// Description	: UART������ַ���
// Return type  : void
//Argument:BYTE *string
void printString(BYTE *string);

// FunctionName	: printNum
// Description	: UART�����ASII��ʮ���ƣ�Ҳ���ǰ��ַ����������
// Return type  : void
//Argument:BYTE :INT16U data,INT8U dataType
void printNum(INT32U data,PRINT_DATA_TYPE dataType);

#endif

