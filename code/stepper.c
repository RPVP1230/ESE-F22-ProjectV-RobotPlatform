// Stepper.c Module
// By: Ben Allen, Swen Augustine
// Jaunuary 29, 2022
// Functions for stepper initialization and setting

#include "stepper.h"

void Stepper_Init(void){
	// Sets the clock
	RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;  
	
	// Sets PC0 to PC3 as output pins

	GPIOC -> MODER &= ~(0x01UL << (2*0));
	GPIOC -> MODER &= ~(0x01UL << (2*1));
	GPIOC -> MODER &= ~(0x01UL << (2*2));
	GPIOC -> MODER &= ~(0x01UL << (2*3));
	
	GPIOC -> MODER |= (0x01UL << (2*0));
	GPIOC -> MODER |= (0x01UL << (2*1));
	GPIOC -> MODER |= (0x01UL << (2*2));
	GPIOC -> MODER |= (0x01UL << (2*3));
	
	
	GPIOC -> OTYPER &= ~(0x00UL << (1*0)); // Set PC0 to push-pull output
	GPIOC -> OTYPER &= ~(0x01UL << (1*1)); // Set PC1 to push-pull output
	GPIOC -> OTYPER &= ~(0x02UL << (1*2)); // Set PC2 to push-pull output
	GPIOC -> OTYPER &= ~(0x03UL << (1*3)); // Set PC3 to push-pull output

}


void stepperSetting(int a, int b, int c, int d){
	
	if (a == 1) 			GPIOC -> ODR |= (0x01UL << (1*0));	 	// TURN ON PC0
	else if (a == 0) 	GPIOC -> ODR &= ~(0x01UL << (1*0));	 	// TURN OFF PC0
	
	if (b == 1) 			GPIOC -> ODR |= (0x01UL << (1*1));	 	// TURN ON PC1   
	else if (b == 0) 	GPIOC -> ODR &= ~(0x01UL << (1*1));	 	// TURN OFF PC1
	
	if (c == 1) 			GPIOC -> ODR |= (0x01UL << (1*2));	 	// TURN ON PC2
	else if (c == 0) 	GPIOC -> ODR &= ~(0x01UL << (1*2));	 	// TURN OFF PC2
	
	if (d == 1) 			GPIOC -> ODR |= (0x01UL << (1*3));	 	// TURN ON PC3
	else if (d == 0) 	GPIOC -> ODR &= ~(0x01UL << (1*3));	 	// TURN OFF PC3
	
}


void Wait_ms(uint32_t msec) {
    SysTick->CTRL = 0;                        
    // Disable Timer + SysTick interrupt + default AHB/8 clock + clear CountFlag (how?)
    SysTick->LOAD = ((SystemCoreClock /8) / 1000UL) * msec;  // Set reload register (like BRR)
    SysTick->VAL = 0;                 // Reset the SysTick counter value
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;  // Start SysTick. Use AHBclock/8
    while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));  // Wait for timeout
    SysTick->CTRL = 0;                        // Disable SysTick until next time
 }
