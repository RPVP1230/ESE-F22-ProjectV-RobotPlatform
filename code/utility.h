// utility.h Header file
// By: Ben Allen, Swen Augustine
// February 5, 2022
// Macros

#ifndef __STM3F303RE_UART_H		// Header guard checks if "stm323f303xe.h" has already been called
#define __STM3F303RE_UART_H

#include "stm32f303xe.h"

#endif



#ifndef __MYUTILITY_H
#define __MYUTILITY_H

#define SET_BITS( port, mask ) ( (port) |= (mask) )

#define CLEAR_BITS( port, mask) ( (port) &= (~(mask)) )

#define FLIP_BITS( port, mask) ( (port) ^= (mask) )

#define FORCE_BITS( port, mask, value )	( (port) = ( ( (port) & (~(mask)) ) | ( (value) & (mask) ) ) )

#define FORCE_BITS_FULL( port, mask, value, pos ) ( (port) = ( ( (port) & (~(mask)) ) | ( (value) & (mask) ) ) | ( (mask) << (pos) ) )

#define IS_BIT_SET( port, bits ) ( (port) |= (bits) )

#define IS_BIT_CLEAR( port, bits ) ( (port) &= (~(bits)) )

#define GPIOx( port ) GPIO ## port

#define GPIO_OTYPER_SET( port, pin, type ) FORCE_BITS( GPIOx(port) -> OTYPER, (1UL << ((pin) * 1) ), ( (type) << ((pin) * 1) ) ) // *1 comes from OTYPER being 1 bit register

#define GPIO_MODER_SET( port, pin, type ) FORCE_BITS( GPIOx(port) -> MODER, (3UL << ((pin) * 2) ), ( (type) << ((pin) * 2) ) ) // *2 comes from MODER being 2 bit register

#define GPIO_PUPDR_SET( port, pin, type )	FORCE_BITS( GPIOx(port) -> PUPDR, (3UL << ((pin) * 2) ), ( (type) << ((pin) * 2) ) ) // *2 comes from PUPDR being 2 bit register







void Wait_ms(uint32_t msec);	//Sleep function
#endif
