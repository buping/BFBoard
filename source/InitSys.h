#ifndef _INIT_SYS_H
#define _INIT_SYS_H

#include "HAL_Mcu.h"
#include "DataType.h"
#include "LQ-XS128-OledDrv.h"

#define TEST_SLEEP_TIMER  20
#define DEVICE_ID        600
#define TEST_SPEED       1000
#define FLASH_INNITION_DATA (10)
#define CAR_INIT_SPEED  (40000)
//FunctionName: InitSys
//Description:	Init System 
//Return Type:  void
//Argument:	void
void InitSys(void);

extern	void InitPeriphral(void);

//extern 	void InitSci(INT8U DataBitLen,INT8U StopBitLen,INT8U Parity,INT32U BaudRate);
extern	void InitSpi(void);
extern	void InitPwm(void);
extern  void InitInterrupt(void);
extern	void InitAdc(void);
void IO_Init(void);
//初始化全局变量
void InitRam(void);

//初始化FPGA
void InitFpga(void);


//FunctionName: memSet
//Description:	
//Return Type:  void
//Argument:	void
void memSet(void);

void initGpio(void);
#endif


