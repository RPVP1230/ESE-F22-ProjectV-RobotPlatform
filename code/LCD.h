#ifndef __STM32F303RE_NUCLEO64_CLOCK_H
#define __STM32F303RE_NUCLEO64_CLOCK_H


//***********************************************************************************************
// Shell code for Nucleo-64 STM32F303RE @ 72 MHz
//
//	Companion header file to LCD.c
//
// WPS 30 Apr 2020 - from Zhu Virtual Com Port demo code
//
//***********************************************************************************************
#define LCS_RS_MSK 0x0040

#define LCD_E_MSK 0x0080


#define LCD_E_HI SET_BITS( GPIOA->ODR, LCD_E_MSK)
#define LCD_E_LO CLEAR_BITS( GPIOA->ODR, LCD_E_MSK)

#define LCD_RS_DR SET_BITS( GPIOA->ODR, LCS_RS_MSK)
#define LCD_RS_IR CLEAR_BITS( GPIOA->ODR, LCS_RS_MSK)

#define LOW_NYBBLE( value ) (( value) & 0x0F)
#define HI_NYBBLE( value ) (( value >> 4) & 0x0F)


#define LCD_DATA_MSK 0x0F00
#define LCD_DATA_POS 8
#define LCD_BUS( value ) FORCE_BITS( GPIOA -> ODR, LCD_DATA_MSK, ( value << LCD_DATA_POS ))



#define LCD_CMD_ENTRY 0x04
#define LCD_CMD_ENTRY_INC 0x02
#define LCD_CMD_ENTRY_SHIFT 0x01


#define LCD_CLR_DISPLAY 0x01


#define LCD_RETURN_DISPLAY 0x02
#define LCD_ENTRY_COMMAND 0x04

#define LCD_CMD_DISPLAY 0x08
#define LCD_CMD_DISPLAY_ON 0x04
#define LCD_CMD_DISPLAY_CURSOR_ON 0x02
#define LCD_CMD_DISPLAY_BLINK_ON 0x01


#define LCD_CURSOR_SET 0x10

#define LCD_FUNCTION_SET 0x20
#define LCDFUNC_5x10_FONT 0x10
#define Two_LINES 0x08
#define LCD_4Bit_Bus 0x00


#define CGRAMM 0x40
#define DDRAM 0x80
#define DDRAM1 0xC1


#endif
#include "utility.h"
#include "UART.h"

void LCD_GPIO_Init(void);

void LCDinit(void);
void LCDcmd(unsigned char cmd);
void LCD_Data(uint8_t data);
void LCDclear(void);
void HomeCursor (void);
void LCDputc(char c);
void LCDputs(char* s);
void LCDprintf(char *fmt, ...);


