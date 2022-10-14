// Drive.c Module
// By: Ben Allen, Swen Augustine
// Jaunuary 29, 2022
// Functions for stepper initialization and setting

#include "Drive.h"
#include "utility.h"

void Drive_Init(void){
		//Enable clock for GPIO B
		RCC -> AHBENR |= RCC_AHBENR_GPIOCEN; 
	
		//PC10 to PC11 (10)
		GPIO_MODER_SET( C, 10, 2UL );
		GPIO_MODER_SET( C, 11, 2UL );
		
		// Setting PC10, PC11 AF as TIM8_CH2N
		GPIOC -> AFR[1]  |=  (0x04UL << (4 * 2));
		GPIOC -> AFR[1]  |=  (0x04UL << (4 * 3));
	
	
		//PC10, PC11 Push-pull output
		GPIO_OTYPER_SET( C, 10, 0UL );	
		GPIO_OTYPER_SET( C, 11, 0UL );	
	
		//PC10, PC11 No Pull-up/Pull-down
		GPIO_PUPDR_SET( C, 10, 0UL );
		GPIO_PUPDR_SET( C, 11, 0UL );
	
		//Enable TIM8
		RCC -> APB2ENR |= RCC_APB2ENR_TIM8EN;
	
		//2. Program prescaler to ensure TIM8 counts at 1us
		// (TIMER_FREQ = 1000000)
		TIM8 -> PSC = (SystemCoreClock / TIMER_FREQ) - 1;
	
		// 4. Set ARR to 1000 us period
		// ARR = 1000 - 1 us						(COUNT_PER = 0.001)
		TIM8 -> ARR = (unsigned int)(COUNT_PER) - 1;	

		
		//Enable TIM8 ARR Preload
		//0 is not buffered
		//1 is buffered     
		TIM8 -> CR1 |= TIM_CR1_ARPE;
		
		
		//Enable TIM8 main output (MOE flag on BDTR)
		//0 OC outputs are disable
		//1 OC outputs are enabled
		TIM8 -> BDTR |= TIM_BDTR_MOE;
		
		
		//Select PWM mode 1 (low-count mode) on TIM8 CH2 (OC2M flags - 2bits - on CCMR1)

		// These 4 bits are for OC2M
		TIM8 -> CCMR1 &= ~(0x00001000U);
		TIM8 -> CCMR1 |= (0x00002000U);
		TIM8 -> CCMR1 |= (0x00004000U);
		TIM8 -> CCMR1 &= ~(0x01000000U);

		//Enable Output Compare Preload
		TIM8 -> CCMR1 |= (0x01UL << TIM_CCMR1_OC2PE_Pos);


		// These 4 bits are for OC1M
		
		TIM8 -> CCMR1 &= ~(0x00000010U);
		TIM8 -> CCMR1 |= (0x00000020U);
		TIM8 -> CCMR1 |= (0x00000040U);
		TIM8 -> CCMR1 &= ~(0x00010000U);
		
		// Enabling OC1PE in CCMR1
		TIM8 -> CCMR1 |= (0x01UL << TIM_CCMR1_OC1PE_Pos);
		
		
		//Enable output channel 2 with no negate
		//I think write 1 here (maybe 0)
		TIM8 -> CCER |= (0x01UL << TIM_CCER_CC1NE_Pos);
		
		//Make CH2 Active high by writing 0
		TIM8 -> CCER &= ~(0x01UL << TIM_CCER_CC1NP_Pos);
		
		// Set CH2 PWM initial on-tme to 0
		TIM8 -> CCR1 = 0;
		
		//Enable output channel 2 with no negate
		//I think write 1 here (maybe 0)
		TIM8 -> CCER |= (0x01UL << TIM_CCER_CC2NE_Pos);
		
		//Make CH2 Active high by writing 0
		TIM8 -> CCER &= ~(0x01UL << TIM_CCER_CC2NP_Pos);
		
		// Set CH2 PWM initial on-tme to 0
		TIM8 -> CCR2 = 0;
		
		//Set TIM8 off
		//Force and update Event
		TIM8 -> EGR |= (0x01UL << TIM_EGR_UG_Pos);
		
		//Enable TIM8 Counting
		TIM8 -> CR1 |= (0x01UL << TIM_CR1_CEN_Pos);
		
		//PC8, PC9, PC12, PC13 
		GPIO_MODER_SET( C, 8, 1UL );
		GPIO_MODER_SET( C, 9, 1UL );
		GPIO_MODER_SET( C, 12, 1UL );
		GPIO_MODER_SET( C, 13, 1UL );
		
		//PC8, PC9, PC12, PC13  Push-pull output
		GPIO_OTYPER_SET( C, 8, 0UL );	
		GPIO_OTYPER_SET( C, 9, 0UL );	
		GPIO_OTYPER_SET( C, 12, 0UL );	
		GPIO_OTYPER_SET( C, 13, 0UL );	
		
		//PC8, PC9, PC12, PC13 No Pull-up/Pull-down
		GPIO_PUPDR_SET( C, 8, 0UL );
		GPIO_PUPDR_SET( C, 9, 0UL );
		GPIO_PUPDR_SET( C, 12, 0UL );
		GPIO_PUPDR_SET( C, 13, 0UL );
		
		TIM8-> CCR1 = 250;

		TIM8-> CCR2 = 750;
		
}

void DriveSetting(int LFWD, int LRWD, int LSTOP, int RFWD, int RRWD, int RSTOP){
	
	if (RFWD == 1) 			GPIOC -> ODR |= (0x01UL << (1*8));	 	// TURN ON PC8
											GPIOC -> ODR &= ~(0x01UL << (1*9));	 	// TURN OFF PC9
	
	if (RRWD == 1) 			GPIOC -> ODR |= (0x01UL << (1*9));	 	// TURN ON PC9   
											GPIOC -> ODR &= ~(0x01UL << (1*8));	 	// TURN OFF PC8
	
	if (RSTOP == 1) 		GPIOC -> ODR &= ~ (0x01UL << (1*9));	 	// TURN OFF PC9   
											GPIOC -> ODR &= ~(0x01UL << (1*8));	 	// TURN OFF PC8
	
	if (LFWD == 1) 			GPIOC -> ODR |= (0x01UL << (1*12));	 	// TURN ON PC12
											GPIOC -> ODR &= ~(0x01UL << (1*13));	 	// TURN OFF PC13
	
	if (LRWD == 1) 			GPIOC -> ODR |= (0x01UL << (1*13));	 	// TURN ON PC13   
											GPIOC -> ODR &= ~(0x01UL << (1*12));	 	// TURN OFF PC12
	
	if (LSTOP == 1) 		GPIOC -> ODR &= ~ (0x01UL << (1*12));	 	// TURN ONF PC12  
											GPIOC -> ODR &= ~(0x01UL << (1*13));	 	// TURN OFF PC13
	
}





uint16_t Drive( int16_t angle ){
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
	TIM8 -> CCR2 = newPW;
	
	result = newPW;
	// Return calculated PW for printout in main
	return(result); //return in microseconds
}
