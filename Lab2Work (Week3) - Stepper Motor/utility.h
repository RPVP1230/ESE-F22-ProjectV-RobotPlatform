#ifndef __SENG_UTILS_H
#define __SENG_UTILS_H

//////////////////////////////////
// General Purpose Utility Macros
//////////////////////////////////

#include "stm32f303xe.h"

// Notice that variables in the macro are resolved into values using "()"

#define SET_BITS(port, bits)		((port) |= (bits))
#define CLEAR_BITS(port, bits) 	((port) &= ~(bits))
#define FLIP_BITS(port, bits)		((port) ^= (bits))

// Use this set or clear multiple bits in a port pointed by the mask
#define FORCE_BITS(port, mask, value) 	(((port) & (~(mask))) | ((value) & (mask)))

// Logic Bit Test
#define IS_BIT_SET(port, bits)		((port) & (bits))
#define IS_BIT_CLEAR(port, bits)	((~(port)) & (bits))

// Nibble Selection
#define LO_NYBBLE(value)	((value) & 0x0F)
#define HI_NYBBLE(value)	((value >> 4) & 0x0F)

#define DELAY_CONSTANT 5000000

void Delay_Arbitrary(void);  // this is only for blocking for loop delay.
// must introduce systick delay in lab 3!

void Delay_ms (uint32_t msec); // blocking loop delay with SysTick precision timer


#endif
