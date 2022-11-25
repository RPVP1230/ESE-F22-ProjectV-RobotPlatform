// LCD Module Source File

#include "lcd.h"
#include "utility.h"
#include <stdarg.h>
#include <stdio.h>

void LCD_GPIO_Init(void)
{
	ENABLE_GPIO_CLOCK( A );
	
	// PA6, 7, 8, 9, 10, and 11 on OUT, NO-PULL, and PUSH-PULL
	GPIO_MODER_SET( LCD_GPIO_PORT, 6, GPIO_MODE_OUT );
	GPIO_PUPDR_SET( LCD_GPIO_PORT, 6, GPIO_PUPD_NO );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 6, GPIO_OTYPE_PP );
	
	GPIO_MODER_SET( LCD_GPIO_PORT, 7, GPIO_MODE_OUT );
	GPIO_PUPDR_SET( LCD_GPIO_PORT, 7, GPIO_PUPD_NO );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 7, GPIO_OTYPE_PP );
	
	GPIO_MODER_SET( LCD_GPIO_PORT, 8, GPIO_MODE_OUT );
	GPIO_PUPDR_SET( LCD_GPIO_PORT, 8, GPIO_PUPD_NO );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 8, GPIO_OTYPE_PP );
	
	GPIO_MODER_SET( LCD_GPIO_PORT, 9, GPIO_MODE_OUT );
	GPIO_PUPDR_SET( LCD_GPIO_PORT, 9, GPIO_PUPD_NO );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 9, GPIO_OTYPE_PP );
	
	GPIO_MODER_SET( LCD_GPIO_PORT, 10, GPIO_MODE_OUT );
	GPIO_PUPDR_SET( LCD_GPIO_PORT, 10, GPIO_PUPD_NO );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 10, GPIO_OTYPE_PP );
	
	GPIO_MODER_SET( LCD_GPIO_PORT, 11, GPIO_MODE_OUT );
	GPIO_PUPDR_SET( LCD_GPIO_PORT, 11, GPIO_PUPD_NO );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 11, GPIO_OTYPE_PP );
}


void LCD_Init(void)
{
	LCD_GPIO_Init();	
	
	// Get Ready for LCD comm
	CLEAR_BITS( LCD_PORT, LCD_PORT_BITS );
	LCD_E_LO;
	LCD_RS_IR;
	Delay_ms(10); //  delay by 10 ms
	
	// Syncing Sequence 1
	LCD_E_HI;
	LCD_BUS( 0x03 );
	LCD_E_LO;	
	Delay_ms(5);
	
	// Syncing Sequence 2
	LCD_E_HI;
	LCD_BUS( 0x03 );
	LCD_E_LO;	
	Delay_ms(1);
	
	// Syncing Sequence 3

LCD_BUS( 0x03 );
	LCD_E_LO;	
	
	// Syncing Sequence 4
	LCD_E_HI;
	LCD_BUS( 0x02 );
	LCD_E_LO;	
	
	LCDcmd( LCD_CMD_FUNCTION | LCD_FUNCTION_4BITBUS | LCD_FUNCTION_2LINES | LCD_FUNCTION_5X8FONT );
	LCDcmd( LCD_CMD_DISPLAY | LCD_DISPLAY_OFF );
	LCD_Clear();
	LCDcmd( LCD_CMD_ENTRY | LCD_ENTRY_MOVE_CURSOR | LCD_ENTRY_INC );
	LCDcmd( LCD_CMD_DISPLAY | LCD_DISPLAY_ON | LCD_DISPLAY_NOCURSOR | LCD_DISPLAY_NOBLINK );
	
}

void LCD_Clear(void)
{
	Delay_ms(LCD_STD_CMD_DELAY);
	LCDcmd( LCD_CMD_CLEAR );
	Delay_ms( LCD_LONG_CMD_DELAY );
}


void LCD_HomeCursor(void)
{
	LCDcmd( LCD_CMD_HOME );
	Delay_ms( LCD_LONG_CMD_DELAY );
}

void LCDcmd( uint8_t cmd )
{
	Delay_ms(LCD_STD_CMD_DELAY);
	
	LCD_E_LO;
	LCD_RS_IR;
	
	LCD_E_HI;
	LCD_BUS( HI_NYBBLE( cmd ) );
	LCD_E_LO;
	
	LCD_E_HI;
	LCD_BUS( LO_NYBBLE( cmd ) );
	LCD_E_LO;
}	

void LCDdata( uint8_t data )
{
	Delay_ms(LCD_STD_CMD_DELAY);
	
	LCD_E_LO;
	LCD_RS_DR;
	
	LCD_E_HI;
	LCD_BUS( HI_NYBBLE( data ) );
	LCD_E_LO;
	
	LCD_E_HI;
	LCD_BUS( LO_NYBBLE( data ) );
	LCD_E_LO;
}

void LCDputc( unsigned char ch )
{
	switch(ch)
	{
		case '\n':
			LCDcmd( LCD_CMD_SETDDADDR | LCD_DDRAM_ADDR_LINE2 ); // maybe toggle?
			break;
		
		case '\r':
			LCD_HomeCursor();
			break;
	
		default:
			LCDdata(ch);
			break;
		
	}
}	


void LCDputs( char* str )
{
	while( *str != '\0' )
	{
		LCDputc(*str++);
	}
}	

void LCDprintf( char* str, ... )
{
	va_list args;
	char buffer[MAX_LCD_BUFSIZ];
	
	va_start( args, str );
	(void)vsnprintf(buffer, MAX_LCD_BUFSIZ, str, args);
	va_end( args );
	
	LCDputs(buffer);
}

