// LED Source File

#include "LEDHeader1.h"
#include "utility.h"

void LED_Init(void){
	
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

	GPIOA -> MODER &= ~(3UL << (2*5));
	GPIOA -> MODER |= (1UL << (2*5));
	
	GPIOA -> OTYPER &= ~(1UL << 5);
	
	GPIOA -> ODR |= (1UL << 5);	// ON
//GPIOA -> ODR &= ~(1UL << 5);	// OFF
	
}

void LED_Toggle(void){

	
	FLIP_BITS( GPIOA -> ODR, (1UL << 5));
}

