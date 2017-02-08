/************************************
FileName:flash.h//flash programe
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/

#ifndef _FLASH_H
 #define _FLASH_H

#include "Common.h"

/* Private define ------------------------------------------------------------*/
#define FLASH_FIRST_EN (0xdcba)
#define UNIT_LEN (2)

#define FLASH_FAILED (0)
#define PASSED (!FLASH_FAILED)
#define FLASH_PAGE_SIZE         ((uint32_t)0x00000800)   /* FLASH Page Size */
#define FLASH_USER_START_ADDR   ((uint32_t)0x08007000)   /* Start @ of user Flash area */
#ifdef STM32F303xE
#define FLASH_USER_END_ADDR     ((uint32_t)0x08080000)   /* End @ of user Flash area */
#else
#define FLASH_USER_END_ADDR     ((uint32_t)0x08007800)   /* End @ of user Flash area */
#endif
#define DATA_32                 ((uint32_t)0x12345678)



void writeFlash(INT32U startAddr,INT16U *data);
void readFlash(INT16U *data);

#endif