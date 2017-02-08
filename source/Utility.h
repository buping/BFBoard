/************************************
  File Name: Utility.h,
	Revision By:
	Revised on 2012.02.01
	Comments: Create
	Innover Confidential
************************************/

#ifndef _UTILITY_H
#define _UTILITY_H

#include "Common.h"
#define	INT32_BCDS_BUF_LEN	5 //32位整型转换BCD,必须要的BCD缓存区长度


//@lee[2012/07/10]
//用于内存数据存放形式BIG ENDIAN 与LITTLE ENDIAN 的转化
//交换两个数
#define EXCHANGEVALUES(num1,num2)	{\
	BYTE ChangeBUF=num1;\
	num1= num2;\
	num2 = ChangeBUF;\
	}

typedef union	myLIT16S{
				INT16 va16;
				BYTE vas[2];
			}LIT16S;	//little endian的2字节短整数
			

// Function name	: BCD2HexDate
// Description	    :BCD格式的日期转换成HEX格式的日期
// Return type	: void
// Argument      : BYTE* from	待转换日期/***bcd_data范围[0x99, 0]***/
// Argument      : BYTE* to	转换后的日期
// Argument      : BYTE len	日期格式字符长度
void BCD2HexDate(BYTE* from, BYTE* to, BYTE len);

// Function name	: Hex2BCDDate
// Description	    :HEX格式的日期转换成BCD格式的日期
// Return type	: void
// Argument      : BYTE* from	待转换日期/***hex_data范围[0x63, 0]***/
// Argument      : BYTE* to	转换后的日期
// Argument      : BYTE len	日期格式字符长度
void Hex2BCDDate(BYTE* from, BYTE* to, BYTE len);

// Function name	: int32HexToBCD
// Description	    : 4个字节16进制数转BCD代码
// Argument         : INT32U  hex; hex为32位整数,如果为负数,则取绝对值
// Argument         : BYTE *pBCDs; pBCDs为数组首地址,不小于5byte，否则可能导致内存越界
//				高位BCD数字在前返回
// Return type		: INT16 成功或失败原因
INT16 int32HexToBCD(INT32U hex,BYTE *pBCDs);

// Function name	: int16HexToBCD
// Description	    : 2个字节16进制数转BCD代码
// Argument         : INT16U  hex; hex为16位整数,如果为负数,则取绝对值
// Argument         : BYTE *pBCDs; pBCDs为数组首地址,返回5个BCD数字，每个数字1个字节，缓存不小于5byte，否则可能导致内存越界
//				高位BCD数字在前返回
// Return type		: INT16 成功或失败原因
INT16 int16HexToBCD(INT16U hex,BYTE *pBCDs);

// Function name	: BCD5AddBCD5
// Description	    : 5个BCD数相加
// Argument         : BYTE* paddsource 加数 高位在前
// Argument         : BYTE *padddest 被加数，保存加后的结果，进位由返回值带回
// Return type		: INT16 最高进位或者错误码
INT16 BCD5AddBCD5(BYTE* paddsource,BYTE *padddest);

// Function name	: little_BigConvert
// Description	    : little endian 和big endian互换
// Return type		: void
// Argument         : BYTE len,长度
// Argument         : BYTE* source	待转换数据地址
// Argument         : BYTE* dest	转换后的数据地址
void little_BigConvert(BYTE len, BYTE* dest, BYTE* source );

#endif

