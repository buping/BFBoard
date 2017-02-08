#ifndef _HAL_MCU_H
#define _HAL_MCU_H

#include "Common.h"
#include "Misc.h"
#include "SysConfig.h"

#define  LSI_CLK (42000) 

#define UART_EN         1
#define PWM_EN          0
#define AD7328_EN       0
#define ADG_EN          0
#define ADG1414_EN      0
#define LS_CODE_EN      0

#define TEST_URAT 0

//mak code define
#define BIT0    0x0001
#define BIT1    0x0002
#define BIT2    0x0004
#define BIT3    0x0008
#define BIT4    0x0010
#define BIT5    0x0020
#define BIT6    0x0040
#define BIT7    0x0080
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000

#define  SYS_CLK (64)
//Define I/O process
#define SETBIT(X,MASK)		(X |= MASK)		
#define CLRBIT(X,MASK)      (X &= ~MASK)
#define CHKBIT(X,MASK)		(X & MASK) 
//Define memory process
#define SETBITONE(X,MASK)		(X |= MASK)
#define SETBITZERO(X,MASK)     (X &= ~MASK)
#define CHECKVALUE(X,MASK)	(X & MASK)

//*********************WatchDog************************//
#define		CLRWD	{IWDG_ReloadCounter();}
#define  ENTER_CRITICAL  { __ASM volatile ("cpsid i"); }
#define  EXIT_CRITICAL   { __ASM volatile ("cpsie i"); }

#define EALLOW  ;
#define EDIS    ;

//*********************UART***************************//
#if(UART_EN)

  #define UART1_TX_EN    GPIO_SetBits(GPIOB, GPIO_Pin_14); 
  #define UART1_RX_EN    GPIO_ResetBits(GPIOB, GPIO_Pin_14); 
  #define UART_TX_BUFF1 (INT16U*)&USART1->TDR
  #define UART_TX_FULL1	(INT16U*)&USART1->ISR
  #define UART_RX_BUFF1	(INT16U*)&USART1->RDR
  #define UART_RX_EMPT1	(INT16U*)&USART1->ISR

  #define UART_TX_BUFF2 (INT16U*)&USART2->TDR
  #define UART_TX_FULL2	(INT16U*)&USART2->ISR
  #define UART_RX_BUFF2	(INT16U*)&USART2->RDR
  #define UART_RX_EMPT2	(INT16U*)&USART3->ISR

  #define UART3_TX_EN    GPIO_SetBits(GPIOB, GPIO_Pin_13);
  #define UART_TX_BUFF3 (INT16U*)&USART3->TDR
  #define UART_TX_FULL3	(INT16U*)&USART3->ISR
  #define UART_RX_BUFF3	(INT16U*)&USART3->RDR
  #define UART_RX_EMPT3	(INT16U*)&USART3->ISR

#else
  #define UART_TX_BUFF1 &hardWireReg
  #define UART_TX_FULL1	&hardWireReg
  #define UART_RX_BUFF1 &hardWireReg
  #define UART_RX_EMPT1 &hardWireReg

  #define UART_TX_BUFF2 &hardWireReg
  #define UART_TX_FULL2	&hardWireReg
  #define UART_RX_BUFF2 &hardWireReg
  #define UART_RX_EMPT2 &hardWireReg

  #define UART_TX_BUFF3 &hardWireReg
  #define UART_TX_FULL3	&hardWireReg
  #define UART_RX_BUFF3 &hardWireReg
  #define UART_RX_EMPT3 &hardWireReg
#endif

//#define MOTOR_HALL_STATUS GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
#define MOTOR_HALL_STATUS_A GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)
#define MOTOR_HALL_STATUS_B GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

#define CAR_LOADING_NO  GPIO_SetBits(GPIOB, GPIO_Pin_12); 
#define CAR_LOADING_YES GPIO_ResetBits(GPIOB, GPIO_Pin_12);



#define GOODS_READY GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

 
  #define MOTOR_INDIRECT
  #define CAR_LED_NO  GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
  #define CAR_LED_YES  GPIO_SetBits(GPIOB, GPIO_Pin_12);

  #define   MOTOR_INDIRECT_P (0)
  #define   MOTOR_INDIRECT_N (1)
  #define   MOTOR_INDIRECT_STOP (2)

  #define MOTOR_INDIRECT(indirect){\
  	if(indirect == MOTOR_INDIRECT_P)\
  	{\
  		GPIO_ResetBits(GPIOB, GPIO_Pin_8);GPIO_SetBits(GPIOB, GPIO_Pin_9);\
  	}\
    else if(indirect == MOTOR_INDIRECT_N)\
    {\
    	GPIO_SetBits(GPIOB, GPIO_Pin_8); GPIO_ResetBits(GPIOB, GPIO_Pin_9);\
    }\
    else\
    {\
    	GPIO_SetBits(GPIOB, GPIO_Pin_8); GPIO_SetBits(GPIOB, GPIO_Pin_9);\
    }\
  }

#define LED_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_2); 
#define LED_OFF  GPIO_SetBits(GPIOB, GPIO_Pin_2); 
#define GREEN_LED_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_7); 
#define GREEN_LED_OFF  GPIO_SetBits(GPIOB, GPIO_Pin_7); 

#define LED_FLASH {LED_ON;Delay(10000);LED_OFF;}

//#define SET_MOTO(speed,indirect) {(DAC_SetChannel1Data(DAC1,DAC_Align_12b_R,(INT16U)(65535 - (float)speed * 4095/65535))); MOTOR_INDIRECT(indirect);}
#define SET_MOTO(speed,indirect) {TIM_SetCompare2(TIM3,speed); MOTOR_INDIRECT(indirect);}

//#define SET_REFV(voltage) DAC_SetChannel1Data(DAC2,DAC_Align_12b_R,(INT16U)voltage);
#define SET_REFV(voltage) DAC_SetChannel2Data(DAC1,DAC_Align_12b_R,(INT16U)voltage);

#define RUN_POWER_OFF   GPIO_ResetBits(GPIOB, GPIO_Pin_1);
#define RUN_POWER_ON   GPIO_SetBits(GPIOB, GPIO_Pin_1);

#define CAR_BREAK_NO  //GPIO_SetBits(GPIOB, GPIO_Pin_12); 
#define CAR_BREAK_YES  //GPIO_ResetBits(GPIOB, GPIO_Pin_12);

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void);

 /* @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void);
void TIM1_UP_TIM16_IRQHandler(void);

void USART1_IRQHandler (void);

void USART2_IRQHandler (void);

void USART3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
#endif

