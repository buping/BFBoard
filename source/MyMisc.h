
#ifndef		_MISC_H
#define		_MISC_H


#include    "Common.h"

#define ADD_SUM	1
#define XOR_SUM 2
//D13
#define TEST_PULS

void Delay_us(INT32U Count);
void Delay_ms(INT32U Count);
BYTE	CalculateChekSum(BYTE *Data,BYTE Lenth,BYTE Method);
void	IO_NOT(void);
#endif

