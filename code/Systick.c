// UART.c Module
// By: Ben Allen, Swen Augustine
// March 10, 2022
// Functions for UART2 initialization, getting a character, and printing to terminal

#include "Systick.h"
#include "UART.h"
#include "LEDHeader1.h"


void Enable_systick_Init(void);


void Sys_Tick_Init(void){

SysTick ->CTRL = 0;
	SysTick -> LOAD = 1000000 - 1;
	SysTick ->VAL = 0;
	SysTick -> CTRL |= SysTick_CTRL_TICKINT_Msk;
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn,0);
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;


}

void SysTick_Handler(void){

	LED_Toggle();
	UARTprintf("SysTick Fired\n");

}