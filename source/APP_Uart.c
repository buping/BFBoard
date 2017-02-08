//FileName:APP_Uart.c
//Description:
//Creation:
//Editor:

#include "APP_Uart.h"

volatile LSK_RX gCarRxFromExit,gCarRxFromEntry,gCarRxFromWaster,gExitRxFromCar,gExitRxPC,gExitRxKey,gWasterRxFromCar,
     gEntryRxFormCar,gEntryRxFormPc,gEntryRxFormCar,
     gEntryRxFormPlc,gTriggerRxFromCar,gRewriteRxFromCar;
LSK_TX gCarTx;

//FunctionName: UartFromPC
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
INT8U serchPackageHead(INT8U *p,INT8U packageHead,INT8U deviceId)
{
	INT8U searchCnt,cnt;
	
	for(searchCnt = 0; searchCnt < (UARTRXBUFFLEN - LSK_RX_LEN); searchCnt++)
	{
    if( (*p == packageHead) && ( *(p + 1) == deviceId ))
    {
      cnt = searchCnt;	
			break;
    }
    else
    {
      cnt = 0;
    }
	}
  return (cnt);
}

//FunctionName: UartFromPC
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromPC(INT8U UartId,UART_SOURCE *UartSource)
{
	BYTE *pData,*pDest;
	static INT8U i,checkSum;
  LSK_RX uartRx;
	LSK_RX_SET userSet;
	INT16U id;
  
  ENTER_CRITICAL;
  
  //
  //�ϼ��ڴ��ϼ�̨��ȡ����
  //
	//if(gEntry.rxFromPcDis == FALSE)  //����ɹ��������������û�����꣬�������µ�����
	{
		//if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == PC_COMMAND_ID))
		if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == ENTRY_COMMAND_ID))  // ��ʱ���ܿ�̨����
		{
      //copy data
      pData = (BYTE*)&uartRx;
			for(i = 0;i < sizeof(LSK_RX);i++)
			{
				*(pData + i) = UartSource->UartRxBuff[i];
			}
      
			//UartSource->UartRxBuff[0] = 0;
      pData = (BYTE*)&uartRx;  
      
			//����У���
			//pData = (BYTE*)&gPlatRx.Code;(LSK_RX_LEN - 1)
      checkSum = 0;
			checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
			if(uartRx.checkSum == checkSum)
			{
        if(gDeviceType == DEVICE_ENTRY)   //�ϼ���
        {
          pData = (BYTE*)&uartRx;
          pDest = (BYTE*)&gEntryRxFormPc;
          for(i = 0;i < sizeof(uartRx);i++)
          {
            *(pDest + i) = *(pData + i);
          }
					gEntry.rxPcStatus  = gEntryRxFormPc.status.all;
          
					if(gEntryRxFormPc.status.all)
					{
						gEntry.goodsReady = TRUE;
					}
					else
					{
						gEntry.goodsReady = FALSE;
					}
          
					gEntry.rxFromPcDis = TRUE;   //����ϼ��ڽӵ������ϻ�������
        }
        
        //ʹ��������ڴ�
        
        for(i = 0;i < sizeof(LSK_RX);i++)
        {
          UartSource->UartRxBuff[i] = 0;
        }
        
			}
		}
	}
  
	if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == PC_COMMAND_ID))
	{
    //copy data
    pData = (BYTE*)&uartRx;
    for(i = 0;i < sizeof(LSK_RX);i++)
    {
      *(pData + i) = UartSource->UartRxBuff[i];
    }
    
    //UartSource->UartRxBuff[0] = 0;
    pData = (BYTE*)&uartRx;  
    
    //����У���
    //pData = (BYTE*)&gPlatRx.Code;(LSK_RX_LEN - 1)
    checkSum = 0;
    checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
    if(uartRx.checkSum == checkSum)
    {
      if(gDeviceType == DEVICE_EXIT)   //����
      {
        pData = (BYTE*)&uartRx;
        pDest = (BYTE*)&gExitRxPC;
        for(i = 0;i < sizeof(uartRx);i++)
        {
          *(pDest + i) = *(pData + i);
        }
        
        id =  (gExitRxPC.deviceIdH << 8) +  gExitRxPC.deviceIdL;
        if(gExit.id == id)  //������㲥��ַ���
        {
          gExit.exitToPcEn = TRUE;  //���������ͱ�־����PC����Ϣ
        }
        else
        {
          gExit.exitToPcEn = FALSE;
        }
        
        //ʹ��������ڴ�
        for(i = 0;i < sizeof(LSK_RX);i++)
        {
          UartSource->UartRxBuff[i] = 0;
        }
      }
    }
	}
  EXIT_CRITICAL;
  
	//
	//�������в���
	//
	if((UartSource->UartRxBuff[0] == FROM_PC_HEAD1) && (UartSource->UartRxBuff[1] == USER_COMMAND_ID))
	{
		gSysFlg0.Bits.sysDebugEn = TRUE;
    //copy data
    pData = (BYTE*)&userSet;
		for(i = 0;i < sizeof(LSK_RX_SET);i++)
		{
			*(pData + i) = UartSource->UartRxBuff[i];
		}
    
		//UartSource->UartRxBuff[0] = 0;
    pData = (BYTE*)&userSet;  
    
    ENTER_CRITICAL;
		//����У���
		//pData = (BYTE*)&gPlatRx.Code;
    checkSum = 0;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX_SET) - 1),ADD_SUM) & 0xff;
    
    
    if(userSet.checkSum == checkSum)
		{
      
			gFlashData.carLoadDelay = (userSet.loadDeyTimeH << 8) + userSet.loadDeyTimeL;
			gFlashData.carLoadOpa = (userSet.loadOpaTimeH << 8) + userSet.loadOpaTimeL;
			gFlashData.carUnloadDelay = (userSet.unLoadDeyTimeH << 8) + userSet.unLoadDeyTimeL;
			gFlashData.carUnloadOpa = (userSet.unLoadOpaTimeH << 8) + userSet.unLoadOpaTimeL;
			gFlashData.carSpeed = (userSet.motorSpeedH << 8) + userSet.motorSpeedL;
      
			/*
			if(gFlashData.carSpeed > 4000)
      {
      gFlashData.carSpeed = 4000;
    }
      */
      
#if(PROGRAM_FLASH_EN)			
      writeFlash(FLASH_USER_START_ADDR,(INT16U*)&gFlashData.flashWrEn);
#endif
      
			//readFlash(fashdata);
			
      UartSource->UartRxBuff[0] = 0;//��ֹ�ظ�дflash
      UartSource->UartRxBuff[12] = 0;//��ֹ�ظ�дflash
      
#if(MOTOR_COMM_UART_EN)
			gMotor.loadDeyTime = gFlashData.carLoadDelay;
			gMotor.loadOpaTime = gFlashData.carLoadOpa;
			gMotor.unLoadDeyTime = gFlashData.carUnloadDelay;
			gMotor.unLoadOpaTime = gFlashData.carUnloadOpa;
			gCar.speed = gFlashData.carSpeed;
#else
			gMotor.loadDeyTime = gFlashData.carLoadDelay;
			gMotor.loadOpaTime = gFlashData.carLoadOpa;
			gMotor.unLoadDeyTime = gFlashData.carUnloadDelay;
			gMotor.unLoadOpaTime = gFlashData.carUnloadOpa;
			gMotor.speed = gFlashData.carSpeed;
#endif
			
		}
    
    //ʹ��������ڴ�
    for(i = 0;i < sizeof(LSK_RX);i++)
    {
      UartSource->UartRxBuff[i] = 0;
    }
    EXIT_CRITICAL;
	}
}	

//FunctionName: UartFromEntry
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromEntry(INT8U UartId,UART_SOURCE *UartSource)
{
	BYTE *pData,*pDest;
	INT8U i,checkSum = 0;
  LSK_RX uartRx;
	LSK_RX_SET userSet;
	INT16U id = 0;
  
	if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == ENTRY_COMMAND_ID))
	{
    //copy data
    pData = (BYTE*)&uartRx;
		for(i = 0;i < sizeof(LSK_RX);i++)
		{
			*(pData + i) = UartSource->UartRxBuff[i];
		}
    
		//UartSource->UartRxBuff[0] = 0;
    pData = (BYTE*)&uartRx;  
    
		//����У���
		//pData = (BYTE*)&gPlatRx.Code;
    checkSum = 0;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      
			if(gSysFlg0.Bits.sysDebugEn)
			{
				LED_ON;   //�����ã�С���ӵ��ϼ���
			}
			
      //if(gCar.fromEntryDis && (gCarRxFromEntry.deviceIdH != 0 && gCarRxFromEntry.deviceIdL != 0 )) //�����С�������Ѿ����ܵ���
      if(gCar.fromEntryDis) //�����С�������Ѿ����ܵ���
      {
      }
      else
      {
        id = ((uartRx.deviceIdH << 8) & 0xff00) + uartRx.deviceIdL;
        if(id != 0)
        {
          pData = (BYTE*)&uartRx;
          pDest = (BYTE*)&gCarRxFromEntry;
          for(i = 0;i < sizeof(LSK_RX);i++)
          {
            *(pDest + i) = *(pData + i);
          }
          
          gCar.idFromEntry = (gCarRxFromEntry.deviceIdH << 8) + gCarRxFromEntry.deviceIdL;
          //gCar.speed = ( ((gCarRxFromEntry.carReTimerH << 8) & 0xff00)  +  (gCarRxFromEntry.carReTimerL & 0xff));
          //gCar.sleepEn = TRUE;
          
          gCar.dest = ( ((gCarRxFromEntry.deviceIdH << 8) & 0xff00)  +  (gCarRxFromEntry.deviceIdL & 0xff));
          /*                          
          if(gCarRxFromEntry.carReTimerH)
          {
          gCar.speedK = gCarRxFromEntry.carReTimerH;
        }
					  else
          {
          gCar.speedK = MOTOR_V_BASE;
        }
          
          if(gCarRxFromEntry.carReTimerL)
          {
          gCar.lodeOpTimeK = gCarRxFromEntry.carReTimerL;  //������תʱ�� 
        }
					  else
          {
          gCar.lodeOpTimeK = LODER_OP_TIMER_BASE;  //������תʱ�� 
        }
          */                  
          
          gCar.direction = gCarRxFromEntry.direction; //����
          
          
          if(((gCar.goodOk == FALSE) && (gCarRxFromEntry.status.all == TRUE)) && (gCar.idFromEntry != 0)) //С���޻�
            //if(gCar.goodOk == FALSE) //С���޻�
          {
            gCar.fromEntryDis = TRUE;  	 //�Ѿ��ϻ�ȷ�ϣ��������ٴν���
            gCar.uartTxCnt = 1;  
            gMotor.loadDeyEn = TRUE;
            gCarRxFromEntry.status.all = 0;
          }
          else
          {
            //���Ը�PC��PLC�ش�ready��
          }
          
          //******************************2015-12-06
          //gCar.uartTxEn = TRUE;
          //gCar.uartTxCnt = 1;   
          //ENTER_CRITICAL;
          //UartSendDada(CAR_TO_ENTRY);
          //EXIT_CRITICAL;
          //******************************
        }
      }
      
			//ʹ��������ڴ�
			for(i = 0;i < sizeof(LSK_RX);i++)
			{
        UartSource->UartRxBuff[i] = 0;
			}
    }
	}
  
  //*
	//������������в���
	if((UartSource->UartRxBuff[0] == FROM_PC_HEAD1) && (UartSource->UartRxBuff[1] == USER_COMMAND_ID))
	{
		gSysFlg0.Bits.sysDebugEn = TRUE;
    //copy data
    pData = (BYTE*)&userSet;
		for(i = 0;i < sizeof(LSK_RX_SET);i++)
		{
			*(pData + i) = UartSource->UartRxBuff[i];
		}
    
		//UartSource->UartRxBuff[0] = 0;
    pData = (BYTE*)&userSet;  
    
    ENTER_CRITICAL;
		//����У���
		//pData = (BYTE*)&gPlatRx.Code;
    checkSum = 0;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX_SET) - 1),ADD_SUM) & 0xff;
    
    
    if(userSet.checkSum == checkSum)
		{
      
			gFlashData.carLoadDelay = (userSet.loadDeyTimeH << 8) + userSet.loadDeyTimeL;
			gFlashData.carLoadOpa = (userSet.loadOpaTimeH << 8) + userSet.loadOpaTimeL;
			gFlashData.carUnloadDelay = (userSet.unLoadDeyTimeH << 8) + userSet.unLoadDeyTimeL;
			gFlashData.carUnloadOpa = (userSet.unLoadOpaTimeH << 8) + userSet.unLoadOpaTimeL;
			gFlashData.carSpeed = (userSet.motorSpeedH << 8) + userSet.motorSpeedL;
      
      /*
      if(gFlashData.carSpeed > 4000)
      {
      gFlashData.carSpeed = 4000;
    }
      */            
      
#if(PROGRAM_FLASH_EN)			
      writeFlash(FLASH_USER_START_ADDR,(INT16U*)&gFlashData.flashWrEn);
#endif
      
			//readFlash(fashdata);
      UartSource->UartRxBuff[0] = 0;//��ֹ�ظ�дflash
      UartSource->UartRxBuff[11] = 0;//��ֹ�ظ�дflash
      UartSource->UartRxBuff[12] = 0;//��ֹ�ظ�дflash
      
			gMotor.loadDeyTime = gFlashData.carLoadDelay;
			gMotor.loadOpaTime = gFlashData.carLoadOpa;
			gMotor.unLoadDeyTime = gFlashData.carUnloadDelay;
			gMotor.unLoadOpaTime = gFlashData.carUnloadOpa;
			gMotor.speed = gFlashData.carSpeed;
		}
    EXIT_CRITICAL;
	}
  //*/
}

//FunctionName: UartFromKey
//Description:	�˹���ť���ƣ�ͨ��PC��ת��
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromKey(INT8U UartId,UART_SOURCE *UartSource)
{
	BYTE *pData,*pDest;
	INT8U i,checkSum = 0;
  LSK_RX uartRx;
  INT16U id;
  
	if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == EXIT_KEY_ID))
	{
    //copy data
    pData = (BYTE*)&uartRx;
		for(i = 0;i < sizeof(LSK_RX);i++)
		{
			*(pData + i) = UartSource->UartRxBuff[i];
		}
    
		//UartSource->UartRxBuff[0] = 0;
    pData = (BYTE*)&uartRx;  
    
		//����У���
		//pData = (BYTE*)&gPlatRx.Code;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      
			if(gSysFlg0.Bits.sysDebugEn)
			{
				LED_OFF;   //�����ã�С���ӵ��ϼ���
			}
      
      pData = (BYTE*)&uartRx;
      pDest = (BYTE*)&gExitRxKey;
      for(i = 0;i < sizeof(LSK_RX);i++)
      {
        *(pDest + i) = *(pData + i);
      }
      
      id = (gExitRxKey.deviceIdH << 8) + gExitRxKey.deviceIdL;
      
      
			if(gExit.id == id)
			{
        //
        //��ĳһ�෢����ָ����ҷ�����ָ����Ч��ִ��
        //
				if(gExitRxKey.status.key.exitUnlodeEn)
				{
          if((gExitRxKey.direction & EXIT_IN_CTL) && ((gExitRxKey.direction & EXIT_IN_OUT_DIRCTION) == FALSE))  //�ڲ����
          {
				 		gExit.inExitToCarEn = TRUE;
				 	}
				 	
          if((gExitRxKey.direction & EXIT_OUT_CTL) && (gExitRxKey.direction & EXIT_IN_OUT_DIRCTION))  //������
          {
				 		gExit.outExitToCarEn = TRUE;
				 	}
				 	gExit.goodCnt = 0;   //�����»�ʱ���������������
				}
        
        
				
        
				if(gExitRxKey.status.key.exitUnlodeDis)
				{
          if(((gExitRxKey.direction & EXIT_OUT_CTL) == FALSE) && ((gExitRxKey.direction & EXIT_IN_OUT_DIRCTION) == FALSE))  //�ڲ����
          {
				 		gExit.inExitToCarEn = FALSE;
				 	}
				 	
          if(((gExitRxKey.direction & EXIT_IN_CTL) == FALSE) && (gExitRxKey.direction & EXIT_IN_OUT_DIRCTION))  //������
          {
				 		gExit.outExitToCarEn = FALSE;
				 	}
				}
			}
      
			//ʹ��������ڴ�
			for(i = 0;i < sizeof(LSK_RX);i++)
			{
        UartSource->UartRxBuff[i] = 0;
			}
      
		}
	}
}

//FunctionName: UartFromExit
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromExit(INT8U UartId,UART_SOURCE *UartSource)
{
	BYTE *pData,*pDest;
	INT8U i,checkSum = 0,temp;
  LSK_RX uartRx;
  
	if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == EXIT_COMMAND_ID))
	{
    //copy data
    pData = (BYTE*)&uartRx;
		for(i = 0;i < sizeof(LSK_RX);i++)
		{
			*(pData + i) = UartSource->UartRxBuff[i];
		}
    
		//UartSource->UartRxBuff[0] = 0;
    pData = (BYTE*)&uartRx;  
    
		//����У���
		//pData = (BYTE*)&gPlatRx.Code;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      
			if(gSysFlg0.Bits.sysDebugEn)
			{
				LED_OFF;   //�����ã�С���ӵ��ϼ���
			}
      
      pData = (BYTE*)&uartRx;
      pDest = (BYTE*)&gCarRxFromExit;
      for(i = 0;i < sizeof(LSK_RX);i++)
      {
        *(pDest + i) = *(pData + i);
      }
      
      gCar.idFromExit = (gCarRxFromExit.deviceIdH << 8) + gCarRxFromExit.deviceIdL;
      
			if((gCarRxFromExit.direction & EXIT_IN_CTL)) //�����»�����
      {
        gCar.direction |= EXIT_IN_CTL;
			}
			else
			{
				gCar.direction &= ~EXIT_IN_CTL;
			}
			
			if((gCarRxFromExit.direction & EXIT_OUT_CTL)) //�����»�����
      {
        gCar.direction |= EXIT_OUT_CTL;
			}
			else
			{
				gCar.direction &= ~EXIT_OUT_CTL;
			}
      
			if(gCarRxFromExit.carReTimerH)
			{
        gCar.speedK = gCarRxFromExit.carReTimerH;
			}
			else
			{
        gCar.speedK = MOTOR_V_BASE;
			}
			
      if(gCarRxFromExit.carReTimerL)
			{
        gCar.unLodeOpTimeK =  gCarRxFromExit.carReTimerL;  //������תʱ�� 
			}
			else
			{
        gCar.unLodeOpTimeK = UNLODER_OP_TIMER_BASE;  //������תʱ�� 
			}    
      
      if((gCar.idFromExit == gCar.idFromEntry) && (((gCar.direction & EXIT_IN_CTL) && ((gCar.direction & CAR_EXIT_DIR) == FALSE)) || ( (gCar.direction & EXIT_OUT_CTL) && (gCar.direction & CAR_EXIT_DIR))) )   //�ҵ�����
      {
        if(gCar.goodOk == TRUE)  //��δж��
        {
          //gCarTx.carUnLoadTimerH = gCarRxFromExit.deviceIdH;
          //gCarTx.carUnLoadTimerL = gCarRxFromExit.deviceIdL;
          gCar.fromEntryDis = 0; //�����´ν���
          gCarTx.status.bits.exitStatus = TRUE;
          
          
          //SET_MOTO(gCar.speed);
          gMotor.unLoadDeyEn  = TRUE;
        }
      }
      
			//ʹ��������ڴ�
			for(i = 0;i < sizeof(LSK_RX);i++)
			{
        UartSource->UartRxBuff[i] = 0;
			}
      
		}
	}
}

//FunctionName: UartFromPlc
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromPlc(INT8U UartId,UART_SOURCE *UartSource)
{
	BYTE *pData,*pDest;
	INT8U i,checkSum = 0;
  LSK_RX uartRx;
  
	if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == EXIT_COMMAND_ID))
	{
    //copy data
    pData = (BYTE*)&uartRx;
		for(i = 0;i < sizeof(LSK_RX);i++)
		{
			*(pData + i) = UartSource->UartRxBuff[i];
		}
    
		//UartSource->UartRxBuff[0] = 0;
    pData = (BYTE*)&uartRx;  
    
		//����У���
		//pData = (BYTE*)&gPlatRx.Code;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      
			if(gSysFlg0.Bits.sysDebugEn)
			{
				LED_OFF;   //�����ã�С���ӵ��ϼ���
			}
      
      pData = (BYTE*)&uartRx;
      pDest = (BYTE*)&gEntryRxFormPlc;
      for(i = 0;i < sizeof(LSK_RX);i++)
      {
        *(pDest + i) = *(pData + i);
      }
      
      //�л�����
      /*
      if(gEntryRxFormPlc.status)
      {
      
    }
			else
			{
    }
      
			//С���ϼ�����ʱ��
			if()
			{
    }
			else
			{
    }
			
      */
			//ʹ��������ڴ�
			for(i = 0;i < sizeof(LSK_RX);i++)
			{
        UartSource->UartRxBuff[i] = 0;
			}
      
		}
	}
}

//FunctionName: UartFromCar
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromCar(INT8U UartId,UART_SOURCE *UartSource)
{
	BYTE *pData,*pDest;
	INT8U i,j,checkSum;
	INT16U id = 0;
	INT32U goodId = 0;
  LSK_RX uartRx;
  
	if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == CAR_COMMAND_ID))
	{
    //copy data
    pData = (BYTE*)&uartRx;
		for(i = 0;i < sizeof(LSK_RX);i++)
		{
			*(pData + i) = UartSource->UartRxBuff[i];
		}
    
		//UartSource->UartRxBuff[0] = 0;
    pData = (BYTE*)&uartRx;  
    
		//����У���
		//pData = (BYTE*)&gPlatRx.Code;
    checkSum = 0;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      if(gDeviceType == DEVICE_ENTRY)
      {
        pData = (BYTE*)&uartRx;
        pDest = (BYTE*)&gEntryRxFormCar;
        for(i = 0;i < sizeof(LSK_RX);i++)
        {
          *(pDest + i) = *(pData + i);
        }
        
        gEntry.carStatus = gEntryRxFormCar.status.all;   //��С���ӵ���һ��okָ��
        
        if(gEntryRxFormCar.status.all == TRUE)  //�ӵ�С���л�
        {
          gEntry.CarEnLoad = TRUE; //�����ϻ�	
        }
        else
        {  
          gEntry.CarEnLoad = FALSE;  //�����ϻ�
        }
        
      }
      
      if(gDeviceType == DEVICE_WASTER)
      {
        pData = (BYTE*)&uartRx;
        pDest = (BYTE*)&gWasterRxFromCar;
        for(i = 0;i < sizeof(LSK_RX);i++)
        {
          *(pDest + i) = *(pData + i);
        }
        gWaste.motorStatus = gWasterRxFromCar.status.all;
        LED_ON;
        //LED_OFF;
      }
      
      if(gDeviceType == DEVICE_EXIT)
      {
        id = (uartRx.deviceIdH << 8) + uartRx.deviceIdL;
        if((id != 0) && (id == gExit.id))
        {
          gExit.uartToCarCnt = 1;  //�����¼�����
        }
        
        //  �ϼ�̨���                            ������ˮ�Ÿ�                 ������ˮ�ŵ�
        goodId = (uartRx.carReTimerL << 16) + (uartRx.carUnLoadTimerL << 8) + uartRx.carReTimerH;
        
        //if((id != 0) && (id == gExit.id) && (uartRx.status.all == TRUE))   //Ŀ�ĵ���������
        
        if((id != 0) && (id == gExit.id))
        {
          pData = (BYTE*)&uartRx;
          pDest = (BYTE*)&gExitRxFromCar;
          for(i = 0;i < sizeof(LSK_RX);i++)
          {
            *(pDest + i) = *(pData + i);
          }
          
          //  �ϼ�̨���                                  ������ˮ�Ÿ�                                 ������ˮ�ŵ�
          goodId = (gExitRxFromCar.carReTimerL << 16) + (gExitRxFromCar.carUnLoadTimerL << 8) + gExitRxFromCar.carReTimerH;
          if(goodId != 0)
          {
            gExit.goodId = goodId;
            
            ENTER_CRITICAL;
            if(gExit.oldGoodId != gExit.goodId)
            {	
              for(j = 0;j < 2;j++)
              {
                if(gGoods.cnt < GOOGS_LEN)
                {
                  for(i = (GOOGS_LEN - 1);i > 0;i--)
                  {
                    //if(gGoods.cnt < GOOGS_LEN)
                    {
                      gGoods.goodsId[i] = gGoods.goodsId[i - 1];        //����ID
                      gGoods.exitDirect[i] = gGoods.exitDirect[i - 1];  //���ڷ���  
                      
                    }
                  }
                  gGoods.cnt++;  //���ڴ��ڷ���ָ�����
                  
                }
                gGoods.goodsId[0] = goodId;
                gGoods.exitDirect[0] = gExitRxFromCar.direction;
              }
              gExit.goodCnt++;
              gExit.oldGoodId = gExit.goodId;
            }
            //�غŻ�������λ�����
            EXIT_CRITICAL;
          }
        }
      }
      
      if (gDeviceType == DEVICE_TRIGGER)
      {
        if (gTriggerCount == 0){
          gTriggerCount = 1;
          gTriggerRxFromCar = uartRx;
        }
      }
      
      if (gDeviceType == DEVICE_REWRITE)
      {
        gRewriteRxFromCar = uartRx;
      }
      
      //
      //��ȷ���պ�����
      //
      for(i = 0;i < sizeof(LSK_RX);i++)
      {
        UartSource->UartRxBuff[i] = 0;
      }				
    }
	}
	else
	{
		gEntryRxFormCar.status.all == FALSE;
	}
}

//FunctionName: UartFromWaster
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
void UartFromWaster(INT8U UartId,UART_SOURCE *UartSource)
{
	BYTE *pData,*pDest;
	INT8U i,checkSum,searchCnt = 0;
  LSK_RX uartRx;
	
	pData =(BYTE*)UartSource;
  searchCnt = serchPackageHead(pData,FROM_PC_HEAD0,WASTER_COMMAND_ID);
  
	if((UartSource->UartRxBuff[searchCnt + 0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[searchCnt + 1] == WASTER_COMMAND_ID))
	{
    //copy data
    pData = (BYTE*)&uartRx;
		for(i = searchCnt;i < sizeof(LSK_RX);i++)
		{
			*(pData + i) = UartSource->UartRxBuff[i + searchCnt];
		}
    
		//UartSource->UartRxBuff[0] = 0;
    pData = (BYTE*)&uartRx;  
    
		//����У���
		//pData = (BYTE*)&gPlatRx.Code;
    checkSum = 0;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      //����У���
      //pData = (BYTE*)&gPlatRx.Code;
      checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
      if(uartRx.checkSum == checkSum)
      {
        pData = (BYTE*)&uartRx;
        pDest = (BYTE*)&gCarRxFromWaster;
        for(i = 0;i < sizeof(LSK_RX);i++)
        {
          *(pDest + i) = *(pData + i);
        }
      }
      
      //gCar.direction = (gCarRxFromWaster.deviceIdH << 8) + gCarRxFromWaster.deviceIdL;
      gCar.direction = gCarRxFromWaster.direction;
			
      if(gCarRxFromWaster.carReTimerH)
			{
        gCar.speedK = gCarRxFromWaster.carReTimerH;
			}
			else
			{
        gCar.speedK = MOTOR_V_BASE;
			}
      
      if(gCarRxFromWaster.carReTimerL)
			{
        gCar.unLodeOpTimeK = gCarRxFromWaster.carReTimerL;  //������תʱ�� 
			}
			else
			{
        gCar.unLodeOpTimeK = LODER_OP_TIMER_BASE;  //������תʱ�� 
			}   
			
      //if(gCar.goodOk == TRUE || gMotor.status == FAILED)  //��δж�����ߵ������������ͼ�޸�һ��
      {
        
        //gCarTx.carUnLoadTimerH = gCarRxFromExit.deviceIdH;
        //gCarTx.carUnLoadTimerL = gCarRxFromExit.deviceIdL;
        gCar.fromEntryDis = 0; //�����´ν���
        //gCarTx.status.bits.exitStatus = TRUE;
        
        
        //SET_MOTO(gCar.speed);
        gMotor.unLoadDeyEn  = TRUE;
        
        //if(gCarRxFromExit.direction == 1)
        {
          //MOTO_P_EN;
          //MOTO_N_DIS; 
        }
      }
			
			//ʹ��������ڴ�
			for(i = 0;i < UARTRXBUFFLEN;i++)
			{
        UartSource->UartRxBuff[i] = 0;
			}
		}
	}
}

//FunctionName: CarToEntry
//Description:	
//Return Type:  void
//Argument:	void
void CarToEntry(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
	
	ENTER_CRITICAL;
	//
	//֡ͷ
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = CAR_COMMAND_ID;
  
  //�����
  uartTx.deviceIdH = gCarRxFromEntry.deviceIdH;
  uartTx.deviceIdL = gCarRxFromEntry.deviceIdL;
  
	//Ŀ�ĵ�ID,�¼��ڸ�ֵ
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = 0;   //PC��Ҫ��������
  
  uartTx.carUnLoadTimerH = gSwitch & 0xFF;
  
  //uartTx.status.all = gMotor.status;
  
	/*
  if(gCar.goodOk)   //С���л�
  { 
  uartTx.status.all = TRUE; 		
}
                  else
  { 
  if(gMotor.status == SUCCEED)
  {
  uartTx.status.all = FALSE; //ͨ��PLC�Ѿ��ҵ�������С��
}
					   else
  {
  uartTx.status.all = TRUE;
}
}
	*/			  
  if(gCar.uartTxCnt)   //С���޻������ҽӵ��ϼ����Ѿ�װ����
  { 
    uartTx.status.all = TRUE; 		
  }
  else
  { 
    uartTx.status.all = FALSE;  
  }				  
	//
	//����У���
	//
	uartTx.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uartTx.checkSum += *((BYTE*)&uartTx + i);
	}
	EXIT_CRITICAL;
	
	UartSend(WASTER_CAR_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
}

//FunctionName: CarToExit
//Description:	
//Return Type:  void
//Argument:	void
void CarToExit(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
	INT8U id;
  
	ENTER_CRITICAL;
	//
	//֡ͷ
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = CAR_COMMAND_ID;
  
  //����Ŀ�ĵأ�Ҳ���ǳ���id
  
  uartTx.deviceIdH = gCarRxFromEntry.deviceIdH;
  uartTx.deviceIdL = gCarRxFromEntry.deviceIdL;
  
  id = (gCarRxFromEntry.deviceIdH << 8) + gCarRxFromEntry.deviceIdL;
  
  if(id = 0)
  {
    id = 0;
  }
  else
  {
		//Ŀ�ĵ�ID,�¼��ڸ�ֵ
    uartTx.carUnLoadTimerH = gCarRxFromEntry.carUnLoadTimerH;
    uartTx.carUnLoadTimerL = gCarRxFromEntry.carUnLoadTimerL;
    uartTx.carReTimerH = gCarRxFromEntry.carReTimerH;
    uartTx.carReTimerL = gCarRxFromEntry.carReTimerL;
    uartTx.direction = gCarRxFromEntry.direction;   //PC��Ҫ��������
    uartTx.status.all= gMotor.status;
    
    uartTx.carUnLoadTimerH = gSwitch & 0xFF;
  }
	//
	//����У���
	//
	uartTx.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uartTx.checkSum += *((BYTE*)&uartTx + i);
	}
	EXIT_CRITICAL;
	
	UartSend(WASTER_CAR_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
}

//FunctionName: CarToWaster
//Description:	
//Return Type:  void
//Argument:	void
void CarToWaster(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
  
	ENTER_CRITICAL;
	//
	//֡ͷ
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = CAR_COMMAND_ID;
  
  //�����ID
  uartTx.deviceIdH = (gSwitch & 0xff00) >> 8;
  uartTx.deviceIdL = gSwitch & 0xff;
  
	//Ŀ�ĵ�ID,�¼��ڸ�ֵ
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = 0;   //PC��Ҫ��������
  uartTx.status.all = gMotor.status;
	//
	//����У���
	//
	uartTx.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uartTx.checkSum += *((BYTE*)&uartTx + i);
	}
	EXIT_CRITICAL;
  
	UartSend(WASTER_CAR_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
}

//FunctionName: CarToPlc
//Description:	
//Return Type:  void
//Argument:	void
void CarToPlc(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
  
	ENTER_CRITICAL;
	//
	//֡ͷ
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = CAR_COMMAND_ID;
  uartTx.deviceIdH = 0;
  uartTx.deviceIdL = 0;
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = 0;   //PC��Ҫ��������
  uartTx.status.all = SUCCEED;
	//
	//����У���
	//
	uartTx.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uartTx.checkSum += *((BYTE*)&uartTx + i);
	}
	EXIT_CRITICAL;
	
	UartSend(PLC_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
}

//FunctionName: ExitToCar;
//Description:	
//Return Type:  void
//Argument:	void
void ExitToCar(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
  
	ENTER_CRITICAL;
	//if()
	{
		//
		//֡ͷ
		//
		uartTx.packetHead0 = LSK_TX_HEAD0;
    uartTx.instructionId = EXIT_COMMAND_ID;
    uartTx.deviceIdH = (gSwitch >> 8) & 0xff;
    uartTx.deviceIdL = gSwitch & 0xff;
		//************************
    uartTx.carUnLoadTimerH = 0;
    uartTx.carUnLoadTimerL = 0;
    uartTx.carReTimerH = 0;
		//************************
    uartTx.carReTimerL = 255;//UNLODER_OP_TIMER_BASE * 0.3;
    uartTx.carReTimerH = 255;//0.9 * MOTOR_V_BASE;
    
    //
    uartTx.direction = 0;
    
		if(gExit.inExitToCarEn) //�ڲ����
		{
      uartTx.direction |= EXIT_IN_CTL;   //PC��Ҫ��������	
		}
		else
		{
      uartTx.direction &= ~EXIT_IN_CTL; 
		}
    
		if(gExit.outExitToCarEn)    //������
		{
      uartTx.direction |= EXIT_OUT_CTL;   //PC��Ҫ��������	
		}
		else
		{
      uartTx.direction &= ~EXIT_OUT_CTL; 
		}
		
		//*
	 	//uartTx.direction = 0xff;  //����
		//*/
    
    uartTx.status.bits.pcStatus = SUCCEED;
		//
		//����У���
		//
		uartTx.checkSum = 0;
		for(i = 0;i < (LSK_TX_LEN - 1);i++)
		{
			uartTx.checkSum += *((BYTE*)&uartTx + i);
		}
		EXIT_CRITICAL;
		
		UartSend(EXIT_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
	}
}

//FunctionName: ExitToPc;
//Description:	
//Return Type:  void
//Argument:	void
void ExitToPc(void)
{
	LSK_TX uartTx;
	INT8U i = 0;
	INT32U goodsId;
	static BYTE cnt;
  
	ENTER_CRITICAL;
  //if(gExit.exitToPcEn)  //PC�㲥���г��ڳɹ��������
  {
		//
		//֡ͷ
		//
		uartTx.packetHead0 = LSK_TX_HEAD0;
    uartTx.instructionId = EXIT_COMMAND_ID;
    uartTx.deviceIdH = (gSwitch >> 8) & 0xff;
    uartTx.deviceIdL = gSwitch & 0xff;
    uartTx.direction = 0;
    uartTx.status.all = 0;
    
		//************************
    
		//*
    
    //  �ϼ�̨���                                  ������ˮ�Ÿ�                                 ������ˮ�ŵ�
    //goodId = (gExitRxFromCar.carReTimerL << 16) + (gExitRxFromCar.carUnLoadTimerL << 8) + gExitRxFromCar.carReTimerH;
		
		if(gGoods.cnt)
		{
      uartTx.carReTimerL = (gGoods.goodsId[0] >> 16) & 0xff; //����bit0 ~ bit7
      uartTx.carReTimerH = (gGoods.goodsId[0] >> 0) & 0xff; //����bit8 ~ bit15
      uartTx.carUnLoadTimerL = (gGoods.goodsId[0] >> 8) & 0xff; //����bit16 ~ bit23
      uartTx.direction = gGoods.exitDirect[0];   //PC��Ҫ��������
      uartTx.status.bits.pcStatus = 1;	
      
      if(gGoods.cnt)
      {
        for(i = 0;i < (GOOGS_LEN - 1);i++)
        {
          gGoods.goodsId[i] = gGoods.goodsId[i + 1];
          gGoods.exitDirect[i]= gGoods.exitDirect[i + 1];
        }
        gGoods.cnt--;
			}
			//gExit.oldGoodId = 0;
		}
		else
		{
      uartTx.carReTimerL = 0; //����bit0 ~ bit7
      uartTx.carReTimerH = 0; //����bit8 ~ bit15
      uartTx.carUnLoadTimerL = 0; //����bit16 ~ bit23
			uartTx.status.bits.pcStatus = 0;
		}
		
    
    /*
    uartTx.carReTimerL = gExitRxFromCar.carReTimerL;//����bit0 ~ bit7
    uartTx.carReTimerH = gExitRxFromCar.carReTimerH; //����bit8 ~ bit15
    uartTx.carUnLoadTimerL = gExitRxFromCar.carUnLoadTimerL; //����bit16 ~ bit23
    */
    uartTx.carUnLoadTimerH = gExit.goodCnt;  //ͳ�ƻ�������
    //uartTx.direction = gExitRxFromCar.direction;   //PC��Ҫ��������
    
    //*/
    //
    //����
    //
    /*
    uartTx.carReTimerL = 0x11;//����bit0 ~ bit7
    uartTx.carReTimerH = 0x22; //����bit8 ~ bit15
    uartTx.carUnLoadTimerL = 0x33; //����bit16 ~ bit23
    uartTx.direction = 1;   //PC��Ҫ��������
    uartTx.carUnLoadTimerH = cnt;
    cnt++;
    //*/
    
    
		//
		//����У���
		//
		uartTx.checkSum = 0;
		for(i = 0;i < (LSK_TX_LEN - 1);i++)
		{
			uartTx.checkSum += *((BYTE*)&uartTx + i);
		}
		EXIT_CRITICAL;
		
		UartSend(EXIT_PC_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);		
	}
}
//FunctionName: wasterToCar
//Description:	
//Return Type:  void
//Argument:	void
void wasterToCar(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
	
	ENTER_CRITICAL;
	//
	//֡ͷ
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = WASTER_COMMAND_ID;
  uartTx.deviceIdH = (gSwitch & 0xff00) >> 8;
  uartTx.deviceIdL = gSwitch & 0xff;
	
	//****************************************
  uartTx.carUnLoadTimerH = 0x00;
  uartTx.carUnLoadTimerL = 0x00;
  uartTx.carReTimerH = 0x00;
	//*****************************************
  
  uartTx.carReTimerL = 255;
  uartTx.carReTimerH = 255;
  //uartTx.carReTimerH = 30;
  uartTx.direction = 0;   //PC��Ҫ��������
  if(1)
  {
    uartTx.status.bits.wasterStatus = SUCCEED;
  }
	//
	//����У���
	//
	uartTx.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uartTx.checkSum += *((BYTE*)&uartTx + i);
	}
	EXIT_CRITICAL;
	
	UartSend(WASTER_CAR_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
}

//FunctionName: wasterToPc
//Description:	
//Return Type:  void
//Argument:	void
void wasterToPc(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
  
	ENTER_CRITICAL;
	//
	//֡ͷ
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = WASTER_COMMAND_ID;
  uartTx.deviceIdH = (gSwitch & 0xff00) >> 8;
  uartTx.deviceIdL = gSwitch & 0xff;
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = 0;   //PC��Ҫ��������
  
  uartTx.status.bits.wasterStatus = gMotor.status;
  
	//
	//����У���
	//
	uartTx.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uartTx.checkSum += *((BYTE*)&uartTx + i);
	}
	EXIT_CRITICAL;
  
	UartSend(PC_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
}

//FunctionName: entryToCar
//Description:	
//Return Type:  void
//Argument:	void
void entryToCar(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
  
  ENTER_CRITICAL;
	//
	//֡ͷ
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = ENTRY_COMMAND_ID;
  uartTx.deviceIdH = gEntryRxFormPc.deviceIdH;
  //uartTx.deviceIdH = 0x3; 
  uartTx.deviceIdL = gEntryRxFormPc.deviceIdL;
  //uartTx.deviceIdL = 0xB6;
  uartTx.carReTimerL= gEntryRxFormPc.carReTimerL;
  uartTx.carReTimerH= gEntryRxFormPc.carReTimerH;
  uartTx.carUnLoadTimerL= gEntryRxFormPc.carUnLoadTimerL;
  uartTx.carUnLoadTimerH= gEntryRxFormPc.carUnLoadTimerH;
  
  
  
  
	//if(MOTOR_COMM_UART_EN)
  //	{
  //		uartTx.carReTimerL = gEntryRxFormPc.carReTimerL;
  //  	uartTx.carReTimerH = gEntryRxFormPc.carReTimerH;
  //	}
  //	else
  //	{
  //		uartTx.carReTimerL = LODER_OP_TIMER_BASE * 1 ;
  //  	uartTx.carReTimerH = MOTOR_V_BASE * 0.95;		
	//}  
	
	//uartTx.carReTimerH = 200;
  uartTx.direction = gEntryRxFormPc.direction;   //PC��Ҫ��������
  
  if(gEntry.goodsReady)
  {
    uartTx.status.all = TRUE;  //2016.1.18��Ϊ����״̬
  }
	else
	{
		uartTx.status.all = FALSE;
	}
	//
	//����У���
	//
	uartTx.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uartTx.checkSum += *((BYTE*)&uartTx + i);
	}
  EXIT_CRITICAL;
  UartSend(ENTRY_CAR_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
}

//FunctionName: entryToPc
//Description:	
//Return Type:  void
//Argument:	void
void entryToPc(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
	
	ENTER_CRITICAL;
	//
	//֡ͷ
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = ENTRY_COMMAND_ID;
  uartTx.deviceIdH = gEntryRxFormPc.deviceIdH;
  uartTx.deviceIdL = gEntryRxFormPc.deviceIdL;
  uartTx.carUnLoadTimerH = gEntryRxFormPc.carUnLoadTimerH;
  uartTx.carUnLoadTimerL = gEntryRxFormPc.carUnLoadTimerL;
  uartTx.carReTimerH = gEntryRxFormPc.carReTimerH;
  uartTx.carReTimerL = gEntryRxFormPc.carReTimerL;
  uartTx.direction = gEntryRxFormPc.direction;   //PC��Ҫ��������
  //if((gEntryRxFormCar.deviceIdH == gEntryRxFormPc.deviceIdH) && (gEntryRxFormCar.deviceIdL == gEntryRxFormPc.deviceIdL))
  
  
  
	
	//uartTx.status.all = gEntryTxEn;
	
	//uartTx.status.all = gEntry.entryToPcCnt;  //����2015-4-29
	if(gEntry.goodsOk)
    //if(gEntry.goodsOkCnt > 100)
	{
		uartTx.status.all = 1; //����2015-4-29
	}
	else
	{
		uartTx.status.all = 0;
	}
	//
	//����У���
	//
	uartTx.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uartTx.checkSum += *((BYTE*)&uartTx + i);
	}
	EXIT_CRITICAL;
	
	UartSend(PC_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
}

//FunctionName: entryToPlc
//Description:	
//Return Type:  void
//Argument:	void
void entryToPlc(void)
{
	LSK_TX uartTx;
	BYTE i = 0;
	
	ENTER_CRITICAL;
	//
	//֡ͷ
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = ENTRY_COMMAND_ID;
  uartTx.deviceIdH = gEntryRxFormPc.deviceIdH;
  uartTx.deviceIdL = gEntryRxFormPc.deviceIdL;
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = gEntryRxFormPc.direction;   //PC��Ҫ��������
  //if((gEntryRxFormCar.deviceIdH == gEntryRxFormPc.deviceIdH) && (gEntryRxFormCar.deviceIdL == gEntryRxFormPc.deviceIdL))
  
  
  
	
	//uartTx.status.all = gEntryTxEn;
	
	//uartTx.status.all = gEntry.entryToPcCnt;  //����2015-4-29
	uartTx.status.all = gEntry.goodsOk; //2016-1-18
	
	//
	//����У���
	//
	uartTx.checkSum = 0;
	for(i = 0;i < (LSK_TX_LEN - 1);i++)
	{
		uartTx.checkSum += *((BYTE*)&uartTx + i);
	}
	EXIT_CRITICAL;
	
	UartSend(PC_UART,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);
}

//FunctionName: UartSendDada
//Description:	
//Return Type:  void
//Argument:	INT8U CommandType
void UartSendDada(INT8U CommandType)
{
	switch(CommandType)
	{
  case ENTRY_TO_PC: 
		{
			entryToPc();
			break;
		}
    
  case ENTRY_TO_CAR: 
		{
			entryToCar();
			break;
		}
    
  case EXIT_TO_CAR: 
		{
			ExitToCar();
			break;
		}
    
  case EXIT_TO_PC: 
		{
			ExitToPc();
			break;
		}
		
  case CAR_TO_ENTRY: 
		{
			CarToEntry();
			break;
		}
    
  case CAR_TO_PLC: 
		{
			CarToPlc();
			break;
		}
    
  case CAR_TO_WASTER: 
		{
			CarToWaster();
			break;
		}
    
  case WASTER_TO_PC: 
		{
			wasterToPc();
			break;
		}
    
  case WASTER_TO_CAR: 
		{
			wasterToCar();
			break;
		}
    
  case CAR_TO_EXIT:
		{
			CarToExit();
			break;
		}
  default:break;
	}
}

//FunctionName: UartReadDada
//Description:	
//Return Type:  void
//Argument:	void
void UartReadDada(void)
{
	UART_SOURCE *pUartSource;
  
  
#if(URAT_DMA_RX_EN)
	//if()   //DMA���յ����������ݰ�����
	{
    gUart1.UartRxLen = GET_UART1_DMA_RX_LEN;
		if(gUart1.UartRxLen >= LSK_RX_LEN)
		{
			memcpy((void*)gUart1.UartRxBuff,(void*)USART1_DMA_RX_Buf,USART1_DMA_RX_BUF_LEN);
      gUart1.UartRxLen = 0;
		}
    
		gUart2.UartRxLen = USART2_Rx_DMA_Channel->CNDTR;
		//if(gUart1.UartRxLen >= USART2_DMA_RX_BUF_LEN)
		{
			memcpy((void*)gUart2.UartRxBuff,(void*)USART2_DMA_RX_Buf,USART2_DMA_RX_BUF_LEN);
		}
	}
#endif
  
	//
	//�ϼ��ڻ�ȡָ��
	//	
	
	
  if(gDeviceType == DEVICE_ENTRY)
  {
    //����485
    pUartSource = &gUart1;  //��ȡUART��Դ
    if(gUart1.UartRxLen >= LSK_RX_LEN)
    {
      UartFromPC(PC_UART,pUartSource); 
    }
    
    //������С���ϱ�ͨ��
    pUartSource = &gUart2;  //��ȡUART��Դ
    if(gUart2.UartRxLen >= LSK_RX_LEN)
    {
      UartFromCar(ENTRY_CAR_UART,pUartSource); 
    }
  }
  
	//
	//���ڻ�ȡָ��
	//	 
	
  
  if(gDeviceType == DEVICE_EXIT)
  {
    //����485
    pUartSource = &gUart1;  //��ȡUART��Դ
    if(gUart1.UartRxLen >= LSK_RX_LEN)
    {
      UartFromKey(PC_UART,pUartSource); 
      UartFromPC(PC_UART,pUartSource);
    }
    
    //������С���ϱ�ͨ��
    pUartSource = &gUart2;  //��ȡUART��Դ
    if(gUart2.UartRxLen >= LSK_RX_LEN)
    {
      UartFromCar(CAR_EXIT_UART,pUartSource); 
      
    }
  }
  
  //
  //С����ȡָ��
  //
  if(gDeviceType == DEVICE_CAR )
  {
    if(gUart2.UartRxLen >= LSK_RX_LEN)
    {
      //if(!gCar.sleepEn)      //����ȷ��ȡ����λ��ָ�����
      {
        pUartSource = &gUart2;  //��ȡUART��Դ
        UartFromEntry(ENTRY_CAR_UART,pUartSource);
        
        //��⵽�¼��ڣ��ڴ���
        UartFromExit(EXIT_UART,pUartSource);
	      
        //δ��⵽�¼��ڣ��ڴ���
        UartFromWaster(WASTER_CAR_UART,pUartSource);
      }
      
    }
  }
  
  //
  //�����ڻ�ȡָ��
  //
  if(gDeviceType == DEVICE_WASTER)
  {
    
    //pUartSource = &gUart1;  //��ȡUART��Դ
    //UartFromPC(PC_UART,pUartSource);
    
    //�ϱ�С�����¼��ڵ�״̬
    pUartSource = &gUart2;  //��ȡUART��Դ
    if(gUart2.UartRxLen >= LSK_RX_LEN)
    {
      UartFromCar(WASTER_CAR_UART,pUartSource);
      
    }
  }
}

//FunctionName: GetUartRx
//Description:	
//Return Type:  void
//Argument:	INT8U UartId,UART_SOURCE *UartSource
BOOL GetUartRx(INT8U UartId,UART_SOURCE *UartSource)
{	
	switch(UartId)
	{
    
  case UART1:
		{	
#if UART1_EN
			UartSource = &gUart1; //��ȡUART0����Դ
#endif
			break;
		}
    
  case UART2:
		{	
#if UART2_EN
			UartSource = &gUart2; //��ȡUART1����Դ
#endif
			break;
		}
    
  case UART3:
		{	
#if UART3_EN
			UartSource = &gUart3; //��ȡUART2����Դ
#endif
			break;
		}
    
  case UART4:
		{	
#if UART4_EN
			UartSource = &gUart4; //��ȡUART3����Դ
#endif
			break;
		}
    
  case UART5:
		{
#if UART5_EN	
			UartSource = &gUart5; //��ȡUART4����Դ
#endif
			break;
		}
  default: FALSE ;
	}
  
	return	TRUE;
}

//FunctionName: UartTimeout
//Description:	
//Return Type:  void
//Argument:	void
void UartTimeout(void)
{
	//
	//Ӳ��UART0��ʱ����
	//
	if(gUart1.UartRxLen) //�Ѿ�����1���ֽ�
	{
		gUart1.gUartTimeOutCnt++;
		if(gUart1.gUartTimeOutCnt > UART1_TIMEOUT_CNT) 	//�������һ����Ϊ��ʱ������ 
		{
			gUart1.UartRxLen = 0;			//UART���ջ������������
			gUart1.gUartTimeOutCnt = 0;		//���ݰ���ʱ����������
			memset((void*)gUart1.UartRxBuff,0,sizeof(LSK_RX_LEN));  //��ʱ���ڻ���������
		}	
	}
	
  
  if(gUart2.UartRxLen) //�Ѿ�����1���ֽ�
	{
		gUart2.gUartTimeOutCnt++;
		if(gUart2.gUartTimeOutCnt > UART2_TIMEOUT_CNT) 	//�������һ����Ϊ��ʱ������ 
		{
			gUart2.UartRxLen = 0;			//UART���ջ������������
			gUart2.gUartTimeOutCnt = 0;		//���ݰ���ʱ����������
			memset((void*)gUart2.UartRxBuff,0,sizeof(LSK_RX_LEN));  //��ʱ���ڻ���������
		}	
	}   
	
  if(gUart3.UartRxLen) //�Ѿ�����1���ֽ�
	{
		gUart3.gUartTimeOutCnt++;
		if(gUart3.gUartTimeOutCnt > UART3_TIMEOUT_CNT) 	//�������һ����Ϊ��ʱ������ 
		{
			gUart3.UartRxLen = 0;			//UART���ջ������������
			gUart3.gUartTimeOutCnt = 0;		//���ݰ���ʱ����������
			memset((void*)gUart3.UartRxBuff,0,sizeof(LSK_RX_LEN)); //��ʱ���ڻ���������
		}	
	}
}




