#include "APP_Main.h"
#include "uart.h"
#include "APP_Motor.h"
#include"PWM.h"
void main(void)
{
	InitSys();
	//memSet();
	sysSelfCheck();
	initUart();  //�Լ�������³�ʼ������
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
			//��ȡ����ָ��
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

