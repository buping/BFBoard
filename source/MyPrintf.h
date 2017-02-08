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
typedef enum _PRINT_DATA_TYPE   //打印数据格式
{
    PRINT_CHAR = 0,
    PRINT_INT16,
    PRINT_INT16U,
    PRINT_INT32,
    PRINT_INT32U,
    PRINT_X              //按16进制显示
}PRINT_DATA_TYPE;

/*
#if(MY_DEBUG)
#define PRINT_ENTER() UartTx(MY_DEBUG_UART,"\n",1)    //打印换行符
#define PRINT_STRING printString
#define PRINT_NUM printNum
#else
#define PRINT_ENTER()     //打印换行符
#define PRINT_STRING 
#define PRINT_NUM 
#endif
*/

// FunctionName	: printString
// Description	: UART口输出字符串
// Return type  : void
//Argument:BYTE *string
void printString(BYTE *string);

// FunctionName	: printNum
// Description	: UART口输出ASII码十进制，也就是按字符串输出数据
// Return type  : void
//Argument:BYTE :INT16U data,INT8U dataType
void printNum(INT32U data,PRINT_DATA_TYPE dataType);

#endif

