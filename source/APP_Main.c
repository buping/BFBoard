#include "APP_Main.h"
#include "uart.h"
#include "APP_Motor.h"
#include"PWM.h"
void main(void)
{
	InitSys();
	//memSet();
	sysSelfCheck();
	initUart();  //自检完后重新初始化串口
	//*    
	LED_ON;  
	Delay_ms(2 * 1000);
	LED_OFF;
	GREEN_LED_OFF;


	 
#if (WATCHDOG_EN)
	enWatchdog();
#endif
	LcdMain();

	while(1)
	{
		EALLOW;
		if(gSysFlg0.Bits.TimerIntFlg)
		{
			//CAR_LOADING_NO;
			//CAR_LOADING_YES;
#if(WATCHDOG_EN)
			CLRWD;
#endif	
			//
			//获取串口指令
			//
			//if(gMotor.status==SUCCEED)
			//{
				ENTER_CRITICAL;
				UartReadDada();
				EXIT_CRITICAL;           
				//hardwareTest(); 
				taskSchdule();
			//}
			
			gSysFlg0.Bits.TimerIntFlg = 0;
		}
        
	EDIS;	
	}
}


//===========================================================================
// No more.
//===========================================================================

