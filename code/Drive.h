// Drive.h Header file
// By: Ben Allen, Swen Augustine
// Jaunuary 29, 2022
// Function definitions
#ifndef __STM3F303RE_UART_H		// Header guard checks if "stm323f303xe.h" has already been called
#define __STM3F303RE_UART_H

#include "stm32f303xe.h"
#endif

#define TIMER_FREQ 1000000UL 
#define COUNT_PER 1000

#include <stdint.h>

void Drive_Init(void);			//Stepper initialization
void DriveSetting(int LFWD, int LRWD, int LSTOP, int RFWD, int RRWD, int RSTOP);
uint16_t Drive( int16_t angle );
