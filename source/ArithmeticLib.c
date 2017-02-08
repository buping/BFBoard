//FileName:ArithmeticLib.c
//Description:Usually Used Arithmetic Library Function
//Creation:2010.11.16
//Editor:GPS

#include 	"ArithmeticLib.h"


INT16S 	Compensate(COMPENSATE* p)
{
	INT32S Temp = 0;
	INT16U i = 0;

    for(i = 0;i < DEGREE;i++)
	{
		Temp += p->OutputK[i] * p->OutPut[i] + p->K * ( p->InputK[i] * p->InPut[i]); 
 	} 
	p->OutPut[0] = (INT16S)Temp;
	p->OutPut[0] = Limit(p->OutPut[0],p->Temp);
	for(i = DEGREE - 1; i > 0;i-- )
	{	
		p->InPut[i] = p->InPut[i - 1];
		p->OutPut[i] = p->OutPut[i - 1];
	}
	
 	return (Temp);	
}

void	SelectOrder(INT16S* Data,INT8U Len)
{
	INT8U i = 0,j = 0,k = 0;
	INT16S temp = 0;
	INT16S *pData;

	pData = Data;

	for(i = 0;i < Len - 1;i++)
	{
		k = i;
		for(j = i + 1; j < Len; j++)
		{
			if( *(pData + j) > *(pData + k) )
			{
				k = j;
			}
		}

		if(k != i)
		{
			temp = *(pData + k);
			*(pData + k) = *(pData + i);
			*(pData + i) = temp;
		}	
		
	}   
}

INT16S 	Average(INT16S* pData,INT8U Len)
{
	INT8U	i = 0,Temp;
	INT16S	Average = 0;
	
	Temp = Average >> 1;
	for(i = Temp - ALEN;i < Temp + ALEN; i++)
	{
		Average += *(pData + i);
	}
	Average = Average >> ALEN;
	return Average;  
}

INT16S  Limit(INT16S Data,INT16S Limit)
{
	INT16S data;

	data = Data;
	if(data >= Limit)
	{	
		data = Limit;
	}
	else if(data <= -Limit)
	{
		data = -Limit;
	}
	else
	{
		data = data;
	}
	return	data;
}

INT16S  increaseLimit(INT16S input,INT16S output,INT16S Limit)
{
	if(input - output >= Limit)
	{	
		output += Limit;
	}
	else if(input - output <= -Limit)
	{
		output -= Limit;
	}
	else
	{
		output = input;
	}
	return	output;
}

