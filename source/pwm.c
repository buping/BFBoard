/************************************
FileName:pwm.c//pwm驱动函数
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/


#include "pwm.h"

#include "stm32f30x.h"
void initPwm(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
//unsigned int TimerPeriod = 0;
unsigned int Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;


/* TIM1 Configuration ---------------------------------------------------
  Generate 7 PWM signals with 4 different duty cycles:
  TIM1 input clock (TIM1CLK) is set to 2 * APB2 clock (PCLK2), since APB2 
   prescaler is different from 1.	
   TIM1CLK = 2 * PCLK2	
   PCLK2 = HCLK / 2 
   => TIM1CLK = 2 * (HCLK / 2) = HCLK = SystemCoreClock
  TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
  SystemCoreClock is set to 168 MHz for STM32F4xx devices
  
  The objective is to generate 7 PWM signal at 17.57 KHz:
	- TIM1_Period = (SystemCoreClock / 17570) - 1
  The channel 1 and channel 1N duty cycle is set to 50%
  The channel 2 and channel 2N duty cycle is set to 37.5%
  The channel 3 and channel 3N duty cycle is set to 25%
  The channel 4 duty cycle is set to 12.5%
  The Timer pulse is calculated as follows:
	- ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
  
  Note: 
   SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
   Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
   function to update SystemCoreClock variable value. Otherwise, any configuration
   based on this variable will be incorrect. 
 ----------------------------------------------------------------------- */
 /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
// TimerPeriod = (SystemCoreClock / 5000 ) - 1;

 /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
 //Channel1Pulse = (unsigned int) (((unsigned long) 5 * (TimerPeriod - 1)) / 10);
 /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
// Channel2Pulse = (unsigned int) (((unsigned long) 375 * (TimerPeriod - 1)) / 1000);
 /* Compute CCR3 value to generate a duty cycle at 25%	for channel 3 and 3N */
 //Channel3Pulse = (unsigned int) (((unsigned long) 25 * (TimerPeriod - 1)) / 100);
 /* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
 //Channel4Pulse = (unsigned int) (((unsigned long) 125 * (TimerPeriod- 1)) / 1000);

 /* TIM1 clock enable */
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
 
 /* Time Base configuration */
 TIM_TimeBaseStructure.TIM_Prescaler = 0;
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
 TIM_TimeBaseStructure.TIM_ClockDivision = 0;
 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

 /* Channel 1, 2,3 and 4 Configuration in PWM mode */
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
 TIM_OCInitStructure.TIM_Pulse =Pulse_25;
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
 TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
 TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
 TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
 //TIM_OC1Init(TIM4, &TIM_OCInitStructure);

 //TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
 TIM_OC2Init(TIM3, &TIM_OCInitStructure);

 //TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
 //TIM_OC3Init(TIM4, &TIM_OCInitStructure);

 //TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
// TIM_OC4Init(TIM4, &TIM_OCInitStructure);
   //TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器
 
  //TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能

 /* TIM1 counter enable */
 TIM_Cmd(TIM3, ENABLE);

   /* PWM I/O */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_2);
  //GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_2);

  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

  /* PWM pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

 	/* TIM1 Main Output Enable */
 	TIM_CtrlPWMOutputs(TIM3, ENABLE);	  
}