


#ifndef __STM3F303RE_UART_H		// Header guard checks if "stm323f303xe.h" has already been called
#define __STM3F303RE_UART_H

#include "stm32f303xe.h"
#endif

#define IDRMask0  (1UL << 0)	
#define IDRMask1  (1UL << 1)	
#define IDRMask2  (1UL << 2)	
#define IDRMask3  (1UL << 3)

#define IDRMask4  (1UL << 4)
#define IDRMask5  (1UL << 5)
#define IDRMask6  (1UL << 6)
#define IDRMask7  (1UL << 7)



void keypad_init(void);

char keypad_scan(void);

int debounce(uint8_t col);



