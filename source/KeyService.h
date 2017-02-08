#ifndef _KEY_SERVICE_H
#define	_KEY_SERVICE_H

//#include "HardWare.h"
//#include "Common.h"
//#include "DataType.h"
extern volatile int gDx;
extern volatile int gDy;
extern unsigned int gState;

extern unsigned int gState;	//laser����������ָ����Ϣ
extern unsigned int gLaserFlag;  //laser������������ָ�����DSP���Ϳ���
extern unsigned int gHostInstruct;  //�ŷ�����״̬��Ĭ�ϵ���/����
extern int  Sx,Sy;		//�����Ƕ�����
extern int gThetax,gThetay;  //���Ʒ���PC�Ŀ�ܽ�
extern unsigned int gZoom;  //�ӳ����С
/*
struct LaserFlag
{
	//unsigned	BhFlag:1;
	//unsigned	GdFlag:1;
	//unsigned	NqFlag:1;
	//unsigned
}LASER_FLAG;
*/

//��ܽ����룬����궨
#define SET_FRAME 0 //��ܽǱ궨ʹ��

typedef	struct FrameData
{
	float Kx;		//����ϵ��k
	float Ky;		//��λ����b
	int Bx;
	int By;
	int FrameAdcX[20];	//���AD����ֵ
	int FrameAdcY[20];
	int FrameSourceX[20]; //�������Ŀ�ܽ�
	int FrameSourceY[20];
	float Kxx[20];	//�洢ÿ��������K
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

//�ڡ������ٶ�
//OUTSIDE SPEED
#define OUTSIDE_SPEED		400
#define ERROR_MAX		400
//OUT_LIMIT
#define	OUT_LIMIT	4000

//gHostInstruct MASK define
//��Ʒ����״̬
#define POWER_LOCK	0x0000 //����
#define STEADY		0x0001 //�ȶ�
#define FOLLOW      0x0002 //����
#define SEARCH		0x0004 //����
#define SET_ZOOM    0x0008 //�ӳ�����
#define SYS_START	0x0010 //ϵͳ��ʼ����
#define SYS_STOP	0x0020 //ϵͳֹͣ����

//����Ԥ�ý����ֵ
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
//��ܽǱ궨
void CorrectFrame(void); 
#endif


