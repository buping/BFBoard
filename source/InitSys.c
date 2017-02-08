#include "InitSys.h"
#include "UartService.h"
#include "uart.h"
#include "APP_Uart.h"
#include "pwm.h"
#include "adc.h"
#include "dac.h"
#include "spi.h"
#include "cap.h"
#include "exit.h"
//FunctionName: memSet
//Description:	
//Return Type:  void
//Argument:	void
void memSet(void)
{
  static INT16U chek,*data,fashData[10];
  //MOTOR motor;
  
  INT8U i;  
#if(TEST_URAT)
  
  LSK_TX uart1Test;
  INT32U i = 0;
  
  uart1Test.packetHead0 = LSK_TX_HEAD0;
  uart1Test.instructionId = PC_COMMAND_ID;
  uart1Test.deviceIdH = (DEVICE_ID & 0xff00) >> 8;
  uart1Test.deviceIdL = DEVICE_ID;
  uart1Test.sleepTimerH = (TEST_SLEEP_TIMER & 0xff00) >> 8;
  uart1Test.carUnLoadTimerL = TEST_SLEEP_TIMER;
  uart1Test.carReTimerH = (TEST_SPEED & 0xff00) >> 8;
  uart1Test.carReTimerL = TEST_SPEED;
  uart1Test.direction = 1;
  uart1Test.status.pcStatus = SUCCEED;
  
  uart1Test.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uart1Test.checkSum += *((BYTE*)&uart1Test + i);
	}
  
  for(i = 0;i < LSK_TX_LEN;i++)
	{
		gUart1.UartRxBuff[i] = *((BYTE*)&uart1Test + i);
	}
  
  uart1Test.instructionId = PC_TO_ENTRY;
  uart1Test.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uart1Test.checkSum += *((BYTE*)&uart1Test + i);
	}
  
  for(i = 0;i < LSK_TX_LEN;i++)
	{
		gUart2.UartRxBuff[i] = *((BYTE*)&uart1Test + i);
	}
#endif
	gSysFlg0.All = 0;
  
  
	gUart1.UartTxTicks = 0;
  
	gUart1.UartRxLen = 0 ;
	gUart1.UartFlag = 0;
  
  if(TEST_URAT)
  {
    
  }
	
  ENTER_CRITICAL;
  chek = *((INT16U*)FLASH_USER_START_ADDR);
  if(*((INT16U*)FLASH_USER_START_ADDR) != FLASH_FIRST_EN)
  {
    gFlashData.flashWrEn = FLASH_FIRST_EN;
		gFlashData.carLoadDelay = 0x00c8;
		gFlashData.carLoadOpa = 0x0910;
		gFlashData.carUnloadDelay = 0x0032;
		gFlashData.carUnloadOpa = 0x0a00;
		gFlashData.carSpeed = CAR_INIT_SPEED;  
    writeFlash(FLASH_USER_START_ADDR,(INT16U*)(&gFlashData.flashWrEn));
  }
  EXIT_CRITICAL;
	
	gMotor.loadDeyEn = FALSE;
	gMotor.loadEn = FALSE;
	gMotor.unLoadDeyEn = FALSE;
	gMotor.unLoadEn = FALSE;
  
	//MOTOR_INDIRECT(0);
	
	SET_MOTO(0,MOTOR_INDIRECT_STOP);
  data = (INT16U*)FLASH_USER_START_ADDR;
  //for(i = 0;i < sizeof(FASH_DATA) / 2;i++)
  for(i = 0;i < sizeof(FASH_DATA) / 2;i++)
  {
    *( (INT16U*)(&gFlashData.flashWrEn) + i) = *(data + i); 
    fashData[i] =  *(data + i); 
  }
  
  gFlashData.carUnloadDelay = 0x0032;
  
	gMotor.loadDeyTime = gFlashData.carLoadDelay;
	gMotor.loadOpaTime = gFlashData.carLoadOpa;
	gMotor.unLoadDeyTime = gFlashData.carUnloadDelay;
	gMotor.unLoadOpaTime = gFlashData.carUnloadOpa;
	gMotor.speed = gFlashData.carSpeed;
	gCar.speedK  = 127;
  /*
  gMotor.loadDeyTime = 0xc8;
	gMotor.loadOpaTime = 1111;
	gMotor.unLoadDeyTime = 0xa0;
	gMotor.unLoadOpaTime = 1111;
	gMotor.speed = 30000;
	
  */  
  //gMotor.speed = 40000;
  
	gCar.goodOk = FALSE; //上电默认无货
	gMotor.status = FALSE; //上电默认电机坏
	gEntry.goodsOk = FALSE; //上电默认有货
	//gEntry.enteryToCarEn = 0; //上电默认允许PC机发指令
	gEntry.entryToPcCnt = 0; 
	//gCar.goodOk = 1;
  gExit.goodId = 0;
	gCar.idFromEntry = 0;
  
	//上电默认允许下货
	gExit.inExitToCarEn = TRUE;
	gExit.outExitToCarEn = TRUE;
  
	gExit.uartToCarCnt = 0;
  
  gTriggerTime  = 200;   //触发200毫秒
  
  
	memset((void*)gUart1.UartRxBuff,0,sizeof(LSK_RX_LEN));
	memset((void*)gUart2.UartRxBuff,0,sizeof(LSK_RX_LEN));
	memset((void*)gUart3.UartRxBuff,0,sizeof(LSK_RX_LEN));
	memset((void*)&gCarRxFromExit.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gCarRxFromEntry.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gCarRxFromWaster.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gExitRxFromCar.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gExitRxPC.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gExitRxKey.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gWasterRxFromCar.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gEntryRxFormCar.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gEntryRxFormPc.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gEntryRxFormCar.packetHead0,0,sizeof(LSK_RX_LEN));
	memset((void*)&gEntryRxFormPlc.packetHead0,0,sizeof(LSK_RX_LEN));
}

void initGpio(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* Configure PA0 and PA1 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_12  | GPIO_Pin_11 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //OLED
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //
  // 
  //
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* Configure PB in input pushpull mode */
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_15 |GPIO_Pin_10 | GPIO_Pin_11 ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_15 |GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //LED
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //GPIO_Init(GPIOB, &GPIO_InitStructure);  
  //
  // 
  //
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  
  /* Configure PC in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Configure PA0 and PA1 in output pushpull mode */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure PA0 and PA1 in output pushpull mode */   
  //motor   p,n, ok
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /*  
  //Configure MCO 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_0);
  */
}

//FunctionName: InitSys
//Description:	Init System 
//Return Type:  void
//Argument:	void
void InitSys(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  
  //set sysTicks is 1ms
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  
  initGpio();
  initUart();
	//initSpi(); 
#if(CHEEK_HALL_EXIT)	
  initExit();
#else
  
#endif
	//initAdc();
  initDac();
  
  //initCap();
  initTimer();
  SET_REFV(250);
	initPwm();
#if(MOTOR_COMM_UART_EN)
	//GPIO_ResetBits(GPIOA, GPIO_Pin_4);
  // GPIO_SetBits(GPIOA, GPIO_Pin_4);
#else
  
  SET_MOTO(0,MOTOR_INDIRECT_STOP);
#endif
  CAR_LOADING_NO;
  RUN_POWER_ON;
  //初始化片外的外围
  memSet();
  
  LCD_Init();
}








