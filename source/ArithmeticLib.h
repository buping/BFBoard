#ifndef		_ARITHMETIC_LIB_H
	#define		_ARITHMETIC_LIB_H


#include	"DataType.h"

#define		DEGREE	4
#define     ALEN 	2
//FUZZY CONTROL
#define		NB	-500
#define		NM	-400
#define		NS	-300
#define		ZO	0
#define		PS	300
#define		PM	400
#define		PB	500

typedef	struct Compensate
{		
	FP32	InputK[DEGREE];
	FP32	OutputK[DEGREE];
	FP32	K;
	FP32    voltageK;
	INT16S	InPut[DEGREE];
    INT16S	OutPut[DEGREE];
	INT16S	Temp;
}COMPENSATE;
	
typedef struct
{
	INT16S	CurInputX[DEGREE];
	INT16S  VolInputX[DEGREE];
	INT16S	PosInputX[DEGREE];

	INT16S	CurOutputX[DEGREE];
	INT16S  VolOutputX[DEGREE];
	INT16S	PosOutputX[DEGREE];

	INT16S	CurInputY[DEGREE];
	INT16S  VolInputY[DEGREE];
	INT16S	PosInputY[DEGREE];

	INT16S	CurOutputY[DEGREE];
	INT16S  VolOutputY[DEGREE];
	INT16S	PosOutputY[DEGREE];
}INOUT_VARIABLE;

extern INT16S 	Average(INT16S* pData,INT8U Len);
extern void		SelectOrder(INT16S* Data,INT8U Len);
extern INT16S 	Compensate(COMPENSATE* p);
extern INT16S   Limit(INT16S Data,INT16S Limit);
extern INT16S  increaseLimit(INT16S Data,INT16S Data1,INT16S Limit);
#endif
