/************************************
FileName:SysSelfCheck.c//系统自检
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/

#include "SysSelfCheck.h"
#include "APP_Switch.h"

// Function   name   :  sysSelfCheck
// Description       : 
// Return     type   : BOOL
// Argument         : void
BOOL sysSelfCheck(void)
{
    BOOL ret = FAILED;
    
    //
	//读拨码开关
	//
    gSwitch = scanSwitch();
    
    if((gSwitch >= CAR_NO_MIN) && (gSwitch <= CAR_NO_MAX))
    {
        gDeviceType = DEVICE_CAR;
    }
    else if((gSwitch >= ENTRY_NO_MIN) && (gSwitch <= ENTRY_NO_MAX))
    {
        gDeviceType = DEVICE_ENTRY;
    }
    else if((gSwitch >= EXIT_NO_MIN) && (gSwitch <= EXIT_NO_MAX))
    {
        gDeviceType = DEVICE_EXIT;
        gExit.id = gSwitch;
    }
    else if((gSwitch >= WASTER_NO_MIN) && (gSwitch <= WASTER_NO_MAX))
    {
        gDeviceType = DEVICE_WASTER;
    }
    else if((gSwitch >= TRIGGER_NO_MIN) && (gSwitch <= TRIGGER_NO_MAX))
    {
        gDeviceType = DEVICE_TRIGGER;
    }
    else if((gSwitch >= REWRITE_NO_MIN) && (gSwitch <= REWRITE_NO_MAX))
    {
        gDeviceType = DEVICE_REWRITE;
    }
    else if((gSwitch >= BRIDGE_NO_MIN) && (gSwitch <= BRIDGE_NO_MAX))
    {
        gDeviceType = DEVICE_BRIDGE;
    }
    
    ret = SUCCEED;
    
    if(!ret)
    {
        gSysFlg0.Bits.SysStart = 1;  //PC发系统状态
    }
    
    return(ret);
}