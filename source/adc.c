/************************************
FileName:adc.c//adcÇý¶¯º¯Êý
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/

#include "adc.h"

void initAdc(void)
{
	ADC_InitTypeDef 	ADC_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* ADCCLK = PCLK2/4 */
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div8); 

	/* GPIOB Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	  
	/* Configure ADC Channel9 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ADC1 Periph clock enable */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);

	/* ADCs DeInit */  
	ADC_DeInit(ADC1);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* Configure the ADC1 in continuous mode */
	//ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2;
	//ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//ADC_InitStructure.ADC_NbrOfChannel = 0;
	//ADC_Init(ADC1, &ADC_InitStructure);

	/* Convert the ADC1 Channel 9 with 55.5 Cycles as sampling time */ 
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 0, ADC_SampleTime_239Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);	

	/* ADC1 reset calibration register */	
	//ADC_ResetCalibration(ADC1);

	//while(ADC_GetResetCalibrationStatus(ADC1));

	/* ADC1 calibration start */
	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1)); 


	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); 
    
    //ADC_SoftwareStartConv(ADC1);
}
