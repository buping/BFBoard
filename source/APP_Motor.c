//FileName:APP_Motor.c
//Description:
//Creation:
//Editor:

#include "APP_Motor.h"

/*
void testMotor(void)
{

ENTER_CRITICAL;
//setMotor(MOTOR_N_R,500);
if(motorSpeed < 3000 && motorFlag)
{
motorSpeed += 1;
                    }
                    else
{
motorFlag = 0;
                    }

if(motorSpeed > 0 && motorFlag ==0)
{
motorSpeed -= 1;
                    }
                    else
{
motorFlag = 1;
                    }
setMotor(MOTOR_P_R,motorSpeed);
motorRun();
EXIT_CRITICAL;

//Delay_us(10000);
//motorStop();
//Delay_us(1000000);
//motorStop();
}
*/

// Function   name   :  setMotorIncAcc
// Description       : 
// Return     type   : void
// Argument         : INT32U incAccTime
void setMotorIncAcc(INT32U incAccTime)
{
	MOTOR_COMMAND command;
	BYTE i = 0,checkSum = 0;
	INT16U data;
	BYTE *P;
	
	command.start = MOTOR_START;
	command.address1 = MOTOR_ADDR1;
  command.address2 = MOTOR_ADDR2;
	command.command = MOTOR_INC_ACC;
  command.data0 = (BYTE)(incAccTime >> 16);
	command.data1 = (BYTE)(incAccTime >> 8);
	command.data2 = (BYTE)incAccTime;
  
  P = (BYTE*)&command.address1;
	for(i = 0;i < sizeof(MOTOR_COMMAND) - M_N_CHECK_LEN;i++)
	{
		checkSum += *(P + i);
	}
	checkSum = (0xff - checkSum) + 1;
  
	command.checkSum = checkSum;
	command.end = MOTOR_END;
	UartSend(CAR_MOTOR_UART,(BYTE*)&command,UART_MOTOR_LEN);
}

// Function   name   :  setMotorDecAcc
// Description       : 
// Return     type   : void
// Argument         : INT32U incAccTime
void setMotorDecAcc(INT32U decAccTime)
{
	MOTOR_COMMAND command;
	BYTE i = 0,checkSum = 0;
	INT16U data;
	BYTE *P;
	
	command.start = MOTOR_START;
	command.address1 = MOTOR_ADDR1;
  command.address2 = MOTOR_ADDR2;
	command.command = MOTOR_DEC_ACC;
  command.data0 = (BYTE)(decAccTime >> 16);
	command.data1 = (BYTE)(decAccTime >> 8);
	command.data2 = (BYTE)decAccTime;
  
  P = (BYTE*)&command.address1;
	for(i = 0;i < sizeof(MOTOR_COMMAND) - M_N_CHECK_LEN;i++)
	{
		checkSum += *(P + i);
	}
	checkSum = (0xff - checkSum) + 1;
  
	command.checkSum = checkSum;
	command.end = MOTOR_END;
	UartSend(CAR_MOTOR_UART,(BYTE*)&command,UART_MOTOR_LEN);
}

// Function   name   :  setMotorDistance
// Description       : 
// Return     type   : void
// Argument         : INT32U distance
void setMotorDistance(INT32U distance,INT32U indirect)
{
	MOTOR_COMMAND command;
	BYTE i = 0,checkSum = 0;
	INT16U data;
	BYTE *P;
	
	command.start = MOTOR_START;
	command.address1 = MOTOR_ADDR1;
  command.address2 = MOTOR_ADDR2;
	if(indirect)
	{
		command.command = MOTOR_DISTANCE;
	}
	else
	{
		command.command = MOTOR_DISTANCE + 0x1; //A1
	}
  command.data0 = (BYTE)(distance >> 16);
	command.data1 = (BYTE)(distance >> 8);
	command.data2 = (BYTE)distance;
  
  P = (BYTE*)&command.address1;
	for(i = 0;i < sizeof(MOTOR_COMMAND) - M_N_CHECK_LEN;i++)
	{
		checkSum += *(P + i);
	}
	checkSum = (0xff - checkSum) + 1;
  
	command.checkSum = checkSum;
	command.end = MOTOR_END;
	UartSend(CAR_MOTOR_UART,(BYTE*)&command,UART_MOTOR_LEN);
}


// Function   name   :  motorRun
// Description       : 
// Return     type   : void
// Argument         : void
void motorRun(void)
{
	MOTOR_COMMAND command;
  
	BYTE i = 0,checkSum = 0;
	INT16U data;
	BYTE *P;
	
	command.start = MOTOR_START;
	command.address1 = MOTOR_ADDR1;
  command.address2 = MOTOR_ADDR2;
	command.command = MOTOR_STATUS;
  command.data0 = 0x00;
	command.data1 = 0x00;
	command.data2 = MOTOR_RUN;
  
  P = (BYTE*)&command.address1;
	for(i = 0;i < sizeof(MOTOR_COMMAND) - M_N_CHECK_LEN;i++)
	{
		checkSum += *(P + i);
	}
	checkSum = (0xff - checkSum) + 1;
	
	command.checkSum = checkSum;
	command.end = MOTOR_END;
	UartSend(CAR_MOTOR_UART,(BYTE*)&command.start,UART_MOTOR_LEN);
}

// Function   name   :  motorRun
// Description       : 
// Return     type   : void
// Argument         : void
void motorStop(void)
{
	MOTOR_COMMAND command;
  
	BYTE i = 0,checkSum = 0;
	INT16U data;
	BYTE *P;
	
	command.start = MOTOR_START;
	command.address1 = MOTOR_ADDR1;
  command.address2 = MOTOR_ADDR2;
	command.command = MOTOR_STATUS;
  command.data0 = 0x00;
	command.data1 = 0x00;
	command.data2 = MOTOR_STOP;
  P = (BYTE*)&command.address1;
  
	for(i = 0;i < sizeof(MOTOR_COMMAND) - M_N_CHECK_LEN;i++)
	{
		checkSum += *(P + i);
	}
	checkSum = (0xff - checkSum) + 1;
	
	command.checkSum = checkSum;
	command.end = MOTOR_END;
	UartSend(CAR_MOTOR_UART,(BYTE*)&command.start,UART_MOTOR_LEN);
}

// Function   name   :  setMotorDirect
// Description       : 
// Return     type   : void
// Argument         : BOOL indirect,INT32U speed
void setMotor(INT32U speed,INT32U decAccTime,INT32U incAccTime,BOOL indirect,INT32U distance)
{   
  static INT16U cnt = 0 ;
  static INT16U oldSpeed = 0;
  static INT16U txCnt = 0;
  
  
	if(oldSpeed != speed)
  {
    
		//ENTER_CRITICAL;
		cnt++;
		
		if(gDeviceType == DEVICE_CAR)
		{
      UART1_TX_EN;
    }
    
		if(speed == 0)
    {
	    /*
      motorStop();
      
			setMotorDirect(0);
			
			setMotorSpeed(0);
			
			//*/
			oldSpeed = speed;
		}
		else
		{
			//motorRun();
			
			{
				switch(txCnt)
				{
        case 0:
          {
						setMotorSpeed(speed); break;
					}
        case (1 * MOTOR_SET_INTERVAL):
					{
						setMotorSpeed(speed);break;
					}
        case (2 * MOTOR_SET_INTERVAL):
					{
						setMotorDecAcc(decAccTime);break;
					}
        case (3 * MOTOR_SET_INTERVAL):
					{
						setMotorIncAcc(incAccTime);break;
					}
        case (4 * MOTOR_SET_INTERVAL):
					{
						setMotorDirect(indirect);break;
					}
					
					//最后一条指令发两遍
        case (5 * MOTOR_SET_INTERVAL):
					{
						setMotorDistance(distance,indirect);break;
					}
        case (6 * MOTOR_SET_INTERVAL):
					{
						setMotorDistance(distance,indirect);break;
					}
        case (7 * MOTOR_SET_INTERVAL):
					{
						setMotorDistance(distance,indirect);
						oldSpeed = speed;
						break;
					}
					
        default: break;
				}
        
        txCnt++;
			}
		}
    
		
		//UART1_RX_EN;
		//EXIT_CRITICAL;
  }
  else
	{
		txCnt = 0;
	}
	
  if(cnt > 50)
  {
    cnt = 0;
  }
}

// Function   name   :  setMotorDirect
// Description       : 
// Return     type   : void
// Argument         : BOOL indirec
void setMotorDirect(BOOL indirect)
{
	MOTOR_COMMAND command;
  
	BYTE i = 0,checkSum = 0;
	INT16U data;
	BYTE *P;
	
	command.start = MOTOR_START;
	command.address1 = MOTOR_ADDR1;
  command.address2 = MOTOR_ADDR2;
	command.command = MOTOR_DIRECT;
  command.data0 = 0x00;
	command.data1 = 0x00;
	command.data2 = (BYTE)indirect;
  P = (BYTE*)&command.address1;
  
	for(i = 0;i < sizeof(MOTOR_COMMAND) - M_N_CHECK_LEN;i++)
	{
		checkSum += *(P + i);
	}
	checkSum = (0xff - checkSum) + 1;
	
	command.checkSum = checkSum;
	command.end = MOTOR_END;
	UartSend(CAR_MOTOR_UART,(BYTE*)&command.start,UART_MOTOR_LEN);
}

// Function   name   :  setMotorSpeed
// Description       : 
// Return     type   : void
// Argument         : INT32U speed
void setMotorSpeed(INT32U speed)
{
	MOTOR_COMMAND command;
	BYTE i = 0,checkSum = 0;
	INT16U data;
	BYTE *P;
	
	command.start = MOTOR_START;
	command.address1 = MOTOR_ADDR1;
  command.address2 = MOTOR_ADDR2;
	command.command = MOTOR_V;
  command.data0 = (BYTE)(speed >> 16);
	command.data1 = (BYTE)(speed >> 8);
	command.data2 = (BYTE)speed;
  
  P = (BYTE*)&command.address1;
	for(i = 0;i < sizeof(MOTOR_COMMAND) - M_N_CHECK_LEN;i++)
	{
		checkSum += *(P + i);
	}
	checkSum = (0xff - checkSum) + 1;
  
	command.checkSum = checkSum;
	command.end = MOTOR_END;
	UartSend(CAR_MOTOR_UART,(BYTE*)&command,UART_MOTOR_LEN);
}

// Function   name   :  setMotorStatus
// Description       : 
// Return     type   : void
// Argument         : BOOL status
void setMotorStatus(BOOL status)
{
	MOTOR_COMMAND command;
	BYTE i = 0,checkSum = 0;
	INT16U data;
	BYTE *P;
	
	command.start = MOTOR_START;
	command.address1 = MOTOR_ADDR1;
  command.address2 = MOTOR_ADDR2;
	command.command = MOTOR_STATUS;
  command.data0 = 0x00;
	command.data1 = 0x00;
	command.data2 = (BYTE)status;
  P = (BYTE*)&command.address1;
	for(i = 0;i < sizeof(MOTOR_COMMAND) - M_N_CHECK_LEN;i++)
	{
		checkSum += *(P + i);
	}
	checkSum = (0xff - checkSum) + 1;
	
	command.checkSum = checkSum;
	command.end = MOTOR_END;
	UartSend(CAR_MOTOR_UART,(BYTE*)&command.start,UART_MOTOR_LEN);
}

// Function   name   :  readMotorSpeed
// Description       : 
// Return     type   : void
// Argument         : BOOL readCommand
void readMotorSpeed(BOOL readCommand)
{
	MOTOR_COMMAND command;
	BYTE i = 0,checkSum = 0;
	INT16U data;
	BYTE *P;
	
	command.start = MOTOR_START;
	command.address1 = MOTOR_ADDR1;
  command.address2 = MOTOR_ADDR2;
	command.command = readCommand;
  command.data0 = 0x00;
	command.data1 = 0x00;
	command.data2 = 0x00;
  P = (BYTE*)&command.address1;
	
	for(i = 0;i < sizeof(MOTOR_COMMAND) - M_N_CHECK_LEN;i++)
	{
		checkSum += *(P + i);
	}
	checkSum = (0xff - checkSum) + 1;
	
	command.checkSum = checkSum;
	command.end = MOTOR_END;
	UartSend(CAR_MOTOR_UART,(BYTE*)&command.start,UART_MOTOR_LEN);
}


