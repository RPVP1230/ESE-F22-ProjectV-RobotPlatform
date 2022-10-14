// Servo.c Implementation file
// By: Ben Allen, Swen Augustine
// February 5, 2022
// Instructions to configure and control the servo motor

#include "servo.h"
#include "utility.h"

void servo_init(){
		//Enable clock for GPIO B
		RCC -> AHBENR |= RCC_AHBENR_GPIOBEN; 
	
		//PB15 to AF1 (10)
		GPIO_MODER_SET( B, 15, 2UL );
		
		// Setting PB15 AF as TIM15_CH2N
		GPIOB -> AFR[1]  |=  (0x01UL << (4 * 7));
	
		//PB15 Push-pull output
		GPIO_OTYPER_SET( B, 15, 0UL );	
	
		//PB15 No Pull-up/Pull-down
		GPIO_PUPDR_SET( B, 15, 0UL );
	
		//Enable TIM15
		RCC -> APB2ENR |= RCC_APB2ENR_TIM15EN;
	
		//2. Program prescaler to ensure TIM15 counts at 1us
		// (TIMER_FREQ = 1000000)
		TIM15 -> PSC = (SystemCoreClock / TIMER_FREQ) - 1;
	
		// 4. Set ARR to 20000 us period
		// ARR = 1 - 20000 us						(COUNT_PER = 0.02)
		TIM15 -> ARR = (unsigned int)(COUNT_PER) - 1;	

		
		//Enable TIM15 ARR Preload
		//0 is not buffered
		//1 is buffered     
		TIM15 -> CR1 |= TIM_CR1_ARPE;
		
		//Enable TIM15 main output (MOE flag on BDTR)
		//0 OC outputs are disable
		//1 OC outputs are enabled
		TIM15 -> BDTR |= TIM_BDTR_MOE;
		
		
		//Select PWM mode 1 (low-count mode) on TIM15 CH2 (OC2M flags - 2bits - on CCMR1)

		TIM15 -> CCMR1 &= ~(0x00001000U);
		TIM15 -> CCMR1 |= (0x00002000U);
		TIM15 -> CCMR1 |= (0x00004000U);
		TIM15 -> CCMR1 &= ~(0x01000000U);
		
		
		
		//Enable Output Compare Preload
		TIM15 -> CCMR1 |= (0x01UL << TIM_CCMR1_OC2PE_Pos);
		
		//Enable output channel 2 with no negate
		//I think write 1 here (maybe 0)
		TIM15 -> CCER |= (0x01UL << TIM_CCER_CC2E_Pos);
		
		//Make CH2 Active high by writing 0
		TIM15 -> CCER &= ~(0x01UL << TIM_CCER_CC2P_Pos);
		
		// Set CH2 PWM initial on-tme to 0
		TIM15 -> CCR2 = 0;
		
		//Set TIM15 off
		//Force and update Event
		TIM15 -> EGR |= (0x01UL << TIM_EGR_UG_Pos);
		
		//Enable TIM15 Counting
		TIM15 -> CR1 |= (0x01UL << TIM_CR1_CEN_Pos);
		
		
		
}

uint16_t RCServo_SetAngle( int16_t angle ){
	uint16_t newPW;
	uint16_t result;
	// Set pulse width according to angle
	// Neutral Pos is 1500us			
	// 45 degrees is +- 450us			
	// 1 degrees is +- 10us			
	newPW = 1500 + (angle * 10);
	
	// Check if PW exceeded +45 degrees or -45 degrees
	if (newPW > 1950) newPW = 1950;
	else if (newPW < 1050) newPW = 1050;
	
	// Write new PW into TIM15 CR2
	TIM15 -> CCR2 = newPW;
	
	result = newPW;
	// Return calculated PW for printout in main
	return(result); //return in microseconds
}

