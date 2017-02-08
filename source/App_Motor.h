#ifndef _APP_MOTOR_H
#define _APP_MOTOR_H

#include "Common.h"
#include "APP_Uart.h"

#define  HALL_OK_PULSE_MIN  (5)  // 电机旋转一次最少的脉冲个数
#define  HALL_JUDGE_TIMER  (500)

#define MOTOR_SET_INTERVAL (16)  //每条指令发送的间隔，需要根据波特率，字长，电机的技术参数设定

//****************************
//  1起始符 2地址       3命令  4数据位5校验位6 停止位0x05
//0x02              0x00 0x01     0xxx       0x  0x  0x
//****************************
#define UART_MOTOR_LEN 9
#define M_N_CHECK_LEN 3
#define MOTOR_START  0X02
#define MOTOR_ADDR1  0X00
#define MOTOR_ADDR2  0X01
#define MOTOR_END    0X05

//设置命令
#define MOTOR_V    0x80    //
#define MOTOR_INC_ACC    0x81    //
#define MOTOR_DEC_ACC    0x82    //
#define MOTOR_DIRECT  0X83   //
#define MOTOR_STATUS  0XA2   //
#define MOTOR_DISTANCE  0XA0   //
#define MOTOR_RUN  0X00   //
#define MOTOR_STOP   0X01   //
#define MOTOR_P 0X00   //  正转
#define MOTOR_N 0X01   //  反转

//查询电机命令
#define READ_SPEED 0X90
#define READ_CODER_NOBER  0X0E
 
typedef struct
{
	    BYTE start;    
	    BYTE address1;
	    BYTE address2;
	    BYTE command;
	    BYTE data0;
	    BYTE data1;
	    BYTE data2;
	    BYTE checkSum;
		BYTE end;	
}MOTOR_COMMAND;

// Function   name   :  setMotorIncAcc
// Description       : 
// Return     type   : void
// Argument         : INT32U incAccTime
void setMotorIncAcc(INT32U incAccTime);

// Function   name   :  setMotorDecAcc
// Description       : 
// Return     type   : void
// Argument         : INT32U incAccTime
void setMotorDecAcc(INT32U decAccTime);

// Function   name   :  setMotorDistance
// Description       : 
// Return     type   : void
// Argument         : INT32U distance
void setMotorDistance(INT32U distance,INT32U indirect);

// Function   name   :  motorRun
// Description       : 
// Return     type   : void
// Argument         : void
void motorRun(void);

// Function   name   :  motorRun
// Description       : 
// Return     type   : void
// Argument         : void
void motorStop(void);

// Function   name   :  setMotorDirect
// Description       : 
// Return     type   : void
// Argument         : BOOL indirect,INT32U speed
void setMotor(INT32U speed,INT32U decAccTime,INT32U incAccTime,BOOL indirect,INT32U distance);

// Function   name   :  setMotorDirect
// Description       : 
// Return     type   : void
// Argument         : void
void setMotorDirect(BOOL indirect);

// Function   name   :  setMotorSpeed
// Description       : 
// Return     type   : void
// Argument         : void
void setMotorSpeed(INT32U speed);

// Function   name   :  setMotorStatus
// Description       : 
// Return     type   : void
// Argument         : BOOL status
void setMotorStatus(BOOL status);

// Function   name   :  readMotorSpeed
// Description       : 
// Return     type   : void
// Argument         : BOOL readCommand
void readMotorSpeed(BOOL readCommand);
#endif
