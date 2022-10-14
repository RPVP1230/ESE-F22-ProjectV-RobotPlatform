// UART.h Header file
// By: Ben Allen, Swen Augustine
// Jaunuary 21, 2022
// Function definitions

#ifndef __STM3F303RE_UART_H		// Header guard checks if "stm323f303xe.h" has already been called
#define __STM3F303RE_UART_H

#include "stm32f303xe.h"
#include <stdarg.h>  //for printf function
#include <stdio.h>

#define BAUD_RATE 9600
#define MAX_UART_BUFSIZ 255

void UART2_Init(void);

void UARTputc(char c);
void UARTputs(char *str);
char UARTgetc(void);
char UARTgetc_NP(void); // A secondary UARTgetc that doesn't pause the program
void UARTprintf(char *fmt, ... );


#endif
