//FileName:OledDrv.h

#ifndef	_OLED_DRV_H
#define	_OLED_DRV_H

#include "Common.h"
//#include "hal.h"
//#include "stm32f10x_gpio.h"

//
//OLED厂商提供
//
#define X_WIDTH 128
#define Y_WIDTH 64

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 
//
//

#define LCD_SCL 1 
#define LCD_SDA	1
#define LCD_RST 1 
#define LCD_DC  1


//时钟,DO/CLK
#define LCD_CLK_OUT	 	//I/O输出功能
#define LCD_CLK_H	 SETBITONE(GPIOA->ODR, BIT0)	//低电平
#define LCD_CLK_L	 SETBITZERO(GPIOA->ODR, BIT0)	//高电平


//主机出从机入		  DI/MISO
#define LCD_SDA_OUT
#define LCD_SDA_L	 SETBITZERO(GPIOA->ODR, BIT1)
#define LCD_SDA_H	 SETBITONE(GPIOA->ODR, BIT1)
#if(!OLED_LONGQIU)
//复位				 RES
#define LCD_RST_OUT		
#define LCD_RST_L	 SETBITZERO(GPIOA->ODR, BIT3)
#define LCD_RST_H	 SETBITONE(GPIOA->ODR, BIT3)



//数据/命令控制		DC   
#define LCD_DC_OUT
#define LCD_DC_L	 SETBITZERO(GPIOA->ODR, BIT2)
#define LCD_DC_H	 SETBITONE(GPIOA->ODR, BIT2)
#else
//复位				 RES
#define LCD_RST_OUT		
#define LCD_RST_L	 SETBITZERO(GPIOA->ODR, BIT2)
#define LCD_RST_H	 SETBITONE(GPIOA->ODR, BIT2)

//数据/命令控制		DC   
#define LCD_DC_OUT
#define LCD_DC_L	 SETBITZERO(GPIOA->ODR, BIT3)
#define LCD_DC_H	 SETBITONE(GPIOA->ODR, BIT3)
#endif

 extern BYTE longqiu96x64[768];

 void LcdMain(void);
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(BYTE x,BYTE y,BYTE ch[]);
 void LCD_P8x16Str(BYTE x,BYTE y,BYTE ch[]);
 void LCD_P14x16Str(BYTE x,BYTE y,BYTE ch[]);
 void LCD_Print(BYTE x, BYTE y, BYTE ch[]);
 void LCD_PutPixel(BYTE x,BYTE y);
 void LCD_Rectangle(BYTE x1,BYTE y1,BYTE x2,BYTE y2,BYTE gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_BMP(BYTE x0,BYTE y0,BYTE x1,BYTE y1,BYTE bmp[]); 
 void LCD_Fill(BYTE dat);
 void LCD_WrDat(BYTE data);
 void LCD_WrCmd(BYTE cmd);
 void LCD_Set_Pos(BYTE x, BYTE y);
 void LCD_DLY_ms(INT16U ms);
 void adjust(BYTE a);
 void SetStartColumn(unsigned char d);
 void SetAddressingMode(unsigned char d);
 void SetColumnAddress(unsigned char a, unsigned char b);
 void SetPageAddress(unsigned char a, unsigned char b);
 void SetStartLine(unsigned char d);
 void SetContrastControl(unsigned char d);
 void Set_Charge_Pump(unsigned char d);
 void Set_Entire_Display(unsigned char d);
 void Set_Entire_Display(unsigned char d);
 void Set_Inverse_Display(unsigned char d);
 void Set_Multiplex_Ratio(unsigned char d);
 void Set_Display_On_Off(unsigned char d);
 void SetStartPage(unsigned char d);
 void Set_Common_Remap(unsigned char d);
 void Set_Display_Offset(unsigned char d);
 void Set_Display_Clock(unsigned char d);
 void Set_Precharge_Period(unsigned char d);
 void Set_Common_Config(unsigned char d);
 void Set_VCOMH(unsigned char d);
 void Set_NOP(void);
 void Set_Segment_Remap(unsigned char d);
#endif	



																				 