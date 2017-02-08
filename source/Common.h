//FileName:Common.h
//Description:Application global variable define
//argument:no
//return:no

#ifndef	_COMMON_H
#define _COMMON_H

#include "stm32f30x.h"
#include "stm32303c_eval.h"

#include "DataType.h"
#include "HAL_Mcu.h"
#include "SysConfig.h"
#include "spi.h"
#include "math.h"
#include "Misc.h"
#include "flash.h"
#include "Utility.h"

//Function Operation Return 
#define		SUCCEED		0
#define		FAILED		1
//BOOL 
#define		TRUE		1
#define		FALSE		0	

#define MOTOR_FOREWARD 0
#define MOTOR_V_BASE 127
#define LODER_OP_TIMER_BASE 127
#define UNLODER_OP_TIMER_BASE 127

extern  volatile INT16U  hardWireReg;
extern	volatile INT8U   gWorkModeCur;	//System Work Mode define
extern	volatile INT8U	gWorkModeNew;
extern	volatile INT32U	gTickCnt;	//clock ticks counter,ms 
extern  volatile BOOL	gRunOk;
extern	volatile BOOL	gMotoEn;
extern	volatile BYTE	gError;

extern	volatile INT8U	gWorkState;

extern	volatile INT16U  gSwitch;  //���뿪��
extern  volatile INT16U  gCarSleepTimer;    //С������ʱ��

extern  volatile INT8U gDeviceType;  //�豸����

extern  volatile INT16U  gWorkStatus0;
extern  volatile INT16U  gWorkStatus1;



//����ϵͳ��־
typedef union
{
	struct
	{
		INT16U TimerIntFlg:1;	//��ʱ���ж�
		INT16U SysStart:1;
		INT16U  BsEn:1;         //�������LS
		INT16U  sysDebugEn : 1;  //��������
	}Bits;
	INT16U All;
}SYS_FLAG0;

//ʱ�ӽ��Ķ���
typedef struct
{
	INT32U	carSleepTicks;
	INT8U  exitTxEnTimer;
    INT16U  led;
}TICKS;

//car
typedef struct
{
    INT16U  speed;
	INT8U   speedK;
	INT8U   lodeOpTimeK;
    INT8U   unLodeOpTimeK;
    INT8U  direction;
    BOOL    status;
    BOOL    motoStatus;
    BOOL    sleepEn;
    INT16U  dest;
    INT16U  id;
    INT16U  idFromEntry;
    INT16U  idFromExit;
    BYTE   fromEntryDis;
	BOOL   goodOk;
	BOOL   uartTxEn;
	INT16U uartTxCnt;
	BOOL   motorDirect;

}CAR;

//exit
typedef struct
{
    INT16U  id;
    BOOL    status; 
    BOOL   exitToPcEn;
    BOOL   inExitToCarEn;
    BOOL   outExitToCarEn;
    INT16U   uartToCarCnt;
    INT32U  goodId;
    INT32U  oldGoodId;
    INT8U  goodCnt;
}EXIT;


//entry
typedef struct
{
    BOOL goodsReady;
	BOOL carStatus; //���յ��ĵ��״̬Ҳ����С��״̬
	BOOL  goodsOk;
	BOOL  CarEnLoad;   
	INT16U  goodsReadyCnt;// С���ϻ��Ѿ���ɣ��ϼ��ڻظ�OK����ʱ��
	INT16U  goodsOkTimer;
	INT16U  goodsOkCnt; //���۵͵�ƽ��λ�źţ�����
	BOOL rxFromCarDis;
    BOOL rxFromPcDis;
	BOOL rxPcStatus;
	BOOL enteryToPcEn;
	BOOL enteryToCarEn;
	BOOL enPcTx;
	BOOL entryToCarOk;
	INT16U entryToPcCnt;   //��PC���������ʱ
}ENTRY;

//entry
typedef struct
{
    BOOL motorStatus; //���յ��ĵ��״̬
    BOOL carStatus;   //���ܵ���С��״̬
}WASTER;

typedef struct
{
    INT16U  id;
    BOOL    status;
    BOOL    opAlready;     //����Ѿ���������
    INT16U  speed;
	BOOL    loadDeyEn;
	BOOL    loadEn;
	BOOL    unLoadDeyEn;
	BOOL    unLoadEn;
    INT16U  loadDeyTime;   //װ��ǰ��ʱ����ʱ��
	INT16U  loadOpaTime;   //װ������ʱ��
	INT16U  unLoadDeyTime; //ж��ǰ��ʱ����ʱ��
	INT16U  unLoadOpaTime; //ж������ʱ��
	INT16U  loadDeyTicks;  //װ��ǰ��ʱ����ʱ��
	INT16U  loadOpaTicks;  //װ��ǰ��ʱ����ʱ��
	INT16U  unLoadDeyTicks;
	INT16U  unLoadOpaTicks;
    INT16U  hallPlusP;
	INT16U  hallPlusN;
	BOOL    hallStatusOld;
	BOOL    hallStatusNew;
	INT16U  hallPlusCnt;
	INT16U  hallJudgeTimer;  //���ʱ���¼
	BOOL    hallstatus;
	BOOL    hallJudgeEn;
  
	BOOL    opa;    //���ת��
	BOOL    Halldirection;                     //LJT����ת������
	BOOL    checkHallFlag;                  //LJT��������־λ
	
    
}MOTOR;

#define GOOGS_LEN (100)
typedef struct goods
{
	INT32U goodsId[GOOGS_LEN];
	INT8U  cnt;
	INT8U  exitDirect[GOOGS_LEN];
}GOODS;

typedef struct flash
{
  INT16U  flashWrEn;
  INT16U  carLoadDelay;
  INT16U  carLoadOpa;
  INT16U  carUnloadDelay;
  INT16U  carUnloadOpa;
  INT16U  carSpeed;  
  BYTE		carReTimerL;         //����ʱ�� 2�ֽڣ���λ100ms ; 5
  BYTE  	carReTimerH;                               // 6
  BYTE  	carUnLoadTimerL;              //�ٶ�     2�ֽ�      // 7
  BYTE  	carUnLoadTimerH;                                    // 8
}FASH_DATA;

extern volatile SYS_FLAG0 gSysFlg0;	//ϵͳ��־0
extern volatile TICKS gTicks;	//���ʱ�ӽ���
extern volatile CAR  gCar;
extern volatile EXIT gExit;
extern volatile MOTOR gMotor;
extern volatile ENTRY  gEntry;
extern volatile WASTER gWaste;
extern volatile GOODS gGoods;         //����ID �洢
extern volatile FASH_DATA gFlashData;  //flash ������
extern volatile  INT8U gEntryTxEn;

extern volatile INT16U gTriggerCount;   //���崥������
extern volatile INT16U gTriggerTime;    //���崥����ʱ��
#endif

