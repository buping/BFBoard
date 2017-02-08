/************************************
FileName:AppSwitch.c//²¦Âë¿ª¹Ø
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/

#include "APP_Switch.h"

// Function   name   :  scanSwitch
// Description       : 
// Return     type   : INT16U
// Argument         : void
INT16U scanSwitch(void)
{
    INT16U switchStatus = 0;
    
    if(SWITCH0_CHEK)
    {
       SETBITONE(switchStatus,BIT0);
    }
    else
    {
       SETBITZERO(switchStatus,BIT0);
    }
    
    if(SWITCH1_CHEK)
    {
       SETBITONE(switchStatus,BIT1);
    }
    else
    {
       SETBITZERO(switchStatus,BIT1);
    }
    
    if(SWITCH2_CHEK)
    {
       SETBITONE(switchStatus,BIT2);
    }
    else
    {
       SETBITZERO(switchStatus,BIT2);
    }
    
    if(SWITCH3_CHEK)
    {
       SETBITONE(switchStatus,BIT3);
    }
    else
    {
       SETBITZERO(switchStatus,BIT3);
    }
    
    if(SWITCH4_CHEK)
    {
       SETBITONE(switchStatus,BIT4);
    }
    else
    {
       SETBITZERO(switchStatus,BIT4);
    }
    
    if(SWITCH5_CHEK)
    {
       SETBITONE(switchStatus,BIT5);
    }
    else
    {
       SETBITZERO(switchStatus,BIT5);
    }  

    if(SWITCH6_CHEK)
    {
       SETBITONE(switchStatus,BIT6);
    }
    else
    {
       SETBITZERO(switchStatus,BIT6);
    }
    
    if(SWITCH7_CHEK)
    {
       SETBITONE(switchStatus,BIT7);
    }
    else
    {
       SETBITZERO(switchStatus,BIT7);
    }
    
    if(SWITCH8_CHEK)
    {
       SETBITONE(switchStatus,BIT8);
    }
    else
    {
       SETBITZERO(switchStatus,BIT8);
    }

    if(SWITCH9_CHEK)
    {
       SETBITONE(switchStatus,BIT9);
    }
    else
    {
       SETBITZERO(switchStatus,BIT9);
    }
    
    switchStatus = ~switchStatus;
    switchStatus &= 0x3ff;
    
    return(switchStatus);
}