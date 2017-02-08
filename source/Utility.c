/************************************
  File Name: Utility.c
	Revision By: Shi
	Revised on 2012.05.15
	Comments: Create
	Innover Confidential
************************************/

#include "Utility.h"
#include <string.h>

const  BYTE HPartUnit[5] = {6,5,5,3,6};  //˫�ֽڵ�λ0xffff+1


// Function name	: BCD2HexDate
// Description	    :BCD��ʽ������ת����HEX��ʽ������
// Return type	: void
// Argument      : BYTE* from	��ת������/***bcd_data��Χ[0x99, 0]***/
// Argument      : BYTE* to	ת���������
// Argument      : BYTE len	���ڸ�ʽ�ַ�����
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
// Description	    :HEX��ʽ������ת����BCD��ʽ������
// Return type	: void
// Argument      : BYTE* from	��ת������/***hex_data��Χ[0x63, 0]***/
// Argument      : BYTE* to	ת���������
// Argument      : BYTE len	���ڸ�ʽ�ַ�����
void Hex2BCDDate(BYTE* from, BYTE* to, BYTE len)
{
	BYTE bcd_data;
	BYTE temp;
	BYTE hex_data;
	BYTE loop;
	for(loop = 0; loop < len; loop++)
	{
		hex_data = from[loop];
		temp=hex_data%100;		//������λ

		bcd_data = (temp/10)<<4;		//ʮλ
		bcd_data |=(temp%10);	//��λ
		to[loop] = bcd_data;
	}
}

// Function name	: int32HexToBCD
// Description	    : 4���ֽ�16������תBCD����
// Argument         : INT32U  hex; hexΪ32λ����
// Argument         : BYTE *pBCDs; pBCDsΪ�����׵�ַ,����10��BCD���֣�2��BCD����1���ֽڣ����洢��5byte��
//				��λBCD������ǰ����
// Return type		: INT16 �ɹ���ʧ��ԭ��
INT16 int32HexToBCD(INT32U hex,BYTE *pBCDs)
{
	BYTE	idx = 0;	//     �±�
	BYTE	hidx = 0;	//     �±� 	
	BYTE	temp = 0;	//     �±� 	
	BYTE	BCDResultArr[10]={0};	//����ֵ
	
	INT16U	hexH = 0;
	BYTE	BCDResultH[5]={0};	//��2�ֽ�BCD��
	
	INT16U	hexL = 0;
	BYTE	BCDResultL[5]={0};	//�����ֽڶ�ӦBCD��
	INT16	ret = FAILED;

	if ( pBCDs == NULL)
	{
		//return ERR_NULL_POINTER;
	}
	
	//
	//�Ȱ�4Byte�����ֽ��2��˫Byte��(Hpart*65535+Lpart)�����ÿ����㷨�ֱ�תBCD
	//
	hexH = hex>>16;	//ȡ��λ
	hexL = (INT16U)(hex&0xffff);	//ȡ��λ
	
	int16HexToBCD(hexH, BCDResultH);
	int16HexToBCD(hexL, BCDResultL);	

	//�����Ͳ���
	for(; idx < 5 ; idx++)
	{
		BCDResultArr[idx+5] = BCDResultL[idx];	
	}
	
	//
	//HpartBCD(5λ) ʹ��65535���зֽ⣬���ֽڳ˷�����λ���λ��λ
	//
	for(hidx=5; hidx ; hidx--)
	{
		temp = BCDResultH[hidx-1];	//ȡֵ*65535
		for(idx=0; idx < 5 ; idx++)
		{
			BCDResultL[idx] = (BYTE)(temp*HPartUnit[idx]);	//tempΪbcd�룬���Բ��ᳬ��255
		}

		//����BCD����ӷ�����,ѡȡ�ʵ�ƫ��λ��
		ret = BCD5AddBCD5(BCDResultL, &BCDResultArr[hidx]);
		if( ret < SUCCEED)
		{
			return ret;
		}
		
		BCDResultArr[hidx-1] += (BYTE)ret;	//��λ��λ
	}
	
	//
	//���10��BCD,ѹ����5���ֽڷ���
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
// Description	    : 2���ֽ�16������תBCD����
//�����㷨�ο�: http://www.mcu123.com/news/Article/uc/uc8051/200803/4751.html
// Argument         : INT16U  hex; hexΪ16λ����,���Ϊ����,��ȡ����ֵ
// Argument         : BYTE *pBCDs; pBCDsΪ�����׵�ַ,����5��BCD���֣�ÿ������1���ֽڣ����治С��5byte��������ܵ����ڴ�Խ��
//				��λBCD������ǰ����
// Return type		: INT16 �ɹ���ʧ��ԭ��
INT16 int16HexToBCD(INT16U hex,BYTE *pBCDs)
{
	LIT16S    s16hex;	//���ֲ�����
	BYTE      thousand = 0; //���Ԥ����ǧλ

	if ( pBCDs == NULL)
	{
		//return ERR_NULL_POINTER;
	}
	
	s16hex.va16=hex;

#if(BIG_ENDIAN)	

	thousand = s16hex.vas[0]>>2;         //�ݴ�Ԥ����ǧλ,    ȡbit 15~bit10
	s16hex.vas[0] &= 0x03;         //��ȥ����b15~b10��ĸ�λbit9~bit8
	s16hex.va16 += thousand*24;           //bit15~bit10��1000�������
	
	if(s16hex.vas[0]>=4)         //��һ��У��ǧλ
	{                        //��������1024�����������ĸ��ֽ��ж�
		thousand++;
		s16hex.va16 -= 1000;
	}
#else	//little endianS

	thousand = s16hex.vas[1]>>2;         //�ݴ�Ԥ����ǧλ
	s16hex.vas[1 ]&= 0x03;         //��ȥ������Ԥ��ǧλ
	s16hex.va16 += thousand*24;           //������
	if(s16hex.vas[1] >= 4)         //��һ��У��ǧλ
	{                        //��������1024�����������ĸ��ֽ��ж�
		thousand++;
		s16hex.va16 -= 1000;
	}
#endif

	if(s16hex.va16 >= 1000)         //�ڶ���У��ǧλ
	{
		thousand++;
		s16hex.va16 -= 1000;
	}

	*pBCDs ++= thousand/10;             //ȡ��λ
	*pBCDs ++= thousand%10;             //ȡǧλ
	
	thousand = (BYTE)(s16hex.va16>>2)/25;    //ȡ�����İ�λ������thousand����
	*pBCDs ++= thousand;   //��λ
	
	thousand = s16hex.va16-thousand*100;
	*pBCDs ++= thousand/10;                          //ȡʮλ
	
	*pBCDs ++= thousand%10;                          //ȡ��λ

	return SUCCEED;	
}

// Function name	: BCD5AddBCD5
// Description	    : 5��BCD�����
// Argument         : BYTE* paddsource ���� ��λ��ǰ
// Argument         : BYTE *padddest ������������Ӻ�Ľ������λ�ɷ���ֵ����
// Return type		: INT16 ��߽�λ���ߴ�����
INT16 BCD5AddBCD5(BYTE* paddsource,BYTE *padddest)
{
	BYTE	idx = 0;	//     �±�
	BYTE	carry = 0;	//     ��λ
	
	if ( (paddsource == NULL) || (padddest == NULL) )
	{
		//return ERR_NULL_POINTER;
	}	

	for(idx=5; idx ; idx--)
	{
		padddest[idx-1] += carry;	//�ӽ�λ
		padddest[idx-1] += paddsource[idx-1]; //��������ֵ
		carry = padddest[idx-1]/10;	//���λ
		padddest[idx-1] %= 10;		//������
	}

	return (INT16)carry;
}

// Function name	: little_BigConvert
// Description	    : little endian ��big endian����
// Return type		: void
// Argument         : BYTE len,����
// Argument         : BYTE* source	��ת�����ݵ�ַ
// Argument         : BYTE* dest	ת��������ݵ�ַ
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

	//�ж�len�Ƿ�Ϊ����,�ҵ�ַ��ͬʱ
	if( ((len%2) != 0) &&
		(source == dest) )
	{
		//�����м�λ������ֵ
		dest[loop] = source[loop];
	}
}