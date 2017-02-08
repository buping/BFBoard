#ifndef _APP_CORE_H
#define _APP_CORE_H

#include "Common.h"
#include "APP_Uart.h"
#include "APP_Switch.h"

//上件台时序定义
#define  ENTERY_LOAD_OK_TIMER0 	(100)    //上件接到小车上好货后通知PC


#define GOODS_STATUS_CHECK  40  //霍尔检测持续时间
#define CAR_UART_CNT   (120)   //小车正确上件后通知上件


#define CHECK_GOODS_TIME  40   //上件光眼检测时间
#define HALL_PULS_MIN 10  //判断电机旋转需要的最小脉冲数
#define HALL_A_LOW		(0x02)
#define HALL_A_HIGHT     (0x01)
#define HALL_B_LOW		(0x01)
#define HALL_B_HIGHT     (0x02)
#define CHEEK_HALL_Forward      (0)
#define CHEEK_HALL_Reverse      (1)
// Function   name   :  taskSchdule
// Description       : 
// Return     type   : void
// Argument         : void
void taskSchdule(void);

// Function   name   :  checkEntryGoods
// Description       : 
// Return     type   : BOOL
// Argument         : void
BOOL checkEntryGoods(void);

// Function   name   :  checkMotorHall
// Description       : 
// Return     type   : BOOL
// Argument         : void
BOOL checkMotorHall(void);

// Function   name   :  uartSendPackage
// Description       : 
// Return     type   : void
// Argument         : void
void uartSendPackage(void);

// Function   name   :  displayLed
// Description       : 
// Return     type   : void
// Argument         : void
void displayLed(void);

// Function   name   :  motorCtl
// Description       : 
// Return     type   : void
// Argument         : void
void motorCtl(void);


void beginPulse(void);
void stopPulse(void);
void testPulse(void);

#endif

