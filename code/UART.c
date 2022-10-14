// UART.c Module
// By: Ben Allen, Swen Augustine
// Jaunuary 21, 2022
// Functions for UART2 initialization, getting a character, and printing to terminal

#include "UART.h"


void UART2_Init(void)
{
  //PART 1: Clock Source Configuration for UART2
	//1.1: Enable USART2 clock to enable subsystem
	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;                                               
	
	//1.2: Select clock source
	//RCC_CFGR3_USART2SW_1 - MSB
	//RCC_CFGR3_USART2SW_0 - LSB
	// |= is to turn ON bit
	// &=  ~ is to turn OFF bit
	
	RCC -> CFGR3 &= ~RCC_CFGR3_USART2SW_1; // Changes MSB to 0
	RCC -> CFGR3 |= RCC_CFGR3_USART2SW_0;  // Changes LSB to 1

  //PART 2: Enable and Configure GPIO PA2 and PA3
	// 2.1: Enable the clock
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;   

	// 2.2: Enable PA2 and PA3 on AF7
	GPIOA -> MODER &= ~(0x03UL << (2*2));  // Clear PA2 Mode Register value
	GPIOA -> MODER &= ~(0x03UL << (2*3));  // Clear PA3 Mode Register
	
	GPIOA -> MODER |= (0x02UL << (2*2));  // Set PA2 to AF   02UL  10 (binary)
	GPIOA -> MODER |= (0x02UL << (2*3));  // Set PA3 to AF   02UL  10 (binary)
	
	// 2.3: Choose AF for PA2 and PA3 (AF7) -- AF7 is 0111 or 0x07
	GPIOA -> AFR[0]  |=  (0x07UL << (4*2));    // Means AFRL register.   Set PA2 AF to 7
	GPIOA -> AFR[0]  |=  (0x07UL << (4*3));    // Means AFRL register.   Set PA3 AF to 7
	// a better way is available in stm32f202xe.h ----> Would use: GPIO_AFRL_AFRL7 ?  but what about setting PA2 and PA3?
	
	
	//For OSPEEDR: x0 is low speed.
	//OSPEED is 2 bits per register
	//PA2: << 2*2
	//PA3: << 2*3
	GPIOA	-> OSPEEDR |= (0x0UL << (2*2));
	GPIOA -> OSPEEDR |= (0x0UL << (2*3));
	
	//For PUPDR: No pull up / pull down is 00 (0x0UL)
	//PUPDR is 2 bits per register
	//PA2: << 2*2
	//PA3: << 2*3
	GPIOA -> PUPDR &= ~(0x0UL << (2*2)); // PA2 NO PULL UP / PULL DOWN
	GPIOA -> PUPDR &= ~(0x0UL << (2*3)); // PA3 NO PULL UP / PULL DOWN
	
	//For OTYPER: push/pull is 0 (0x0)
	//OTYPER is 1 bit per register
	// PA2: << 1*2
	// PA3: << 1*3
	GPIOA -> OTYPER &= ~(0x0UL << (1*2)); // Set PA2 to push-pull output
	GPIOA -> OTYPER &= ~(0x0UL << (1*3)); // Set PA3 to push-pull output
	

  //PART 3: Configuring UART Comm Mode and Enable it
	//3.1: Disable UART2
	// Can be disable by writing 0 to UE
	USART2 -> CR1 &= ~USART_CR1_UE;
	
	//3.2: Configure the BAUD Rate
	// BRR = clock speed (72MHz) / baud rate (9600)
	USART2 -> BRR |= SystemCoreClock / BAUD_RATE;
	
	
	//3.3  Configure the data length (Word length) to 8 bit
	// Write 00 into USART_CR1_M
	USART2 -> CR1 &= ~USART_CR1_M0;  
	USART2 -> CR1 &= ~USART_CR1_M1;
	// Could also have used USART2 -> CR1 &= ~USART_CR1_M;  Which wil set both bits to 0
	
	//3.4: Configure sampling rate OVER8 needs to be reset to indicate 16x oversampling rate for each USART pulse
	//over8 reset bit is 0
	USART2 -> CR1 &= ~USART_CR1_OVER8;
	
	// 3.5: Set the stop bit to 1 bit
	// STOP bits are on registers 12 and 13
	// 00 = 1 stop bit, so both bits need to be 0
	USART2 -> CR2 &= ~(12UL);  
	USART2 -> CR2 &= ~(13UL);
	
	//3.6: Enable transmitter and receiver
	//RE bit is at register 2
	//TE bit is at register 3
	USART2 -> CR1 |= USART_CR1_RE;
	USART2 -> CR1 |= USART_CR1_TE;
	
	// 3.7 Enable the USART2
	USART2 -> CR1 |= USART_CR1_UE;
	
	//3.8: Need to wait for Transmitter and Receiver Ready status before completing initialization
	while ( (USART2 -> ISR & USART_ISR_TEACK) == 0);  //wait until transmitter is ready
	while ( (USART2 -> ISR & USART_ISR_REACK) == 0);  //wait until receiver is ready
	
}

char UARTgetc(void)
{
	while (!(USART2->ISR & USART_ISR_RXNE));		//Wait until Rx is empty
	return(USART2->RDR);
	
}

char UARTgetc_NP(void)
{
	if(USART2->ISR & USART_ISR_RXNE){		//If Rx is not empty, return with the character in it
		return(USART2->RDR);							
	}	
	else {
		return('\0');											//If Rx is empty, return with null character
	}
	
}


void UARTputc(char c)
{
	while (!(USART2->ISR & USART_ISR_TXE));		//Wait until Tx is empty
	USART2->TDR = (uint8_t)c;									//USART2 becuase we're using #2
	
}

void UARTputs(char *str)
{
	// While the string is active (has not hit \0
	// go to putc function with the next character in the array
	while( *str )
		UARTputc( *str++ ); 
}

void UARTprintf(char *fmt, ... ){
	va_list myArgs; // holds info from the ...
	
	char buffer[MAX_UART_BUFSIZ];
	
	va_start(myArgs, fmt);
	
	vsnprintf( buffer, MAX_UART_BUFSIZ, fmt, myArgs );
	
	va_end( myArgs );
	
	UARTputs ( buffer );
	
}

