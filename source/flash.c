/************************************
FileName:flash.c//flash programe
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/

#include "flash.h"

void writeFlash(INT32U startAddr,INT16U *data)
{
  INT32U EraseCounter = 0x00, Address = 0x00;
  INT16U Data = 0x3210ABCD;
  INT32U NbrOfPage = 0x00;
  __IO FLASH_Status FLASHStatus = FLASH_COMPLETE;
  BOOL MemoryProgramStatus = PASSED;
  
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f30x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f30x.c file
     */ 

  /* Unlock the Flash to enable the flash control register access *************/ 
  FLASH_Unlock();
    
  /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 

  /* Define the number of page to be erased */
  NbrOfPage = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;

  /* Erase the FLASH pages */
  for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  {
    if (FLASH_ErasePage(FLASH_USER_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter))!= FLASH_COMPLETE)
    {
     /* Error occurred while sector erase. 
         User can add here some code to deal with this error  */
      while (1)
      {
#if(WATCHDOG_EN)
            CLRWD;
#endif
      }
    }
  }
  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  Address = FLASH_USER_START_ADDR;

  while (Address < FLASH_USER_END_ADDR)
  {
    //Data = (*data);
    if (FLASH_ProgramHalfWord(Address,*data) == FLASH_COMPLETE)
    {
      Address = Address + UNIT_LEN;
      data = data + 1;
    }
    else
    { 
      /* Error occurred while writing data in Flash memory. 
         User can add here some code to deal with this error */
      while (1)
      {
#if(WATCHDOG_EN)
            CLRWD;
#endif
      }
    }
  }
  //fashData =  (unsigned int *)&FLASH_USER_START_ADDR;//*FLASH_USER_START_ADDR;
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  FLASH_Lock(); 


  /* Check if the programmed data is OK 
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  Address = FLASH_USER_START_ADDR;
  MemoryProgramStatus = PASSED;
  
  while (Address < FLASH_USER_END_ADDR)
  {
    Data = *(__IO uint32_t *)Address;
    data = (INT16U*)(&gFlashData.flashWrEn);
      
    if (Data != *data)
    {
      MemoryProgramStatus = FAILED;  
    }

    Address = Address + UNIT_LEN;
    data = data + UNIT_LEN;
  }
}

void readFlash(INT16U *data)
{
    INT16U *p = (INT16U*)FLASH_USER_START_ADDR;
    INT16U i;
    
    for(i = 0;i < sizeof(FASH_DATA) / 2;i++)
    {
        *(data + i) = *(p + i); 
    }
}