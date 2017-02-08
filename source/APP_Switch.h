/************************************
FileName:APP_Switch.h//²¦Âë¿ª¹Ø
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/

#include "Common.h"

//
//switch check
//
#define SWITCH0_CHEK  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15)
#define SWITCH1_CHEK  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14)
#define SWITCH2_CHEK  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define SWITCH3_CHEK  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)
#define SWITCH4_CHEK  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)
#define SWITCH5_CHEK  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)
#define SWITCH6_CHEK  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)
#define SWITCH7_CHEK  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)
#define SWITCH8_CHEK  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
#define SWITCH9_CHEK  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)

// Function   name   :  scanSwitch
// Description       : 
// Return     type   : INT16U
// Argument         : void
INT16U scanSwitch(void);