

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

volatile INT16U  gSwitch = 0;  //���뿪��
volatile INT16U  gCarSleepTimer = 0;    //С������ʱ��
volatile MOTOR gMotor;

volatile SYS_FLAG0 gSysFlg0;	//ϵͳ��־
volatile TICKS gTicks;	//���ʱ�ӽ���

volatile INT8U gDeviceType = 0;  //�豸����
volatile FASH_DATA gFlashData;  //flash ������
volatile GOODS gGoods;         //����ID�洢

volatile INT16U gTriggerCount;
volatile INT16U gTriggerTime;

//*************************************
//uart���ݸ�ʽ
//*************************************




