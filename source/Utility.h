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
#define	INT32_BCDS_BUF_LEN	5 //32λ����ת��BCD,����Ҫ��BCD����������


//@lee[2012/07/10]
//�����ڴ����ݴ����ʽBIG ENDIAN ��LITTLE ENDIAN ��ת��
//����������
#define EXCHANGEVALUES(num1,num2)	{\
	BYTE ChangeBUF=num1;\
	num1= num2;\
	num2 = ChangeBUF;\
	}

typedef union	myLIT16S{
				INT16 va16;
				BYTE vas[2];
			}LIT16S;	//little endian��2�ֽڶ�����
			

// Function name	: BCD2HexDate
// Description	    :BCD��ʽ������ת����HEX��ʽ������
// Return type	: void
// Argument      : BYTE* from	��ת������/***bcd_data��Χ[0x99, 0]***/
// Argument      : BYTE* to	ת���������
// Argument      : BYTE len	���ڸ�ʽ�ַ�����
void BCD2HexDate(BYTE* from, BYTE* to, BYTE len);

// Function name	: Hex2BCDDate
// Description	    :HEX��ʽ������ת����BCD��ʽ������
// Return type	: void
// Argument      : BYTE* from	��ת������/***hex_data��Χ[0x63, 0]***/
// Argument      : BYTE* to	ת���������
// Argument      : BYTE len	���ڸ�ʽ�ַ�����
void Hex2BCDDate(BYTE* from, BYTE* to, BYTE len);

// Function name	: int32HexToBCD
// Description	    : 4���ֽ�16������תBCD����
// Argument         : INT32U  hex; hexΪ32λ����,���Ϊ����,��ȡ����ֵ
// Argument         : BYTE *pBCDs; pBCDsΪ�����׵�ַ,��С��5byte��������ܵ����ڴ�Խ��
//				��λBCD������ǰ����
// Return type		: INT16 �ɹ���ʧ��ԭ��
INT16 int32HexToBCD(INT32U hex,BYTE *pBCDs);

// Function name	: int16HexToBCD
// Description	    : 2���ֽ�16������תBCD����
// Argument         : INT16U  hex; hexΪ16λ����,���Ϊ����,��ȡ����ֵ
// Argument         : BYTE *pBCDs; pBCDsΪ�����׵�ַ,����5��BCD���֣�ÿ������1���ֽڣ����治С��5byte��������ܵ����ڴ�Խ��
//				��λBCD������ǰ����
// Return type		: INT16 �ɹ���ʧ��ԭ��
INT16 int16HexToBCD(INT16U hex,BYTE *pBCDs);

// Function name	: BCD5AddBCD5
// Description	    : 5��BCD�����
// Argument         : BYTE* paddsource ���� ��λ��ǰ
// Argument         : BYTE *padddest ������������Ӻ�Ľ������λ�ɷ���ֵ����
// Return type		: INT16 ��߽�λ���ߴ�����
INT16 BCD5AddBCD5(BYTE* paddsource,BYTE *padddest);

// Function name	: little_BigConvert
// Description	    : little endian ��big endian����
// Return type		: void
// Argument         : BYTE len,����
// Argument         : BYTE* source	��ת�����ݵ�ַ
// Argument         : BYTE* dest	ת��������ݵ�ַ
void little_BigConvert(BYTE len, BYTE* dest, BYTE* source );

#endif

