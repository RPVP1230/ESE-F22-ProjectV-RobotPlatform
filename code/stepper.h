// Stepper.h Header file
// By: Ben Allen, Swen Augustine
// Jaunuary 29, 2022
// Function definitions
#ifndef __STM3F303RE_UART_H		// Header guard checks if "stm323f303xe.h" has already been called
#define __STM3F303RE_UART_H

#include "stm32f303xe.h"
#endif
void Stepper_Init(void);			//Stepper initialization
void stepperSetting(int a, int b, int c, int d);
