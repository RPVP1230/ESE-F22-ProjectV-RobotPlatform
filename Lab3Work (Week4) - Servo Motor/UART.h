///////////////////////
// UART Module Header
///////////////////////

#ifndef __STM3F303RE_UART_H
#define __STM3F303RE_UART_H

#include "stm32f303xe.h"

#define BAUD_RATE 9600

void UART2_Init(void);

void UARTputc(char myChar);
void UARTputs(char *myStr);
char UARTgetc(void);
char UARTgetcNB(void);

void UARTprintf( char *format, ... );
	
#endif
