#include "HardwareTest.h"
#include "APP_Main.h"
#include "MyPrintf.h"
#include "APP_Uart.h"
#include "App_Core.h"
#include "APP_Uart.h"
#include "math.h"
#include "APP_Motor.h"
#include "Common.h"
#include "uart.h"

void hardwareTest(void)
{
	static INT8U txdata = 0x55,txCnt;
    static INT16U dacData = 0;
    INT16U temp;
    float theta;
    static INT8U DMA_EN_cnt = 0;
    
	//MOTO_P_DIS;
        Delay_us(50);
	
	gCar.direction = 1;
	/*	
	//if(gCar.direction)

		{
		   MOTOR_INDIRECT(1);
		    Delay_us(5);
		   MOTOR_INDIRECT(0);
		}
	//else
		{
	    	MOTOR_INDIRECT(1);;
    
   		 MOTOR_INDIRECT(0);;
		}
    */
    Delay_us(50);
    //MOTO_N_EN;
    
    
    if(MOTOR_HALL_STATUS_A)
    {
        LED_ON;
    }
    else
    {
        LED_OFF;
    }
    //LED_OFF;
    //LED_FLASH;
    
    CAR_LOADING_NO;
    Delay_us(5000);
    CAR_LOADING_YES;
    
    //UART1_RX_EN;
    
    //if(!GOODS_READY)
    {
      //UART1_TX_EN; USART_SendData(USART1,0xaa);
	  //UART1_TX_EN; USART_SendData(USART2,0xaa);
	  //UART1_TX_EN; USART_SendData(USART3,0xaa);


	  
	  //setMotorDirect(MOTOR_P);
	  //setMotorSpeed(200);

	  
    }
    
    if(dacData < 360)
    {
      dacData += 1;
    }
    else
    {
      dacData = 0;
    }
	//SPI_I2S_SendData16(SPI1,dacData);
    //Delay(50);
    //theta = (float)dacData/57.3;
    //temp = 2000 * (1 + sin(theta));
    //DAC_SetChannel1Data(DAC_Align_12b_R,(INT16U)temp);
    //SET_MOTO(0);

    txCnt++;
	if(txCnt == 20)
	{
      
      //UartSend(UART1,&txdata,1);
      //UartSend(UART2,"hellow word",12);
      //USART_SendData(USART2,0xaa);
	  
	  //USART_SendData(USART3,0xaa);
	  //if(DMA_EN_cnt < 10)
	  {
	      DMA_EN_cnt++;
		  UART1_TX_EN;
		  
		  Delay_ms(1);
          //USART1_DMA_Tx(10);
		  //LumMod_Uart_DAM_Tx_Over();
	  }
	  //USART_SendData(USART1,0x0);
      
      SET_MOTO(3000,MOTOR_INDIRECT_P);
	  
      //SET_MOTO(500,MOTOR_INDIRECT_STOP);

	  //SET_MOTO(500,MOTOR_INDIRECT_N);
      //GPIO_ResetBits(GPIOB, GPIO_Pin_8); GPIO_SetBits(GPIOB, GPIO_Pin_9);
      
      //SET_REFV(1000 * 3);  //3.3
      //DAC_SetChannel2Data(DAC1,DAC_Align_12b_R,1000);
	      txCnt = 0;
	}

	//gLsSignal.signalA = ReadOnChipAdc(ONCHIP_ADC1,SA);
	//gLsSignal.signalB = ReadOnChipAdc(ONCHIP_ADC1,SB);
	//gLsSignal.signalC = ReadOnChipAdc(ONCHIP_ADC1,SC);
	//gLsSignal.signalD = ReadOnChipAdc(ONCHIP_ADC1,SD);
}





