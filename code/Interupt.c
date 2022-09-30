

#include "Interupt.h"
#include "LEDHeader1.h"
#include "UART.h"
#include "SysClock.h"


void trigger(void){

	//Enable GPIO Clock
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	//Set GPIO Mode
	GPIOC->MODER &= ~(3U << (2*13));
	//GPIO Push-Pull
	GPIOC->PUPDR &= ~(3U << (2*13));
	GPIOC->PUPDR |= 1U << (2*13); //Pull Up
	
	EXTI->IMR |= EXTI_IMR_IM13; //Enable Interupt
	
	//Connect External Line to the GPI
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
	//Falling Edge trigger selection
	EXTI->FTSR |= EXTI_FTSR_FT13;
	
	//Configure NVIC for EXTI Events on Pin 10-15
	//Set its priority to 0 (Next Highest to NMIs)
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	
	//while(1);

}

void EXTI15_10_IRQHandler(void){

	if((EXTI->PR & EXTI_PR_PIF13) != 0){
	
	//Toggle LED
	LED_Toggle();
	//Cleared flag by writting 1
	EXTI->PR |= EXTI_PR_PIF13;
	
	UARTprintf("Button Pressed");
		
	}

}
