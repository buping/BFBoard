#ifndef	_TYPE_DIFF_
	#define	_TYPE_DIFF_

//#include    "Common.h"

/*********************************/
//平台程序版本号,16进制
//@LEE[12/05/23]
//注意:版本号不能为0
//用户卡需要反写非0值
/*********************************/
#define PVERSION 		0x01
 
#define WATCHDOG_EN     (0)
#define TEST_PRODUCT_LINE (0)
#define PROGRAM_FLASH_EN  ( 1 )
#define	SOFTWARE_TEST	0
#define	HARDWARE_TEST	0
#define	DEBUG			0
#define INT_DEBUG		0
#define	RUN				0
#define MOTOR_COMM_UART_EN   (1)
#define PLC_MOTOR_UART_EN  ( 0 )
#define AD7328	1
#define OLED_LONGQIU 0
#define URAT_DMA_EN (1)
#define URAT_DMA_RX_EN (0)
#define GOODID_NO_CHECK_EN (0)  //不检测货物ID  使能
#define CHEEK_HALL_EXIT (0)
#define CHEEK_HALL_EN (0)

//**************************************
// device type define
//**************************************
#define DEVICE_CAR      2
#define DEVICE_ENTRY    3
#define DEVICE_EXIT     4
#define DEVICE_WASTER   5
#define DEVICE_MOTO     6
#define DEVICE_TRIGGER  7
#define DEVICE_REWRITE  8
#define DEVICE_BRIDGE   9

/*
#define PC_COMMAND_ID       1
#define CAR_COMMAND_ID      2
#define ENTRY_COMMAND_ID    3
#define EXIT_COMMAND_ID     4
#define WASTER_COMMAND_ID   5
#define MOTO_COMMAND_ID   6
*/

#define ENTRY_NO_MIN    600
#define ENTRY_NO_MAX    900

#define EXIT_NO_MIN    901
#define EXIT_NO_MAX    999

#define CAR_NO_MIN    0
#define CAR_NO_MAX    599

#define WASTER_NO_MIN    1020
#define WASTER_NO_MAX    1023


#define BRIDGE_NO_MIN  1000
#define BRIDGE_NO_MAX  1000


#define TRIGGER_NO_MIN 1005
#define TRIGGER_NO_MAX 1009

#define REWRITE_NO_MIN  1010
#define REWRITE_NO_MAX  1019


#endif

