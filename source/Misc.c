#include	"Misc.h"

BYTE	CalculateChekSum(BYTE *Data,BYTE Lenth,BYTE Method)
{
	BYTE	CheckSum = 0,Count = 0;
 
	for(Count = 0;Count < Lenth;Count ++)
	{
		switch(Method)
		{
			case ADD_SUM:	//�����У���
			{ 
				 CheckSum += *(Data + Count);
				 break;
			}
			case XOR_SUM:	//�����У���
			{
				CheckSum ^= *(Data + Count);
				break;
			}				
			default: break;	
		}	
	}
	return	CheckSum;
}

void	IO_NOT(void)
{
	//ION ^= ION;
}

void Delay_us(INT32U Count)
{
	while(Count--)
	{
		//asm(" RPT #111 || NOP");	
		//CLRWD;
	}
}

void Delay_ms(INT32U Count)
{
	while(Count--)
	{
		//asm(" RPT #111 || NOP");
        Delay_us(9400);
        if((Count % 5) == 0)
        {
           CLRWD;
        }
	}
}

