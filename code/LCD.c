// LCD.c
// By: Ben Allen, Swen Augustine
// March 24, 2022



#include "utility.h"
#include "LCD.h"
#include "UART.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdalign.h"
#include "stdarg.h"


void LCD_GPIO_Init(void){
	//Enable clock for GPIO A
		RCC -> AHBENR |= RCC_AHBENR_GPIOAEN; 
	
		GPIO_MODER_SET( A, 6, 01UL);
		GPIO_OTYPER_SET( A, 6, 00UL);
		GPIO_PUPDR_SET( A, 6, 00UL);
	
		GPIO_MODER_SET( A, 7, 01UL);
		GPIO_OTYPER_SET( A, 7, 00UL);
		GPIO_PUPDR_SET( A, 7, 00UL);


		GPIO_MODER_SET( A, 8, 01UL);
		GPIO_OTYPER_SET( A, 8, 00UL);
		GPIO_PUPDR_SET( A, 8, 00UL);
	
		GPIO_MODER_SET( A, 9, 01UL);
		GPIO_OTYPER_SET( A, 9, 00UL);
		GPIO_PUPDR_SET( A, 9, 00UL);
	
		GPIO_MODER_SET( A, 10, 01UL);
		GPIO_OTYPER_SET( A, 10, 00UL);
		GPIO_PUPDR_SET( A, 10, 00UL);
	
		GPIO_MODER_SET( A, 11, 001L);
		GPIO_OTYPER_SET( A, 11, 00UL);
		GPIO_PUPDR_SET( A, 11, 00UL);
	
}

void LCDinit(void){
	
	LCD_GPIO_Init();
	
	CLEAR_BITS( GPIOA -> ODR, (LCS_RS_MSK | LCD_E_MSK | LCD_DATA_MSK ));
	
	LCD_E_LO;
	LCD_RS_IR;
	Wait_ms(10);
	
	//Sync Step 1:
	
	LCD_E_HI;
	LCD_BUS ( 0x03 );
	LCD_E_LO;
	Wait_ms(5);
	
	LCD_E_HI;
	LCD_BUS ( 0x03 );
	LCD_E_LO;
	Wait_ms(1);
	
	LCD_E_HI;
	LCD_BUS ( 0x03 );
	LCD_E_LO;
	
	LCD_E_HI;
	LCD_BUS ( 0x02 );
	LCD_E_LO;
	Wait_ms(2);
	
	//1.Set the lcd function t a) 4 bit buss. b) two line display c) 5x8 font size
	LCDcmd(LCD_FUNCTION_SET | Two_LINES);
	
	//2. Turn off LCD temporarly
	LCDcmd(LCD_CMD_DISPLAY);
	
	//3. Clear the display
	LCDcmd(LCD_CLR_DISPLAY);
	
	//4.Set the LCD entry mode to a) Move the cursor b)Left to right rolling
	LCDcmd(LCD_CMD_ENTRY | LCD_CMD_ENTRY_INC);
	
	
	//5. Set the LCD display to a) ON b) Don't display cursor, c) No cursor blinking
	LCDcmd(LCD_CMD_DISPLAY | LCD_CMD_DISPLAY_ON);

}
void LCDcmd(unsigned char cmd ){

	Wait_ms(2);
	
	LCD_E_LO;
	LCD_RS_IR;
	
	LCD_E_HI;
	LCD_BUS (HI_NYBBLE(cmd) );
	LCD_E_LO;
	
		
	LCD_E_HI;
	LCD_BUS (LOW_NYBBLE(cmd) );
	LCD_E_LO;
	
}
void LCD_Data(uint8_t data ){
	
	Wait_ms(5);

	LCD_E_LO;
	LCD_RS_DR;
	
	LCD_E_HI;
	LCD_BUS (HI_NYBBLE(data) );
	LCD_E_LO;
	
	LCD_E_HI;
	LCD_BUS (LOW_NYBBLE(data) );
	LCD_E_LO;
	
}

void LCDclear(void){
	Wait_ms(2);
	LCDcmd(LCD_CLR_DISPLAY);
	Wait_ms(5);
}

void HomeCursor (void){

	Wait_ms(2);
	LCDcmd(LCD_RETURN_DISPLAY);
	Wait_ms(5);
}



void LCDputc(char c){
LCD_Data(c);

}
void LCDputs(char* s){
	while(*s)
		LCDputc(*s++);

}
void LCDprintf(char *fmt, ...){

	int i, j, count = 0;
	
	va_list myArgs; // holds info from the ...
	
	char buffer[32];
	char buffer1[16];
	char buffer2[16];
	
	va_start(myArgs, fmt);
	
	vsnprintf( buffer, 32, fmt, myArgs );
	
	va_end( myArgs );
	
	for (i = 0; i < 16; i++){
		if(buffer[i] == '\n'){
			break;
		}
		buffer1[i] = buffer[i];
		count = count + 1;
	}
	
	LCDputs ( buffer1 );
	LCDcmd(DDRAM1);
	
	for ( j = 0; j < 16; j++){
		buffer2[j] = buffer[j+count];
	}		
	
	LCDputs ( buffer2 );
	
	
	
}





