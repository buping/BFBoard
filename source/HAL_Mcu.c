/**
  ******************************************************************************
  * @file    Project/STM32F37x_StdPeriph_Templates/stm32f37x_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_it.h"
#include "Common.h"
#include "HAL_Mcu.h"
#include "UartService.h"
#include "APP_Core.h"
#include "APP_Uart.h"
#include "APP_Motor.h"
#include "uart.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  if(TIM2->SR & TIM_IT_Update)
  {
    TIM2->SR &= ~TIM_IT_Update;
    TIM2->ARR = 64 * 50000;
  }
  
}

void SysTick_Handler(void)
{
  unsigned long cnt = 0;
  //TimingDelay_Decrement();
  gSysFlg0.Bits.TimerIntFlg = 1;
          
  //CAR_LOADING_NO;
  
  //
  //»ñÈ¡´®¿ÚÖ¸Áî
  //
  //*
  //ENTER_CRITICAL;
  //UartReadDada();
  //EXIT_CRITICAL;
  //taskSchdule();
  //*/
  //CAR_LOADING_YES;
}


 /* @brief  This function handles TIM4 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM1_UP_TIM16_IRQHandler(void)
{ 

    static uint32_t CaptureNumber = 0;
    static uint32_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
    static uint32_t Capture = 0;
    static uint32_t TIM2Freq = 0;

    //GPIO_SetBits(GPIOB, GPIO_Pin_2);
    //GPIO_ResetBits(GPIOB, GPIO_Pin_2);
        

    
    if(TIM_GetITStatus(TIM16, TIM_IT_CC1) == SET) 
    {
       gMotor.hallPlusP++;
      /* Clear TIM2 Capture compare interrupt pending bit */
      TIM_ClearITPendingBit(TIM16, TIM_IT_CC1);
    }
}

 /* @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{ 

    static uint32_t CaptureNumber = 0;
    static uint32_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
    static uint32_t Capture = 0;
    static uint32_t TIM2Freq = 0;

    //GPIO_SetBits(GPIOB, GPIO_Pin_2);
    //GPIO_ResetBits(GPIOB, GPIO_Pin_2);
        

    
    if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET) 
    {
      /* Clear TIM2 Capture compare interrupt pending bit */
      TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
      
/*test pulse    
GPIOB->BSRR = GPIO_Pin_1;
 asm("nop");
 GPIOB->BRR = GPIO_Pin_1;
*/
      
      
      if(CaptureNumber == 0)
      {
        /* Get the Input Capture value */
        IC3ReadValue1 = TIM_GetCapture2(TIM2);
        CaptureNumber = 1;
      }
      else if(CaptureNumber == 1)
      {
        /* Get the Input Capture value */
        IC3ReadValue2 = TIM_GetCapture2(TIM2); 
        
        /* Capture computation */
        if (IC3ReadValue2 > IC3ReadValue1)
        {
          Capture = (IC3ReadValue2 - IC3ReadValue1); 
        }
        else if (IC3ReadValue2 < IC3ReadValue1)
        {
          Capture = ((0xFFFFFFFF - IC3ReadValue1) + IC3ReadValue2); 
        }
        else
        {
          Capture = 0;
        }
        /* Frequency computation */ 
              TIM2Freq = (uint32_t) (SystemCoreClock / Capture);
        CaptureNumber = 0;
      }
    }
}

void USART1_IRQHandler (void)
{
    INT8U data;
#if(UART1_EN)
    if(gUart1.UartRxLen < UARTRXBUFFLEN )
    {
    	gUart1.UartRxBuff[gUart1.UartRxLen++] = USART1->RDR;
    }
    else
    {
    	data = USART1->RDR; //¿Õ¶Á
    	USART1->ICR |= 0xffff;
    }
#else
    USART1->RDR = USART1->RDR; //¿Õ¶Á
#endif
}

void USART2_IRQHandler (void)
{
    static INT8U data;
#if(UART2_EN)
    if(gUart2.UartRxLen < UARTRXBUFFLEN )
    {
    	gUart2.UartRxBuff[gUart2.UartRxLen++] = USART2->RDR;
    }
    else
    {
    	USART2->RDR = USART2->RDR; //¿Õ¶Á 
        USART2->ICR |= 0xffff;
    }
#else
    data = USART2->RDR; //¿Õ¶Á
#endif
}

void USART3_IRQHandler(void)
{
#if(UART3_EN)
	if(gUart3.UartRxLen < UARTRXBUFFLEN )
    {
    	gUart3.UartRxBuff[gUart3.UartRxLen++] = USART3->RDR;
    }
    else
    {
    	USART3->RDR = USART3->RDR; //¿Õ¶Á
    	USART3->ICR |= 0xffff;
    }
#else
    USART3->RDR = USART3->RDR; //¿Õ¶Á
#endif
}

void DMA1_Channel4_IRQHandler(void)
{
    USART1_DMA_Tx_Over();
}

void DMA1_Channel5_IRQHandler(void)
{
	USART1_DMA_Rx_Over();
}

void DMA1_Channel7_IRQHandler(void)
{
    USART2_DMA_Tx_Over();
}

void DMA1_Channel6_IRQHandler(void)
{
	USART2_DMA_Rx_Over();
}
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line10) != RESET)
  {
 	 if(gMotor.opAlready)
	 {
		gMotor.hallPlusCnt++;
	 }
        EXTI_ClearITPendingBit(EXTI_Line10);
  }
    if(EXTI_GetITStatus(EXTI_Line11) != RESET)
  {
	  if(gMotor.opAlready)
	 {
		gMotor.hallPlusCnt++;
	 }
    EXTI_ClearITPendingBit(EXTI_Line11);
  }
}
/******************************************************************************/
/*                 STM32F37x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f37x.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/





