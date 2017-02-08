
#ifndef		_APP_UART_H
	#define		_APP_UART_H

#include "Common.h"
#include "UartService.h"

#define UART1_TIMEOUT_CNT  (50)   	//uart1,Ӳ����ʱʱ�䳤
#define UART2_TIMEOUT_CNT  (2)   	//uart2,Ӳ����ʱʱ�䳤
#define UART3_TIMEOUT_CNT  (5) 		//uart3,Ӳ����ʱʱ�䳤




//***********************************
//******��������
//***********************************
#define	PC_TO_ENTRY		1	//��λ����ƽ̨
#define PC_TO_WASTER 	2	//��λ����С��
#define ENTRY_TO_CAR		3   //ƽ̨��С��
#define ENTRY_TO_PC		4   //ƽ̨��С��
#define CAR_TO_ENTRY	    5	//С����	ƽ̨	
#define CAR_TO_PLC	    6	//С����	PLC
#define PLC_TO_CAR	    7	//С����	PLC
#define CAR_TO_WASTER   8   //С����������
#define WASTER_TO_CAR   9   //�����ڵ�С��
#define WASTER_TO_PC   10  //�����ڵ�С��
#define EXIT_TO_CAR    11  //���ڵ�С��
#define EXIT_TO_PC     12  //���ڵ�PC ͳ��
#define CAR_TO_EXIT    13  //С�������� 

#define REWRITE_TO_CAR  14  //��д�ڵ�С��
#define TRIGGER_TO_PC  15   //�����嵽pc
#define REWRITE_TO_PC  16   //��д�ڵ�pc

//�豸ʶ���
#define PC_COMMAND_ID       1
#define CAR_COMMAND_ID      2
#define ENTRY_COMMAND_ID    3
#define EXIT_COMMAND_ID     4
#define WASTER_COMMAND_ID   5
#define MOTO_COMMAND_ID     6
#define EXIT_KEY_ID        7   //�˹���ť��

#define USER_COMMAND_ID   (0xBB)
//#define USER_TEST_

//UART�˿ڵ���ϵͳ��ӳ��
#define	PC_UART			UART1	//UART�˿ڳ�����PC��ARM������Ϣ�⣬������ISP����
#define ENTRY_PC_UART       UART1
#define EXIT_PC_UART     UART1
#define CAR_MOTOR_UART       UART1
#define EXIT_UART		UART2
#define ENTRY_CAR_UART       UART2
#define WASTER_CAR_UART       UART2
#define CAR_EXIT_UART		UART2

#define PLC_UART	    UART3

//********************************************
//************UART���ݰ�����****************
//********************************************
#define	FROM_PC_HEAD0 0xAA
#define	FROM_PC_HEAD1 0xCC
#define FROM_PC_HEAD2 0x06

typedef union 
{
    struct
    {
	    BYTE carStatus : 1; 
	    BYTE wasterStatus : 1;
	    BYTE pcStatus : 1;
	    BYTE entryStatus : 1;
	    BYTE exitStatus : 1;
	    BYTE formEntry : 1;
	    BYTE formExit : 1;
	    BYTE formwWaster : 1;
    }bits;
    struct
    {
        BYTE goodsFull : 1; 
	    BYTE exitUnlodeEn : 1;
	    BYTE printEn: 1;
	    BYTE exitUnlodeDis : 1;
	    BYTE exitStatus : 1;
	    BYTE formEntry : 1;
	    BYTE formExit : 1;
	    BYTE formwWaster : 1;
    }key; //�˹���ťָ��
    
	BYTE all;
}DEVICE_STATUS;
//
//LSK ���͵�����
//
#define LSK_RX_LEN 11
#define LSK_RX_HEAD0 0xAA
#define LSK_RX_HEAD1 0xaa
#define LSK_RX_HEAD2 0x12

#define CAR_ENTRY_DIR 	BIT1
#define CAR_EXIT_DIR 	BIT0
#define CAR_FWASTE_DIR 	BIT2
#define EXIT_IN_CTL      BIT3
#define EXIT_OUT_CTL      BIT4
#define EXIT_IN_OUT_DIRCTION BIT5

typedef struct
{
	BYTE inExitDirection : 1;
	BYTE inEntryDirection : 1;
	BYTE inWasteDirection : 1;
}CAR_DIRECTION;

typedef struct LskRx
{
	BYTE	  packetHead0;		//��ͷ    1�ֽ�     ��1
    BYTE      instructionId;       //ָ��ID  1�ֽ�    ��2
	BYTE      deviceIdL;            //�豸ID  2�ֽ�	   �� 3
    BYTE      deviceIdH;                              // 4
    BYTE      carReTimerL;         //����ʱ�� 2�ֽڣ���λ100ms ; 5
    BYTE      carReTimerH;                               // 6
    BYTE      carUnLoadTimerL;              //�ٶ�     2�ֽ�      // 7
    BYTE      carUnLoadTimerH;                                    // 8
    BYTE      direction;          //����                 //10
    DEVICE_STATUS      status;             //�豸״̬ 1�ֽ�        // 9
	BYTE	  checkSum;		// У���ֽ�  	1�ֽ�       //11
}LSK_RX;

//
//�û���������
//
#define LSK_SET_RX_LEN 13
#define LSK_RX_HEAD0 0xAA
#define LSK_RX_HEAD1 0xaa
#define LSK_RX_HEAD2 0x12
typedef struct 
{
	BYTE	  packetHead0;		//��ͷ    1�ֽ�     ��1
	BYTE	  instructionId;		//��ͷ    1�ֽ�     ��1
    BYTE  loadDeyTimeL;   //װ��ǰ��ʱ����ʱ��
    BYTE  loadDeyTimeH;
	BYTE  loadOpaTimeL;   //װ������ʱ��
	BYTE  loadOpaTimeH;
	BYTE  unLoadDeyTimeL; //ж��ǰ��ʱ����ʱ��
	BYTE  unLoadDeyTimeH;
	BYTE  unLoadOpaTimeL; //ж������ʱ��
	BYTE  unLoadOpaTimeH;
	BYTE  motorSpeedL;  //װ��ǰ��ʱ����ʱ��
	BYTE  motorSpeedH;  //װ��ǰ��ʱ����ʱ��
	BYTE	  checkSum;		// У���ֽ�  	1�ֽ�       //11
}LSK_RX_SET;
	
//
//LSK ���͵�����
//
#define LSK_TX_LEN 11
#define LSK_TX_HEAD0 0xAA
#define LSK_TX_HEAD1 0xaa
#define LSK_TX_HEAD2 0x12



typedef	struct LSK_TX
{
	BYTE	  packetHead0;		//��ͷ    1�ֽ�     ��1
    BYTE      instructionId;       //ָ��ID  1�ֽ�    ��2
	BYTE      deviceIdL;            //�豸ID  2�ֽ�	   �� 3
    BYTE      deviceIdH;                              // 4
    BYTE      carReTimerL;         //����ʱ�� 2�ֽڣ���λ100ms ; 5
    BYTE      carReTimerH;                               // 6
    BYTE      carUnLoadTimerL;              //�ٶ�     2�ֽ�      // 7
    BYTE      carUnLoadTimerH;                                    // 8
    BYTE      direction;          //����                 //10
    DEVICE_STATUS      status;             //�豸״̬ 1�ֽ�        // 9
	BYTE	  checkSum;		// У���ֽ�  	1�ֽ�       //11
}LSK_TX;

extern LSK_RX gCarRx,gPlatRx;
extern volatile LSK_RX gCarRxFromExit,gCarRxFromEntry,gCarRxFromWaster,gExitRxFromCar,gExitRxPC,gExitRxKey,
    gWasterRxFromCar,gEntryRxFormCar,gEntryRxFormPc,gEntryRxFormCar,
		gEntryRxFormPlc,gTriggerRxFromCar,gRewriteRxFromCar;

//FunctionName: UartFromPC
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
INT8U serchPackageHead(INT8U *p,INT8U packageHead,INT8U deviceId);

//FunctionName: UartFromExit
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromExit(INT8U UartId,UART_SOURCE *UartSource);

//FunctionName: UartFromEntry
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromEntry(INT8U UartId,UART_SOURCE *UartSource);

//FunctionName: UartFromCar
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromCar(INT8U UartId,UART_SOURCE *UartSource);

//FunctionName: UartFromWaster
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromWaster(INT8U UartId,UART_SOURCE *UartSource);

//FunctionName: UartFromPC
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromPC(INT8U UartId,UART_SOURCE *UartSource);

//FunctionName: CarToWaster
//Description:	
//Return Type:  void
//Argument:	void
void CarToWaster(void);

//FunctionName: CarToPlc
//Description:	
//Return Type:  void
//Argument:	void
void CarToPlc(void);

//FunctionName: CarToEntry
//Description:	
//Return Type:  void
//Argument:	void
void CarToEntry(void);

//FunctionName: CarToExit
//Description:	
//Return Type:  void
//Argument:	void
void CarToExit(void);
    
//FunctionName: PcToplat
//Description:	
//Return Type:  void
//Argument:	void
void PcToplat(void);

//FunctionName: entryToCar
//Description:	
//Return Type:  void
//Argument:	void
void entryToCar(void);

//FunctionName: ExitToCar;
//Description:	
//Return Type:  void
//Argument:	void
void ExitToCar(void);

//FunctionName: entryToPc
//Description:	
//Return Type:  void
//Argument:	void
void entryToPc(void);

//FunctionName: ExitToPc;
//Description:	
//Return Type:  void
//Argument:	void
void ExitToPc(void);

//FunctionName: wasterToCar
//Description:	
//Return Type:  void
//Argument:	void
void wasterToCar(void);

//FunctionName: wasterToPc
//Description:	
//Return Type:  void
//Argument:	void
void wasterToPc(void);

//FunctionName: UartFromPlc
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromPlc(INT8U UartId,UART_SOURCE *UartSource);

//FunctionName: UartFromKey
//Description:	�˹���ť���ƣ�ͨ��PC��ת��
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromKey(INT8U UartId,UART_SOURCE *UartSource);

//FunctionName: entryToPlc
//Description:	
//Return Type:  void
//Argument:	void
void entryToPlc(void);
//FunctionName: UartFromFpga
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromFpga(INT8U UartId,UART_SOURCE *UartSource);

//FunctionName: UartSendDada
//Description:	
//Return Type:  void
//Argument:	INT8U CommandType
void UartSendDada(INT8U CommandType);

//FunctionName: UartReadDada
//Description:	
//Return Type:  void
//Argument:	void
void UartReadDada(void);

//FunctionName: GetUartRx
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
BOOL GetUartRx(INT8U UartId,UART_SOURCE *UartSource);

//FunctionName: UartTimeout
//Description:	
//Return Type:  void
//Argument:	void
void UartTimeout(void);

#endif







