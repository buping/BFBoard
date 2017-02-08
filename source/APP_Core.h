#ifndef _APP_CORE_H
#define _APP_CORE_H

#include "Common.h"
#include "APP_Uart.h"
#include "APP_Switch.h"

//�ϼ�̨ʱ����
#define  ENTERY_LOAD_OK_TIMER0 	(100)    //�ϼ��ӵ�С���Ϻû���֪ͨPC


#define GOODS_STATUS_CHECK  40  //����������ʱ��
#define CAR_UART_CNT   (120)   //С����ȷ�ϼ���֪ͨ�ϼ�


#define CHECK_GOODS_TIME  40   //�ϼ����ۼ��ʱ��
#define HALL_PULS_MIN 10  //�жϵ����ת��Ҫ����С������
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

