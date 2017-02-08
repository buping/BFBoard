/************************************
FileName:APP_Core.c//�������
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/
#include "APP_Core.h"
#include "APP_Uart.h"
#include "APP_Motor.h"
#include "PWM.h"

// Function   name   :  taskSchdule
// Description       : 
// Return     type   : void
// Argument         : void
void taskSchdule(void)
{
  INT8U i,cnt = 0;
  
  static INT16U motorSpeed = 0;
  static BOOL motorFlag = 1;
  
  
  
	//gSysFlg0.Bits.SysStart = 0;  //PC��ϵͳ״̬
	if(gSysFlg0.Bits.SysStart)
	{
 		checkEntryGoods();   //��ȡ�ϼ��ڻ����Ƿ�׼���ã����ϼ�������
    //checkMotorHall();    //���״̬��
#if(CHEEK_HALL_EN)
		if(gMotor.hallstatus)   //һ��������ϣ��Ͳ��ټ�����
		{
      checkMotorHall();    //���״̬���
    }
#endif
    
    
    uartSendPackage();  //UART ��������
		UartTimeout();		//Uart��ʱ����
		//if(gCar.uartTxEn)
		{
			if(gDeviceType == DEVICE_CAR)
			{
				motorCtl();
			}
		}
    
    if (gDeviceType == DEVICE_TRIGGER)
    {
        if (gTriggerCount > 0){
          gTriggerCount++;
          if (gTriggerCount >= gTriggerTime){
            beginPulse();
          }else{
            gTriggerCount = 0;
            stopPulse();
          }
        }
    }
    testPulse();
    displayLed();
	}
}

// Function   name   :  checkEntryGoods
// Description       : 
// Return     type   : BOOL
// Argument         : void
BOOL checkEntryGoods (void)
{
	BOOL ret = FALSE;
  
  /*  //��������ź�
  if(gDeviceType == DEVICE_ENTRY)      
  {
  if(gEntry.goodsReady == FALSE)   //����
  {
  ENTER_CRITICAL;
  
  if(!GOODS_READY)
  {
  gEntry.goodsReadyCnt++;
}
				else
  {
  gEntry.goodsReadyCnt = 0;
}
  
  if(gEntry.goodsReadyCnt > CHECK_GOODS_TIME)
  {
  gEntry.goodsReady = TRUE;
  gEntry.entryToCarOk = TRUE;
}
		        else
  {
  gEntry.goodsReady = FALSE;
}
  EXIT_CRITICAL;
}
			else
  {
  
}
}
  
  */
  //gEntry.goodsReady = 1;
  
	if(gEntry.CarEnLoad && gEntry.goodsReady)    //�����źţ�׼���ø�С����PC  ͨ��
	{
		gEntry.goodsOk = TRUE;		
	}
	else
	{
		//gEntry.goodsOk = FALSE;
		//gEntry.entryToCarOk = FALSE;
    
		//gEntry.goodsReady = FALSE;
		//gEntry.CarEnLoad = FALSE;
	}
  
  if(gEntry.goodsOk)
  {
    gEntry.goodsOkCnt++;
    /*		
		if(gEntry.goodsOkCnt > ENTERY_LOAD_OK_TIMER0)   //֪ͨС��Ҳ�Ѿ��޻�
		{
    gEntry.entryToCarOk = FALSE;
  }
		
    if(gEntry.goodsOkCnt > ENTERY_LOAD_OK_TIMER0)   //֪ͨ����
    {
    CAR_LOADING_YES;
  }
    */
    if(gEntry.goodsOkCnt > (ENTERY_LOAD_OK_TIMER0))      //֪ͨ�ϼ�
    {
      gEntry.goodsOkCnt = 0;
			gEntry.goodsReady = FALSE;
			gEntry.goodsOk = FALSE;
			gEntry.CarEnLoad = FALSE;
			gEntry.entryToCarOk = FALSE;
			CAR_LOADING_NO;	
    }
    
  }
  else
  {
    //gEntry.goodsOkCnt = 0;
    //gEntry.goodsReady = FALSE;
    //gEntry.goodsOk = FALSE;
    //gEntry.CarEnLoad = FALSE;
    //gEntry.entryToCarOk = FALSE;
    //////CAR_LOADING_NO;	
  }
  
	return ret;
}

// Function   name   :  checkMotorHall
// Description       : 
// Return     type   : BOOL
// Argument         : void
BOOL checkMotorHall(void)
{
	BOOL ret = FALSE;
	
	if(gDeviceType == DEVICE_CAR)  //�����С��������Ҫ�����
	{
		if(gMotor.opAlready)
		{
			if(gMotor.hallJudgeTimer < HALL_JUDGE_TIMER)
			{
        gMotor.hallJudgeTimer++;
			}
			else
			{
        gMotor.opAlready = FALSE;
			}
			
			if(MOTOR_HALL_STATUS_A)         //�͵�ƽ��Ч�����ź�,ֻҪ�е͵�ƽ˵�������ת��
			{
				//gMotor.hallPlusCnt++;
			}
			else
			{
        GREEN_LED_ON;
				gMotor.hallPlusCnt++;
				GREEN_LED_OFF;
			}
			gMotor.hallJudgeEn = TRUE; //������������ж����������ڼ��֮������ж�
		}
		else
		{
      
      if(gMotor.hallJudgeEn)
      {
        gMotor.hallJudgeEn = FALSE;
				if(gMotor.hallPlusCnt >= HALL_OK_PULSE_MIN)
				{
					gMotor.hallstatus = TRUE;
          
				}
				else
				{
					gMotor.hallstatus = FALSE;
					GREEN_LED_ON;
				}
				gMotor.hallPlusCnt = 0;  //������ʱ�������������������  
				
				gMotor.hallJudgeTimer = 0;	
			}
		}
	}
  
  return ret;
}

// Function   name   :  uartSendPackage
// Description       : 
// Return     type   : void
// Argument         : void
void uartSendPackage(void)
{
  //
  //���ڻ�ӦPC
  //
  if(gDeviceType == DEVICE_EXIT)  //������ϼ��ڣ�����Ϣ��PC��
  {
    if(gExit.exitToPcEn)
    {
      UART1_TX_EN;     
      gExit.exitToPcEn = FALSE;  //ȷ��ֻ��һ��
      gTicks.exitTxEnTimer = 1;
    }
    
    if(gTicks.exitTxEnTimer && (gTicks.exitTxEnTimer < 8))  //5ms��ָ����ͽ��չ���
    {
      gTicks.exitTxEnTimer++;
      
      if(gTicks.exitTxEnTimer == 3) //��Ϊ��485Ӳ��ʹ���л���Ҫʱ��
      {
        UartSendDada(EXIT_TO_PC);	
      }
    }
    else
    {
      gTicks.exitTxEnTimer = 0;
      UART1_RX_EN;     
    }
    
  }
  
  gUart1.UartTxTicks++;
  if(gUart1.UartTxTicks == UART1_TX_PERIOD)
  {
    gUart1.UartTxTicks = 0;
    
    //if()    //��������Ѿ�׼���ã��ϼ��ڷ����ڣ���С��
    {
      //if(gDeviceType == DEVICE_ENTRY)
      {
        
        //Delay(1000);
        
        if(gDeviceType == DEVICE_ENTRY)  //������ϼ��ڣ�����Ϣ��PC��
        {
          UART1_TX_EN;
          UartSendDada(ENTRY_TO_PC);
        }   
      }
    }
    
    
    
    if(gDeviceType == DEVICE_WASTER)
    {
      if(gMotor.status == FAILED)  //�����������ϱ�PC
      {
        //UART1_TX_EN;
        //UartSendDada(WASTER_TO_PC);
        //Delay_us(2000);
        //UART1_RX_EN;
      }
    }
  }       
  
  if(gUart1.UartTxTicks >= 3 && gUart1.UartTxTicks < UART1_TX_PERIOD)
  {
    if(	(gDeviceType == DEVICE_ENTRY) || (gDeviceType == DEVICE_WASTER) )  
    {
      UART1_RX_EN;
    }
  }
  
  //
  //uart2���ڷ��Ͷ�ʱ��
  //
  if(gUart2.UartTxTicks < UART2_TX_PERIOD)
  {
    
    gUart2.UartTxTicks++;
  }	
  else
  {
    gUart2.UartTxTicks = 0;
  }
  
  if(gUart2.UartTxTicks == 0)
  {
    //gUart2.UartTxTicks = 0;
    if(gDeviceType == DEVICE_ENTRY)
    {    
      //if((gEntry.goodsOkTicks > 0 && gEntry.goodsOkTicks < ENTERY_TO_CAR_TIMER) || gSysFlg0.Bits.sysDebugEn)    //��������Ѿ�׼���ã��ϼ��ڷ����ڣ���С�� 
      {
        //if(!gEntry.rxPcStatus)
        //if(gEntryTxEn)  //����洢ָ����Ч
        //if(gEntry.goodsOk)
        {
          UartSendDada(ENTRY_TO_CAR);
        }
      }
    }
    
    if(gDeviceType == DEVICE_EXIT)
    {
      if(gExit.uartToCarCnt)
      {
        UartSendDada(EXIT_TO_CAR);
      }
    } 
    
    if(gDeviceType == DEVICE_CAR)
    {
      //UartSendDada(CAR_TO_WASTER);
      //if(gCar.uartTxEn)
      if(gCar.uartTxCnt || (gCar.fromEntryDis == FALSE))
      {
        UartSendDada(CAR_TO_ENTRY); 
      }
      else
      {
        UartSendDada(CAR_TO_EXIT); 
      }
    }
    
    if(gDeviceType == DEVICE_WASTER)
    {
      UartSendDada(WASTER_TO_CAR);
    } 

    if(gDeviceType == DEVICE_REWRITE)
    {
      UartSendDada(REWRITE_TO_CAR);
    }    
    
  }       
	else
	{
		//gGpioA->Bits.MotorEn = 1; 		//��ֹ����
		//for(INT8U i = 0;i < sizeof("MWZ TH");i++)
		
	}
	
  
  //*******************
  //*******  С���ӵ��źź��ͣ�ֻ�ܷ���100ms
  //*******************
  
  if(gCar.uartTxCnt > 0  && gCar.uartTxCnt < CAR_UART_CNT)
  {
		gCar.uartTxCnt++;	
  }
	else
	{
		gCar.uartTxCnt = 0;
		//gCar.goodOk = TRUE;
	}
  
	//���ڷ��Ͷ�ʱ��
	if(gExit.uartToCarCnt)
	{
		if(gExit.uartToCarCnt < 150)
		{
			gExit.uartToCarCnt++;
		}
		else
		{
			gExit.uartToCarCnt = 0;
		}
	}
}

// Function   name   :  displayLed
// Description       : 
// Return     type   : void
// Argument         : void
void displayLed(void)
{
	gTicks.led++;
	if(gDeviceType == DEVICE_EXIT)
	{
    if(gTicks.led > 1000)
    {
      LED_ON;
      //CAR_LED_NO;
    }
    
    if(gTicks.led >= 2000)
    {
      LED_OFF;
      //CAR_LED_YES;
      gTicks.led = 0;
    }
	}
  
	if(gDeviceType == DEVICE_CAR)
	{
		if(gCar.goodOk)
		{
			LED_ON;
      CAR_LED_YES;    
      //CAR_LED_NO;
		}
		else
		{
      LED_OFF;
      
      CAR_LED_NO;
		}
	}
  
	if(gDeviceType == DEVICE_ENTRY)
	{
		if(gEntry.goodsOk)
		{
			LED_ON;
		}
		else
		{
      LED_ON;
      LED_OFF;
		}
	}
  
	if(gDeviceType == DEVICE_WASTER)
	{
    
		{
      LED_ON;
      LED_OFF;
		}
	}
}

// Function   name   :  motorCtl
// Description       : 
// Return     type   : void
// Argument         : void
void motorCtl(void)
{
	//BOOL motorDirect;
	
	
	//
  //��������ϼ�ǰ����ʱ
  //
	//if((gCar.direction & CAR_ENTRY_DIR))   //�ϼ��ڷ����ж�
	//{
	//	gCar.motorDirect = TRUE;
	//}
	//else
	//{
	//	gCar.motorDirect = FALSE;
	//}
	
	if(gMotor.loadDeyEn)   //��ʱ�ж�
	{
		if((gCar.direction & CAR_ENTRY_DIR))   //�ϼ��ڷ����ж�
		{
			gCar.motorDirect = TRUE;
		}
		else
		{
			gCar.motorDirect = FALSE;
		}
		if(gDeviceType == DEVICE_CAR)
		{
      //CAR_BREAK_YES;
      //CAR_BREAK_NO;
		}
    
		gMotor.unLoadDeyEn = FALSE;
		gMotor.unLoadEn = FALSE;
    
		gMotor.loadDeyTicks++;
		if(gMotor.loadDeyTicks >= gMotor.loadDeyTime)
		{
			gMotor.loadDeyEn = FALSE; 
			gMotor.loadEn = TRUE;
			gMotor.loadDeyTicks = 0;
			gMotor.opa = FALSE;
#if(MOTOR_COMM_UART_EN)
			setMotor(0,0,0,MOTOR_N,0);
#else
			SET_MOTO(0,MOTOR_INDIRECT_STOP);		
#endif
    }
		else
		{
			gMotor.opa = FALSE;
			gMotor.loadEn = FALSE;
#if(MOTOR_COMM_UART_EN)
			setMotor(0,0,0,MOTOR_N,0);
#else
			SET_MOTO(0,MOTOR_INDIRECT_STOP);
#endif			
		}
	}
  
  //
  //��������ϼ�����ʱ��
  //	
	if(gMotor.loadEn)   //��ʱ�ж�
	{
		gMotor.loadOpaTicks++;
		//if(gMotor.loadOpaTicks++ >= gMotor.loadOpaTime * ((float)gCar.lodeOpTimeK / LODER_OP_TIMER_BASE))
		if(gMotor.loadOpaTicks++ >= gMotor.loadOpaTime)
		{
			gMotor.loadDeyEn = FALSE; 
			gMotor.loadEn = FALSE;
			gMotor.opa = FALSE;
#if(MOTOR_COMM_UART_EN)
			setMotor(0,0,0,gCar.motorDirect,0);
#else
			SET_MOTO(0,MOTOR_INDIRECT_STOP);
#endif
			if(gDeviceType == DEVICE_CAR)
			{
        //CAR_BREAK_YES;
				CAR_BREAK_NO;
			}
      gMotor.loadOpaTicks = 0;
      gMotor.opAlready = FALSE;
      
		}
		else
		{   
			gMotor.opa = TRUE;
			//if(gMotor.status == SUCCEED)
			{
			}
      
			if(gDeviceType == DEVICE_CAR)
			{
				CAR_BREAK_YES;
				//CAR_BREAK_NO;
			}	
      //while(1);
#if(MOTOR_COMM_UART_EN)
      //
      //Ŀǰû���㹻���ֽڴ����ٶȱ���
      //
#if(TEST_PRODUCT_LINE)
			gCar.speedK = 120; 
			gCar.lodeOpTimeK = 105;
#else
			gCar.speedK = 120; 
			gCar.lodeOpTimeK = 165;
#endif
      //
			setMotor(3000 * (float)gCar.speedK / 255,50,50,gCar.motorDirect,250 * (float)gCar.lodeOpTimeK / 255);
			//setMotor(300,50,50,gCar.direction,150);
			//setMotor(3000 * 0XEE /255,50,50,gCar.direction,3000 * 0XEE /255);
			//setMotor(3000 * 0X11 /127,50,50,gCar.direction,3000 * 0X11 /127);
      
#else
      
			//SET_MOTO((INT16U)(gMotor.speed * 0.8 * (float)gCar.speedK / MOTOR_V_BASE),1);
      SET_MOTO(Pulse_25,gCar.motorDirect);
      
      
			//SET_MOTO(45000);
#endif
			gCar.goodOk = TRUE;   //�Ѿ��ϻ�
			gMotor.opAlready = TRUE;
		}
  }
  
  
	//
  //��������¼�ǰ����ʱ
  //
  //SET_MOTO(2000);
	if(gMotor.unLoadDeyEn)   //��ʱ�ж�
	{
		gMotor.loadEn = FALSE;
		gMotor.loadDeyEn = FALSE;
    
		gMotor.unLoadDeyTicks++;
		if(gMotor.unLoadDeyTicks >= gMotor.unLoadDeyTime)
		{
			gMotor.unLoadDeyEn = FALSE; 
			gMotor.unLoadEn = TRUE;
			gMotor.unLoadDeyTicks = 0;
			gMotor.opa = FALSE;
		}
	}
  
  //
  //��������¼�����ʱ��
  //	
	//if((gCar.direction & CAR_EXIT_DIR))   //�жϳ��ڷ���
	//{
  //motorDirect = TRUE;
	//	gCar.motorDirect = FALSE;
	//}
	//else
	//{
  //motorDirect = FALSE;
	//	gCar.motorDirect = TRUE;
	//}
	
	if(gMotor.unLoadEn)   //��ʱ�ж�
	{
    
		if((gCar.direction & CAR_EXIT_DIR))   //�жϳ��ڷ���
		{
      //motorDirect = TRUE;
			gCar.motorDirect = FALSE;
		}
		else
		{
      //motorDirect = FALSE;
			gCar.motorDirect = TRUE;
		}
		gMotor.unLoadOpaTicks++;
#if(MOTOR_COMM_UART_EN)
		if(gMotor.unLoadOpaTicks >= gMotor.unLoadOpaTime * (float)gCar.unLodeOpTimeK/128)
#else
      if(gMotor.unLoadOpaTicks >= 0.2 * gMotor.unLoadOpaTime * (float)gCar.unLodeOpTimeK/128)
#endif
      {
        gMotor.unLoadDeyEn = FALSE; 
        gMotor.unLoadEn = FALSE;
        gMotor.opa = FALSE;
#if(MOTOR_COMM_UART_EN)
        setMotor(0,0,0,gCar.motorDirect,0);
#else
        SET_MOTO(0,MOTOR_INDIRECT_STOP);
#endif
        if(gDeviceType == DEVICE_CAR)
        {
          CAR_BREAK_YES;
          //CAR_BREAK_NO;
        }
        gMotor.unLoadOpaTicks = 0;
        gMotor.opAlready = FALSE;
        
      }
      else
      {
        //if(gMotor.status == FAILED  || gCar.goodOk == TRUE) //����������л�
        {
          gMotor.opa = TRUE;
#if(MOTOR_COMM_UART_EN)
          //setMotor(0.075 * gMotor.speed * (float)gCar.speedK/128,50,80,gCar.direction,150);
          //
          //Ŀǰû���㹻���ֽڴ����ٶȱ���
          //
#if(TEST_PRODUCT_LINE)
          gCar.speedK = 100;
          gCar.unLodeOpTimeK = 100;
#else
          gCar.speedK = 255;
          gCar.unLodeOpTimeK = 220;
#endif
          //
          setMotor(3000 * (float)gCar.speedK / 255,50,50,gCar.motorDirect,150 * (float)gCar.unLodeOpTimeK / 255);
          //setMotor(300,50,50,gCar.direction,150);
#else
          //SET_MOTO(500,gCar.direction);
          //SET_MOTO( 0.5 * gMotor.speed * (float)gCar.speedK/128,gCar.direction);
          //SET_MOTO(30000,1);
          SET_MOTO(Pulse_25,gCar.motorDirect);
#endif
          gMotor.opAlready = TRUE;
          if(gDeviceType == DEVICE_CAR)
          {
            CAR_BREAK_YES;
            //CAR_BREAK_NO;
          }
          gCar.goodOk = FALSE;  //�Ѿ�ж��			
        }
        
      }		
		
	}
	
	//checkMotorHall();
}

void beginPulse(void){
  GPIO_SetBits(GPIOB, GPIO_Pin_8); 
  GPIO_SetBits(GPIOB, GPIO_Pin_9); 
  //GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

void stopPulse(void){
  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
  GPIO_ResetBits(GPIOB, GPIO_Pin_9);
  //GPIO_ResetBits(GPIOB, GPIO_Pin_11);
}

void testPulse(void){
  static INT16U testCounter = 0;
  
  testCounter++;
  
  if (testCounter > 0 && testCounter<2000){
    beginPulse();
  }else if (testCounter>=2000 && testCounter<6000){
    stopPulse();
  }else{
    testCounter = 0;
  }
}
