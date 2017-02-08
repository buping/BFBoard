/************************************
  File Name: Utility.c
	Revision By: Shi
	Revised on 2012.05.15
	Comments: Create
	Innover Confidential
************************************/

#include "Utility.h"
#include <string.h>

const  BYTE HPartUnit[5] = {6,5,5,3,6};  //双字节单位0xffff+1


// Function name	: BCD2HexDate
// Description	    :BCD格式的日期转换成HEX格式的日期
// Return type	: void
// Argument      : BYTE* from	待转换日期/***bcd_data范围[0x99, 0]***/
// Argument      : BYTE* to	转换后的日期
// Argument      : BYTE len	日期格式字符长度
void BCD2HexDate(BYTE* from, BYTE* to, BYTE len)
{
	BYTE temp;
	BYTE bcd_data;
	BYTE loop;
	for(loop = 0; loop < len; loop++)
	{
		bcd_data = from[loop];
		temp=((bcd_data>>4)*10)+(bcd_data&0x0f);
		to[loop] = temp;
	}
}

// Function name	: Hex2BCDDate
// Description	    :HEX格式的日期转换成BCD格式的日期
// Return type	: void
// Argument      : BYTE* from	待转换日期/***hex_data范围[0x63, 0]***/
// Argument      : BYTE* to	转换后的日期
// Argument      : BYTE len	日期格式字符长度
void Hex2BCDDate(BYTE* from, BYTE* to, BYTE len)
{
	BYTE bcd_data;
	BYTE temp;
	BYTE hex_data;
	BYTE loop;
	for(loop = 0; loop < len; loop++)
	{
		hex_data = from[loop];
		temp=hex_data%100;		//丢弃百位

		bcd_data = (temp/10)<<4;		//十位
		bcd_data |=(temp%10);	//个位
		to[loop] = bcd_data;
	}
}

// Function name	: int32HexToBCD
// Description	    : 4个字节16进制数转BCD代码
// Argument         : INT32U  hex; hex为32位整数
// Argument         : BYTE *pBCDs; pBCDs为数组首地址,返回10个BCD数字，2个BCD数字1个字节，共存储于5byte中
//				高位BCD数字在前返回
// Return type		: INT16 成功或失败原因
INT16 int32HexToBCD(INT32U hex,BYTE *pBCDs)
{
	BYTE	idx = 0;	//     下标
	BYTE	hidx = 0;	//     下标 	
	BYTE	temp = 0;	//     下标 	
	BYTE	BCDResultArr[10]={0};	//返回值
	
	INT16U	hexH = 0;
	BYTE	BCDResultH[5]={0};	//高2字节BCD码
	
	INT16U	hexL = 0;
	BYTE	BCDResultL[5]={0};	//低两字节对应BCD码
	INT16	ret = FAILED;

	if ( pBCDs == NULL)
	{
		//return ERR_NULL_POINTER;
	}
	
	//
	//先把4Byte数，分解成2个双Byte数(Hpart*65535+Lpart)，采用快速算法分别转BCD
	//
	hexH = hex>>16;	//取高位
	hexL = (INT16U)(hex&0xffff);	//取低位
	
	int16HexToBCD(hexH, BCDResultH);
	int16HexToBCD(hexL, BCDResultL);	

	//拷贝低部分
	for(; idx < 5 ; idx++)
	{
		BCDResultArr[idx+5] = BCDResultL[idx];	
	}
	
	//
	//HpartBCD(5位) 使用65535进行分解，单字节乘法，低位向高位进位
	//
	for(hidx=5; hidx ; hidx--)
	{
		temp = BCDResultH[hidx-1];	//取值*65535
		for(idx=0; idx < 5 ; idx++)
		{
			BCDResultL[idx] = (BYTE)(temp*HPartUnit[idx]);	//temp为bcd码，所以不会超过255
		}

		//进行BCD数组加法运算,选取适当偏移位置
		ret = BCD5AddBCD5(BCDResultL, &BCDResultArr[hidx]);
		if( ret < SUCCEED)
		{
			return ret;
		}
		
		BCDResultArr[hidx-1] += (BYTE)ret;	//高位进位
	}
	
	//
	//获得10个BCD,压缩成5个字节返回
	//
	for(idx=0; idx < 5 ; idx++)
	{
		*pBCDs = BCDResultArr[idx*2]<<4;
		*pBCDs |= BCDResultArr[idx*2+1];
		pBCDs++;
	}
	
	return SUCCEED;
}

// Function name	: int16HexToBCD
// Description	    : 2个字节16进制数转BCD代码
//快速算法参考: http://www.mcu123.com/news/Article/uc/uc8051/200803/4751.html
// Argument         : INT16U  hex; hex为16位整数,如果为负数,则取绝对值
// Argument         : BYTE *pBCDs; pBCDs为数组首地址,返回5个BCD数字，每个数字1个字节，缓存不小于5byte，否则可能导致内存越界
//				高位BCD数字在前返回
// Return type		: INT16 成功或失败原因
INT16 int16HexToBCD(INT16U hex,BYTE *pBCDs)
{
	LIT16S    s16hex;	//待分拆数字
	BYTE      thousand = 0; //存放预估的千位

	if ( pBCDs == NULL)
	{
		//return ERR_NULL_POINTER;
	}
	
	s16hex.va16=hex;

#if(BIG_ENDIAN)	

	thousand = s16hex.vas[0]>>2;         //暂存预估的千位,    取bit 15~bit10
	s16hex.vas[0] &= 0x03;         //减去整数b15~b10后的高位bit9~bit8
	s16hex.va16 += thousand*24;           //bit15~bit10除1000后的余数
	
	if(s16hex.vas[0]>=4)         //第一次校正千位
	{                        //余数大于1024，利用余数的高字节判断
		thousand++;
		s16hex.va16 -= 1000;
	}
#else	//little endianS

	thousand = s16hex.vas[1]>>2;         //暂存预估的千位
	s16hex.vas[1 ]&= 0x03;         //减去整数中预估千位
	s16hex.va16 += thousand*24;           //求余数
	if(s16hex.vas[1] >= 4)         //第一次校正千位
	{                        //余数大于1024，利用余数的高字节判断
		thousand++;
		s16hex.va16 -= 1000;
	}
#endif

	if(s16hex.va16 >= 1000)         //第二次校正千位
	{
		thousand++;
		s16hex.va16 -= 1000;
	}

	*pBCDs ++= thousand/10;             //取万位
	*pBCDs ++= thousand%10;             //取千位
	
	thousand = (BYTE)(s16hex.va16>>2)/25;    //取余数的百位，借用thousand变量
	*pBCDs ++= thousand;   //百位
	
	thousand = s16hex.va16-thousand*100;
	*pBCDs ++= thousand/10;                          //取十位
	
	*pBCDs ++= thousand%10;                          //取个位

	return SUCCEED;	
}

// Function name	: BCD5AddBCD5
// Description	    : 5个BCD数相加
// Argument         : BYTE* paddsource 加数 高位在前
// Argument         : BYTE *padddest 被加数，保存加后的结果，进位由返回值带回
// Return type		: INT16 最高进位或者错误码
INT16 BCD5AddBCD5(BYTE* paddsource,BYTE *padddest)
{
	BYTE	idx = 0;	//     下标
	BYTE	carry = 0;	//     进位
	
	if ( (paddsource == NULL) || (padddest == NULL) )
	{
		//return ERR_NULL_POINTER;
	}	

	for(idx=5; idx ; idx--)
	{
		padddest[idx-1] += carry;	//加进位
		padddest[idx-1] += paddsource[idx-1]; //加新增加值
		carry = padddest[idx-1]/10;	//算进位
		padddest[idx-1] %= 10;		//求余数
	}

	return (INT16)carry;
}

// Function name	: little_BigConvert
// Description	    : little endian 和big endian互换
// Return type		: void
// Argument         : BYTE len,长度
// Argument         : BYTE* source	待转换数据地址
// Argument         : BYTE* dest	转换后的数据地址
void little_BigConvert(BYTE len, BYTE* dest, BYTE* source )
{
	BYTE index=0;
	BYTE loop= len>>1;
	BYTE temp;

	if(source != dest)
	{
		memcpy((void*)dest, (void*)source, len);
	}
	for(; index< loop ; index++)
	{
		temp = source[len-1-index];
		dest[len-1-index] = source[index];
		dest[index] = temp;
	}

	//判断len是否为基数,且地址相同时
	if( ((len%2) != 0) &&
		(source == dest) )
	{
		//基数中间位置数赋值
		dest[loop] = source[loop];
	}
}