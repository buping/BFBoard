

#include "Common.h"

volatile INT16U  hardWireReg;

volatile	INT32U	gTickCnt;	//clock ticks counter,ms 

volatile INT8U	gWorkState;

volatile  INT8U gEntryTxEn = 0;

volatile CAR  gCar;
volatile EXIT gExit;
volatile ENTRY  gEntry;
volatile WASTER gWaste;

volatile INT16U  gWorkStatus0 = 0;
volatile INT16U  gWorkStatus1 = 0;

volatile INT16U  gSwitch = 0;  //拨码开关
volatile INT16U  gCarSleepTimer = 0;    //小车休眠时间
volatile MOTOR gMotor;

volatile SYS_FLAG0 gSysFlg0;	//系统标志
volatile TICKS gTicks;	//软件时钟节拍

volatile INT8U gDeviceType = 0;  //设备类型
volatile FASH_DATA gFlashData;  //flash 数据区
volatile GOODS gGoods;         //货物ID存储

volatile INT16U gTriggerCount;
volatile INT16U gTriggerTime;

//*************************************
//uart数据格式
//*************************************




