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
  //上件口从上件台获取数据
  //
	//if(gEntry.rxFromPcDis == FALSE)  //如果成功接收了命令，但还没处理完，不接收新的命令
	{
		//if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == PC_COMMAND_ID))
		if((UartSource->UartRxBuff[0] == FROM_PC_HEAD0) && (UartSource->UartRxBuff[1] == ENTRY_COMMAND_ID))  // 暂时与总控台相连
		{
      //copy data
      pData = (BYTE*)&uartRx;
			for(i = 0;i < sizeof(LSK_RX);i++)
			{
				*(pData + i) = UartSource->UartRxBuff[i];
			}
      
			//UartSource->UartRxBuff[0] = 0;
      pData = (BYTE*)&uartRx;  
      
			//计算校验和
			//pData = (BYTE*)&gPlatRx.Code;(LSK_RX_LEN - 1)
      checkSum = 0;
			checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
			if(uartRx.checkSum == checkSum)
			{
        if(gDeviceType == DEVICE_ENTRY)   //上件口
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
          
					gEntry.rxFromPcDis = TRUE;   //如果上件口接到允许上货的命令
        }
        
        //使用完后清内存
        
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
    
    //计算校验和
    //pData = (BYTE*)&gPlatRx.Code;(LSK_RX_LEN - 1)
    checkSum = 0;
    checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
    if(uartRx.checkSum == checkSum)
    {
      if(gDeviceType == DEVICE_EXIT)   //出口
      {
        pData = (BYTE*)&uartRx;
        pDest = (BYTE*)&gExitRxPC;
        for(i = 0;i < sizeof(uartRx);i++)
        {
          *(pDest + i) = *(pData + i);
        }
        
        id =  (gExitRxPC.deviceIdH << 8) +  gExitRxPC.deviceIdL;
        if(gExit.id == id)  //接收与广播地址相符
        {
          gExit.exitToPcEn = TRUE;  //设置允许发送标志，给PC回信息
        }
        else
        {
          gExit.exitToPcEn = FALSE;
        }
        
        //使用完后清内存
        for(i = 0;i < sizeof(LSK_RX);i++)
        {
          UartSource->UartRxBuff[i] = 0;
        }
      }
    }
	}
  EXIT_CRITICAL;
  
	//
	//设置运行参数
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
		//计算校验和
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
			
      UartSource->UartRxBuff[0] = 0;//防止重复写flash
      UartSource->UartRxBuff[12] = 0;//防止重复写flash
      
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
    
    //使用完后清内存
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
    
		//计算校验和
		//pData = (BYTE*)&gPlatRx.Code;
    checkSum = 0;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      
			if(gSysFlg0.Bits.sysDebugEn)
			{
				LED_ON;   //测试用，小车接到上件口
			}
			
      //if(gCar.fromEntryDis && (gCarRxFromEntry.deviceIdH != 0 && gCarRxFromEntry.deviceIdL != 0 )) //如果是小车，且已经接受到了
      if(gCar.fromEntryDis) //如果是小车，且已经接受到了
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
          gCar.lodeOpTimeK = gCarRxFromEntry.carReTimerL;  //调整旋转时间 
        }
					  else
          {
          gCar.lodeOpTimeK = LODER_OP_TIMER_BASE;  //调整旋转时间 
        }
          */                  
          
          gCar.direction = gCarRxFromEntry.direction; //方向
          
          
          if(((gCar.goodOk == FALSE) && (gCarRxFromEntry.status.all == TRUE)) && (gCar.idFromEntry != 0)) //小车无货
            //if(gCar.goodOk == FALSE) //小车无货
          {
            gCar.fromEntryDis = TRUE;  	 //已经上货确认，不允许再次接受
            gCar.uartTxCnt = 1;  
            gMotor.loadDeyEn = TRUE;
            gCarRxFromEntry.status.all = 0;
          }
          else
          {
            //可以给PC及PLC回传ready了
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
      
			//使用完后清内存
			for(i = 0;i < sizeof(LSK_RX);i++)
			{
        UartSource->UartRxBuff[i] = 0;
			}
    }
	}
  
  //*
	//红外口设置运行参数
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
		//计算校验和
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
      UartSource->UartRxBuff[0] = 0;//防止重复写flash
      UartSource->UartRxBuff[11] = 0;//防止重复写flash
      UartSource->UartRxBuff[12] = 0;//防止重复写flash
      
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
//Description:	人工按钮控制，通过PC机转发
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
    
		//计算校验和
		//pData = (BYTE*)&gPlatRx.Code;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      
			if(gSysFlg0.Bits.sysDebugEn)
			{
				LED_OFF;   //测试用，小车接到上件口
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
        //是某一侧发出的指令，而且发出的指令有效才执行
        //
				if(gExitRxKey.status.key.exitUnlodeEn)
				{
          if((gExitRxKey.direction & EXIT_IN_CTL) && ((gExitRxKey.direction & EXIT_IN_OUT_DIRCTION) == FALSE))  //内侧出口
          {
				 		gExit.inExitToCarEn = TRUE;
				 	}
				 	
          if((gExitRxKey.direction & EXIT_OUT_CTL) && (gExitRxKey.direction & EXIT_IN_OUT_DIRCTION))  //外侧出口
          {
				 		gExit.outExitToCarEn = TRUE;
				 	}
				 	gExit.goodCnt = 0;   //允许下货时，货物计数器清零
				}
        
        
				
        
				if(gExitRxKey.status.key.exitUnlodeDis)
				{
          if(((gExitRxKey.direction & EXIT_OUT_CTL) == FALSE) && ((gExitRxKey.direction & EXIT_IN_OUT_DIRCTION) == FALSE))  //内侧出口
          {
				 		gExit.inExitToCarEn = FALSE;
				 	}
				 	
          if(((gExitRxKey.direction & EXIT_IN_CTL) == FALSE) && (gExitRxKey.direction & EXIT_IN_OUT_DIRCTION))  //外侧出口
          {
				 		gExit.outExitToCarEn = FALSE;
				 	}
				}
			}
      
			//使用完后清内存
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
    
		//计算校验和
		//pData = (BYTE*)&gPlatRx.Code;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      
			if(gSysFlg0.Bits.sysDebugEn)
			{
				LED_OFF;   //测试用，小车接到上件口
			}
      
      pData = (BYTE*)&uartRx;
      pDest = (BYTE*)&gCarRxFromExit;
      for(i = 0;i < sizeof(LSK_RX);i++)
      {
        *(pDest + i) = *(pData + i);
      }
      
      gCar.idFromExit = (gCarRxFromExit.deviceIdH << 8) + gCarRxFromExit.deviceIdL;
      
			if((gCarRxFromExit.direction & EXIT_IN_CTL)) //出口下货控制
      {
        gCar.direction |= EXIT_IN_CTL;
			}
			else
			{
				gCar.direction &= ~EXIT_IN_CTL;
			}
			
			if((gCarRxFromExit.direction & EXIT_OUT_CTL)) //出口下货控制
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
        gCar.unLodeOpTimeK =  gCarRxFromExit.carReTimerL;  //调整旋转时间 
			}
			else
			{
        gCar.unLodeOpTimeK = UNLODER_OP_TIMER_BASE;  //调整旋转时间 
			}    
      
      if((gCar.idFromExit == gCar.idFromEntry) && (((gCar.direction & EXIT_IN_CTL) && ((gCar.direction & CAR_EXIT_DIR) == FALSE)) || ( (gCar.direction & EXIT_OUT_CTL) && (gCar.direction & CAR_EXIT_DIR))) )   //找到出口
      {
        if(gCar.goodOk == TRUE)  //还未卸货
        {
          //gCarTx.carUnLoadTimerH = gCarRxFromExit.deviceIdH;
          //gCarTx.carUnLoadTimerL = gCarRxFromExit.deviceIdL;
          gCar.fromEntryDis = 0; //允许下次接收
          gCarTx.status.bits.exitStatus = TRUE;
          
          
          //SET_MOTO(gCar.speed);
          gMotor.unLoadDeyEn  = TRUE;
        }
      }
      
			//使用完后清内存
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
    
		//计算校验和
		//pData = (BYTE*)&gPlatRx.Code;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      
			if(gSysFlg0.Bits.sysDebugEn)
			{
				LED_OFF;   //测试用，小车接到上件口
			}
      
      pData = (BYTE*)&uartRx;
      pDest = (BYTE*)&gEntryRxFormPlc;
      for(i = 0;i < sizeof(LSK_RX);i++)
      {
        *(pDest + i) = *(pData + i);
      }
      
      //有货来了
      /*
      if(gEntryRxFormPlc.status)
      {
      
    }
			else
			{
    }
      
			//小车上件运行时间
			if()
			{
    }
			else
			{
    }
			
      */
			//使用完后清内存
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
    
		//计算校验和
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
        
        gEntry.carStatus = gEntryRxFormCar.status.all;   //从小车接到了一个ok指令
        
        if(gEntryRxFormCar.status.all == TRUE)  //接到小车有货
        {
          gEntry.CarEnLoad = TRUE; //不能上货	
        }
        else
        {  
          gEntry.CarEnLoad = FALSE;  //可以上货
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
          gExit.uartToCarCnt = 1;  //允许下件发送
        }
        
        //  上件台编号                            货物流水号高                 货物流水号低
        goodId = (uartRx.carReTimerL << 16) + (uartRx.carUnLoadTimerL << 8) + uartRx.carReTimerH;
        
        //if((id != 0) && (id == gExit.id) && (uartRx.status.all == TRUE))   //目的地与出口相符
        
        if((id != 0) && (id == gExit.id))
        {
          pData = (BYTE*)&uartRx;
          pDest = (BYTE*)&gExitRxFromCar;
          for(i = 0;i < sizeof(LSK_RX);i++)
          {
            *(pDest + i) = *(pData + i);
          }
          
          //  上件台编号                                  货物流水号高                                 货物流水号低
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
                      gGoods.goodsId[i] = gGoods.goodsId[i - 1];        //货物ID
                      gGoods.exitDirect[i] = gGoods.exitDirect[i - 1];  //出口方向  
                      
                    }
                  }
                  gGoods.cnt++;  //用于串口发送指令次数
                  
                }
                gGoods.goodsId[0] = goodId;
                gGoods.exitDirect[0] = gExitRxFromCar.direction;
              }
              gExit.goodCnt++;
              gExit.oldGoodId = gExit.goodId;
            }
            //重号货物由上位机解决
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
      //正确接收后清零
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
    
		//计算校验和
		//pData = (BYTE*)&gPlatRx.Code;
    checkSum = 0;
		checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
		if(uartRx.checkSum == checkSum)
		{
      //计算校验和
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
        gCar.unLodeOpTimeK = gCarRxFromWaster.carReTimerL;  //调整旋转时间 
			}
			else
			{
        gCar.unLodeOpTimeK = LODER_OP_TIMER_BASE;  //调整旋转时间 
			}   
			
      //if(gCar.goodOk == TRUE || gMotor.status == FAILED)  //还未卸货或者电机坏，在这试图修复一次
      {
        
        //gCarTx.carUnLoadTimerH = gCarRxFromExit.deviceIdH;
        //gCarTx.carUnLoadTimerL = gCarRxFromExit.deviceIdL;
        gCar.fromEntryDis = 0; //允许下次接收
        //gCarTx.status.bits.exitStatus = TRUE;
        
        
        //SET_MOTO(gCar.speed);
        gMotor.unLoadDeyEn  = TRUE;
        
        //if(gCarRxFromExit.direction == 1)
        {
          //MOTO_P_EN;
          //MOTO_N_DIS; 
        }
      }
			
			//使用完后清内存
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
	//帧头
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = CAR_COMMAND_ID;
  
  //自身的
  uartTx.deviceIdH = gCarRxFromEntry.deviceIdH;
  uartTx.deviceIdL = gCarRxFromEntry.deviceIdL;
  
	//目的地ID,下件口赋值
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = 0;   //PC需要给出方向
  
  uartTx.carUnLoadTimerH = gSwitch & 0xFF;
  
  //uartTx.status.all = gMotor.status;
  
	/*
  if(gCar.goodOk)   //小车有货
  { 
  uartTx.status.all = TRUE; 		
}
                  else
  { 
  if(gMotor.status == SUCCEED)
  {
  uartTx.status.all = FALSE; //通报PLC已经找到可运行小车
}
					   else
  {
  uartTx.status.all = TRUE;
}
}
	*/			  
  if(gCar.uartTxCnt)   //小车无货，况且接到上件货已经装备好
  { 
    uartTx.status.all = TRUE; 		
  }
  else
  { 
    uartTx.status.all = FALSE;  
  }				  
	//
	//计算校验和
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
	//帧头
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = CAR_COMMAND_ID;
  
  //货物目的地，也就是出口id
  
  uartTx.deviceIdH = gCarRxFromEntry.deviceIdH;
  uartTx.deviceIdL = gCarRxFromEntry.deviceIdL;
  
  id = (gCarRxFromEntry.deviceIdH << 8) + gCarRxFromEntry.deviceIdL;
  
  if(id = 0)
  {
    id = 0;
  }
  else
  {
		//目的地ID,下件口赋值
    uartTx.carUnLoadTimerH = gCarRxFromEntry.carUnLoadTimerH;
    uartTx.carUnLoadTimerL = gCarRxFromEntry.carUnLoadTimerL;
    uartTx.carReTimerH = gCarRxFromEntry.carReTimerH;
    uartTx.carReTimerL = gCarRxFromEntry.carReTimerL;
    uartTx.direction = gCarRxFromEntry.direction;   //PC需要给出方向
    uartTx.status.all= gMotor.status;
    
    uartTx.carUnLoadTimerH = gSwitch & 0xFF;
  }
	//
	//计算校验和
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
	//帧头
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = CAR_COMMAND_ID;
  
  //自身的ID
  uartTx.deviceIdH = (gSwitch & 0xff00) >> 8;
  uartTx.deviceIdL = gSwitch & 0xff;
  
	//目的地ID,下件口赋值
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = 0;   //PC需要给出方向
  uartTx.status.all = gMotor.status;
	//
	//计算校验和
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
	//帧头
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = CAR_COMMAND_ID;
  uartTx.deviceIdH = 0;
  uartTx.deviceIdL = 0;
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = 0;   //PC需要给出方向
  uartTx.status.all = SUCCEED;
	//
	//计算校验和
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
		//帧头
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
    
		if(gExit.inExitToCarEn) //内侧出口
		{
      uartTx.direction |= EXIT_IN_CTL;   //PC需要给出方向	
		}
		else
		{
      uartTx.direction &= ~EXIT_IN_CTL; 
		}
    
		if(gExit.outExitToCarEn)    //外侧出口
		{
      uartTx.direction |= EXIT_OUT_CTL;   //PC需要给出方向	
		}
		else
		{
      uartTx.direction &= ~EXIT_OUT_CTL; 
		}
		
		//*
	 	//uartTx.direction = 0xff;  //调试
		//*/
    
    uartTx.status.bits.pcStatus = SUCCEED;
		//
		//计算校验和
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
  //if(gExit.exitToPcEn)  //PC广播呼叫出口成功后回数据
  {
		//
		//帧头
		//
		uartTx.packetHead0 = LSK_TX_HEAD0;
    uartTx.instructionId = EXIT_COMMAND_ID;
    uartTx.deviceIdH = (gSwitch >> 8) & 0xff;
    uartTx.deviceIdL = gSwitch & 0xff;
    uartTx.direction = 0;
    uartTx.status.all = 0;
    
		//************************
    
		//*
    
    //  上件台编号                                  货物流水号高                                 货物流水号低
    //goodId = (gExitRxFromCar.carReTimerL << 16) + (gExitRxFromCar.carUnLoadTimerL << 8) + gExitRxFromCar.carReTimerH;
		
		if(gGoods.cnt)
		{
      uartTx.carReTimerL = (gGoods.goodsId[0] >> 16) & 0xff; //货物bit0 ~ bit7
      uartTx.carReTimerH = (gGoods.goodsId[0] >> 0) & 0xff; //货物bit8 ~ bit15
      uartTx.carUnLoadTimerL = (gGoods.goodsId[0] >> 8) & 0xff; //货物bit16 ~ bit23
      uartTx.direction = gGoods.exitDirect[0];   //PC需要给出方向
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
      uartTx.carReTimerL = 0; //货物bit0 ~ bit7
      uartTx.carReTimerH = 0; //货物bit8 ~ bit15
      uartTx.carUnLoadTimerL = 0; //货物bit16 ~ bit23
			uartTx.status.bits.pcStatus = 0;
		}
		
    
    /*
    uartTx.carReTimerL = gExitRxFromCar.carReTimerL;//货物bit0 ~ bit7
    uartTx.carReTimerH = gExitRxFromCar.carReTimerH; //货物bit8 ~ bit15
    uartTx.carUnLoadTimerL = gExitRxFromCar.carUnLoadTimerL; //货物bit16 ~ bit23
    */
    uartTx.carUnLoadTimerH = gExit.goodCnt;  //统计货物数量
    //uartTx.direction = gExitRxFromCar.direction;   //PC需要给出方向
    
    //*/
    //
    //测试
    //
    /*
    uartTx.carReTimerL = 0x11;//货物bit0 ~ bit7
    uartTx.carReTimerH = 0x22; //货物bit8 ~ bit15
    uartTx.carUnLoadTimerL = 0x33; //货物bit16 ~ bit23
    uartTx.direction = 1;   //PC需要给出方向
    uartTx.carUnLoadTimerH = cnt;
    cnt++;
    //*/
    
    
		//
		//计算校验和
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
	//帧头
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
  uartTx.direction = 0;   //PC需要给出方向
  if(1)
  {
    uartTx.status.bits.wasterStatus = SUCCEED;
  }
	//
	//计算校验和
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
	//帧头
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = WASTER_COMMAND_ID;
  uartTx.deviceIdH = (gSwitch & 0xff00) >> 8;
  uartTx.deviceIdL = gSwitch & 0xff;
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = 0;   //PC需要给出方向
  
  uartTx.status.bits.wasterStatus = gMotor.status;
  
	//
	//计算校验和
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
	//帧头
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
  uartTx.direction = gEntryRxFormPc.direction;   //PC需要给出方向
  
  if(gEntry.goodsReady)
  {
    uartTx.status.all = TRUE;  //2016.1.18改为发送状态
  }
	else
	{
		uartTx.status.all = FALSE;
	}
	//
	//计算校验和
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
	//帧头
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = ENTRY_COMMAND_ID;
  uartTx.deviceIdH = gEntryRxFormPc.deviceIdH;
  uartTx.deviceIdL = gEntryRxFormPc.deviceIdL;
  uartTx.carUnLoadTimerH = gEntryRxFormPc.carUnLoadTimerH;
  uartTx.carUnLoadTimerL = gEntryRxFormPc.carUnLoadTimerL;
  uartTx.carReTimerH = gEntryRxFormPc.carReTimerH;
  uartTx.carReTimerL = gEntryRxFormPc.carReTimerL;
  uartTx.direction = gEntryRxFormPc.direction;   //PC需要给出方向
  //if((gEntryRxFormCar.deviceIdH == gEntryRxFormPc.deviceIdH) && (gEntryRxFormCar.deviceIdL == gEntryRxFormPc.deviceIdL))
  
  
  
	
	//uartTx.status.all = gEntryTxEn;
	
	//uartTx.status.all = gEntry.entryToPcCnt;  //测试2015-4-29
	if(gEntry.goodsOk)
    //if(gEntry.goodsOkCnt > 100)
	{
		uartTx.status.all = 1; //测试2015-4-29
	}
	else
	{
		uartTx.status.all = 0;
	}
	//
	//计算校验和
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
	//帧头
	//
	uartTx.packetHead0 = LSK_TX_HEAD0;
  uartTx.instructionId = ENTRY_COMMAND_ID;
  uartTx.deviceIdH = gEntryRxFormPc.deviceIdH;
  uartTx.deviceIdL = gEntryRxFormPc.deviceIdL;
  uartTx.carUnLoadTimerH = 0;
  uartTx.carUnLoadTimerL = 0;
  uartTx.carReTimerH = 0;
  uartTx.carReTimerL = 0;
  uartTx.direction = gEntryRxFormPc.direction;   //PC需要给出方向
  //if((gEntryRxFormCar.deviceIdH == gEntryRxFormPc.deviceIdH) && (gEntryRxFormCar.deviceIdL == gEntryRxFormPc.deviceIdL))
  
  
  
	
	//uartTx.status.all = gEntryTxEn;
	
	//uartTx.status.all = gEntry.entryToPcCnt;  //测试2015-4-29
	uartTx.status.all = gEntry.goodsOk; //2016-1-18
	
	//
	//计算校验和
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
	//if()   //DMA接收到完整的数据包长度
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
	//上件口获取指令
	//	
	
	
  if(gDeviceType == DEVICE_ENTRY)
  {
    //接收485
    pUartSource = &gUart1;  //获取UART资源
    if(gUart1.UartRxLen >= LSK_RX_LEN)
    {
      UartFromPC(PC_UART,pUartSource); 
    }
    
    //可用于小车上报通道
    pUartSource = &gUart2;  //获取UART资源
    if(gUart2.UartRxLen >= LSK_RX_LEN)
    {
      UartFromCar(ENTRY_CAR_UART,pUartSource); 
    }
  }
  
	//
	//出口获取指令
	//	 
	
  
  if(gDeviceType == DEVICE_EXIT)
  {
    //接收485
    pUartSource = &gUart1;  //获取UART资源
    if(gUart1.UartRxLen >= LSK_RX_LEN)
    {
      UartFromKey(PC_UART,pUartSource); 
      UartFromPC(PC_UART,pUartSource);
    }
    
    //可用于小车上报通道
    pUartSource = &gUart2;  //获取UART资源
    if(gUart2.UartRxLen >= LSK_RX_LEN)
    {
      UartFromCar(CAR_EXIT_UART,pUartSource); 
      
    }
  }
  
  //
  //小车获取指令
  //
  if(gDeviceType == DEVICE_CAR )
  {
    if(gUart2.UartRxLen >= LSK_RX_LEN)
    {
      //if(!gCar.sleepEn)      //若正确获取过上位机指令，休眠
      {
        pUartSource = &gUart2;  //获取UART资源
        UartFromEntry(ENTRY_CAR_UART,pUartSource);
        
        //检测到下件口，在此下
        UartFromExit(EXIT_UART,pUartSource);
	      
        //未检测到下件口，在此下
        UartFromWaster(WASTER_CAR_UART,pUartSource);
      }
      
    }
  }
  
  //
  //垃圾口获取指令
  //
  if(gDeviceType == DEVICE_WASTER)
  {
    
    //pUartSource = &gUart1;  //获取UART资源
    //UartFromPC(PC_UART,pUartSource);
    
    //上报小车和下件口的状态
    pUartSource = &gUart2;  //获取UART资源
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
			UartSource = &gUart1; //获取UART0的资源
#endif
			break;
		}
    
  case UART2:
		{	
#if UART2_EN
			UartSource = &gUart2; //获取UART1的资源
#endif
			break;
		}
    
  case UART3:
		{	
#if UART3_EN
			UartSource = &gUart3; //获取UART2的资源
#endif
			break;
		}
    
  case UART4:
		{	
#if UART4_EN
			UartSource = &gUart4; //获取UART3的资源
#endif
			break;
		}
    
  case UART5:
		{
#if UART5_EN	
			UartSource = &gUart5; //获取UART4的资源
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
	//硬件UART0超时控制
	//
	if(gUart1.UartRxLen) //已经收了1个字节
	{
		gUart1.gUartTimeOutCnt++;
		if(gUart1.gUartTimeOutCnt > UART1_TIMEOUT_CNT) 	//如果超过一秒认为超时，可设 
		{
			gUart1.UartRxLen = 0;			//UART接收缓冲计数器清零
			gUart1.gUartTimeOutCnt = 0;		//数据包超时计数器清零
			memset((void*)gUart1.UartRxBuff,0,sizeof(LSK_RX_LEN));  //超时串口缓冲区清零
		}	
	}
	
  
  if(gUart2.UartRxLen) //已经收了1个字节
	{
		gUart2.gUartTimeOutCnt++;
		if(gUart2.gUartTimeOutCnt > UART2_TIMEOUT_CNT) 	//如果超过一秒认为超时，可设 
		{
			gUart2.UartRxLen = 0;			//UART接收缓冲计数器清零
			gUart2.gUartTimeOutCnt = 0;		//数据包超时计数器清零
			memset((void*)gUart2.UartRxBuff,0,sizeof(LSK_RX_LEN));  //超时串口缓冲区清零
		}	
	}   
	
  if(gUart3.UartRxLen) //已经收了1个字节
	{
		gUart3.gUartTimeOutCnt++;
		if(gUart3.gUartTimeOutCnt > UART3_TIMEOUT_CNT) 	//如果超过一秒认为超时，可设 
		{
			gUart3.UartRxLen = 0;			//UART接收缓冲计数器清零
			gUart3.gUartTimeOutCnt = 0;		//数据包超时计数器清零
			memset((void*)gUart3.UartRxBuff,0,sizeof(LSK_RX_LEN)); //超时串口缓冲区清零
		}	
	}
}




