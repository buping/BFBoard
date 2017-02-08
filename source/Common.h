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

extern	volatile INT16U  gSwitch;  //拨码开关
extern  volatile INT16U  gCarSleepTimer;    //小车休眠时间

extern  volatile INT8U gDeviceType;  //设备类型

extern  volatile INT16U  gWorkStatus0;
extern  volatile INT16U  gWorkStatus1;



//定义系统标志
typedef union
{
	struct
	{
		INT16U TimerIntFlg:1;	//定时器中断
		INT16U SysStart:1;
		INT16U  BsEn:1;         //允许接收LS
		INT16U  sysDebugEn : 1;  //测试允许
	}Bits;
	INT16U All;
}SYS_FLAG0;

//时钟节拍定义
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
	BOOL carStatus; //接收到的电机状态也就是小车状态
	BOOL  goodsOk;
	BOOL  CarEnLoad;   
	INT16U  goodsReadyCnt;// 小车上货已经完成，上件口回复OK持续时间
	INT16U  goodsOkTimer;
	INT16U  goodsOkCnt; //光眼低电平到位信号，计数
	BOOL rxFromCarDis;
    BOOL rxFromPcDis;
	BOOL rxPcStatus;
	BOOL enteryToPcEn;
	BOOL enteryToCarEn;
	BOOL enPcTx;
	BOOL entryToCarOk;
	INT16U entryToPcCnt;   //给PC发命令的延时
}ENTRY;

//entry
typedef struct
{
    BOOL motorStatus; //接收到的电机状态
    BOOL carStatus;   //接受到的小车状态
}WASTER;

typedef struct
{
    INT16U  id;
    BOOL    status;
    BOOL    opAlready;     //电机已经可是运行
    INT16U  speed;
	BOOL    loadDeyEn;
	BOOL    loadEn;
	BOOL    unLoadDeyEn;
	BOOL    unLoadEn;
    INT16U  loadDeyTime;   //装货前延时运行时间
	INT16U  loadOpaTime;   //装货运行时间
	INT16U  unLoadDeyTime; //卸货前延时运行时间
	INT16U  unLoadOpaTime; //卸货运行时间
	INT16U  loadDeyTicks;  //装货前延时运行时间
	INT16U  loadOpaTicks;  //装货前延时运行时间
	INT16U  unLoadDeyTicks;
	INT16U  unLoadOpaTicks;
    INT16U  hallPlusP;
	INT16U  hallPlusN;
	BOOL    hallStatusOld;
	BOOL    hallStatusNew;
	INT16U  hallPlusCnt;
	INT16U  hallJudgeTimer;  //检测时间记录
	BOOL    hallstatus;
	BOOL    hallJudgeEn;
  
	BOOL    opa;    //电机转动
	BOOL    Halldirection;                     //LJT霍尔转动方向
	BOOL    checkHallFlag;                  //LJT霍尔检测标志位
	
    
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
  BYTE		carReTimerL;         //休眠时间 2字节，单位100ms ; 5
  BYTE  	carReTimerH;                               // 6
  BYTE  	carUnLoadTimerL;              //速度     2字节      // 7
  BYTE  	carUnLoadTimerH;                                    // 8
}FASH_DATA;

extern volatile SYS_FLAG0 gSysFlg0;	//系统标志0
extern volatile TICKS gTicks;	//软件时钟节拍
extern volatile CAR  gCar;
extern volatile EXIT gExit;
extern volatile MOTOR gMotor;
extern volatile ENTRY  gEntry;
extern volatile WASTER gWaste;
extern volatile GOODS gGoods;         //货物ID 存储
extern volatile FASH_DATA gFlashData;  //flash 数据区
extern volatile  INT8U gEntryTxEn;

extern volatile INT16U gTriggerCount;   //脉冲触发计数
extern volatile INT16U gTriggerTime;    //脉冲触发总时间
#endif

