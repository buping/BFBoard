#ifndef _KEY_SERVICE_H
#define	_KEY_SERVICE_H

//#include "HardWare.h"
//#include "Common.h"
//#include "DataType.h"
extern volatile int gDx;
extern volatile int gDy;
extern unsigned int gState;

extern unsigned int gState;	//laser跟踪器发的指令信息
extern unsigned int gLaserFlag;  //laser跟踪器发出的指令，用于DSP发送控制
extern unsigned int gHostInstruct;  //伺服工作状态，默认电锁/搜索
extern int  Sx,Sy;		//搜索角度设置
extern int gThetax,gThetay;  //控制发给PC的框架角
extern unsigned int gZoom;  //视场大或小
/*
struct LaserFlag
{
	//unsigned	BhFlag:1;
	//unsigned	GdFlag:1;
	//unsigned	NqFlag:1;
	//unsigned
}LASER_FLAG;
*/

//框架角输入，输出标定
#define SET_FRAME 0 //框架角标定使能

typedef	struct FrameData
{
	float Kx;		//比例系数k
	float Ky;		//零位修正b
	int Bx;
	int By;
	int FrameAdcX[20];	//存放AD采样值
	int FrameAdcY[20];
	int FrameSourceX[20]; //存放输入的框架角
	int FrameSourceY[20];
	float Kxx[20];	//存储每两个点间的K
	float Kyy[20];	
}FRAME_DATA;

//gState MASK define
#define	BH	0x0002
#define GD	0x0001
#define INSIDE	0x0080
#define OUTSIDE	0x0040
#define OUTSIDE_XP	0x1000  //Z+
#define OUTSIDE_XN	0x8000  //Z-
#define OUTSIDE_YP	0x4000  //Y+
#define OUTSIDE_YN	0x2000	//Y-

//内、外区速度
//OUTSIDE SPEED
#define OUTSIDE_SPEED		400
#define ERROR_MAX		400
//OUT_LIMIT
#define	OUT_LIMIT	4000

//gHostInstruct MASK define
//产品工作状态
#define POWER_LOCK	0x0000 //电锁
#define STEADY		0x0001 //稳定
#define FOLLOW      0x0002 //跟踪
#define SEARCH		0x0004 //搜索
#define SET_ZOOM    0x0008 //视场调节
#define SYS_START	0x0010 //系统开始工作
#define SYS_STOP	0x0020 //系统停止工作

//搜索预置角最大值
#define THETAX_MAX  1200
#define THETAY_MAX  1200
//VIDEO AREA
#define VIDEO_SMALL_EN	   	{GPADAT &= ~BIT9;VIDEO_BIG_DIS}
#define VIDEO_SMALL_DIS		GPADAT |= BIT9;

#define VIDEO_BIG_EN	   	{GPADAT &= ~BIT10;VIDEO_SMALL_DIS}
#define VIDEO_BIG_DIS		GPADAT |= BIT10;
//DETECTOR POWER
#define		DETECTOR_POWER_ON	GPADAT &= ~BIT8;
#define		DETECTOR_POWER_OFF	GPADAT |= BIT8;	

extern void Search(void);
extern void POWERLOCK(float Kx[],float Ky[],int XE[],int YE[],int XU[],int YU[],int ad[],int GzData[]);
extern void STABILIZATION(float Kx[],float Ky[],int XE[],int YE[],int XU[],int YU[],int ad[],int GzData[]);
extern void FOLLOWING(float Kx[],float Ky[],int XE[],int YE[],int ad[],int GzData[]);
extern void GENZONG(float Kx[],float Ky[],int DXE[],int DYE[],int DXU[],int DYU[]);
void AdjustZoom(void);
//框架角标定
void CorrectFrame(void); 
#endif


