/************************************
FileName:dac.c//DAC驱动函数
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/

#include "dac.h"

void initDac(void)
{
  DAC_InitTypeDef    DAC_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* Configure PA.04 (DAC1_OUT1) in analog mode -------------------------*/
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //通道DAC2
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable DAC clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC1, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC2, ENABLE);
   
  /* DAC1 channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits2_0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  //DAC_Init(DAC1, DAC_Channel_1, &DAC_InitStructure);
  
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(DAC1, DAC_Channel_2, &DAC_InitStructure);
  //DAC_Init(DAC2, DAC_Channel_1, &DAC_InitStructure);
  
  /* Enable DAC1 Channel1 */
  //DAC_Cmd(DAC1, DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC1, DAC_Channel_2, ENABLE);
  //DAC_Cmd(DAC2, DAC_Channel_1, ENABLE);
}

