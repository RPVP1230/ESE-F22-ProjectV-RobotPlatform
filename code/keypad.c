// keypad.c Header file
// By: Ben Allen, Swen Augustine
// February 10, 2022


#include "keypad.h"
#include "utility.h"
#include "UART.h"

void keypad_init(){
	
//Enable clock for GPIO B
		RCC -> AHBENR |= RCC_AHBENR_GPIOBEN; 
	
	//Enabling Outputs of R1 - R4
		GPIO_MODER_SET( B, 0, 01UL);

		GPIO_MODER_SET( B, 1, 01UL);

		GPIO_MODER_SET( B, 2, 01UL);
	
		GPIO_MODER_SET( B, 3, 01UL);
	
	
		//Enabling Inputs of C1 - C4
		GPIO_MODER_SET( B, 4, 00UL);

		GPIO_MODER_SET( B, 5, 00UL);

		GPIO_MODER_SET( B, 6, 00UL);
	
		GPIO_MODER_SET( B, 7, 00UL);
	
	//Open Drain for Rows
		GPIO_OTYPER_SET( B, 0, 01UL);

		GPIO_OTYPER_SET( B, 1, 01UL);

		GPIO_OTYPER_SET( B, 2, 01UL);

		GPIO_OTYPER_SET( B, 3, 01UL);
		
		//no pull/pull up
		
		GPIO_PUPDR_SET( B, 4, 01UL );
		
		GPIO_PUPDR_SET( B, 5, 01UL );

		GPIO_PUPDR_SET( B, 6, 01UL );

		GPIO_PUPDR_SET( B, 7, 01UL );


}

char keypad_scan(){
	
	
			int i; //For for loop
			int col_number = 0; // 
			int row_number = 0;
	  	char key_legend[4][4] = {{'1', '2', '3', 'A'},
														{'4', '5', '6', 'B'},
														{'7', '8', '9', 'C'},
														{'*', '0', '#', 'D'}};
		
														
			CLEAR_BITS( GPIOB -> ODR,(0x01UL << (1*0)) );

			CLEAR_BITS( GPIOB -> ODR,(0x01UL << (1*1)) );

			CLEAR_BITS( GPIOB -> ODR,(0x01UL << (1*2)) );
	
			CLEAR_BITS( GPIOB -> ODR,(0x01UL << (1*3)) );
	
			Wait_ms(1);

			if( (GPIOB -> IDR & IDRMask4) == 0 ){
			
					if (debounce(0) == 1) col_number = 0;	// If button reads pressed after debounce algorithm, set column number
				
			}
			else if( (GPIOB -> IDR & IDRMask5) == 0 ){
					if (debounce(1) == 1) col_number = 1;
				
			}
			else if( (GPIOB -> IDR & IDRMask6) == 0 ){
					if (debounce(2) == 1) col_number = 2;		
							
			}
			else if( (GPIOB -> IDR & IDRMask7) == 0 ){
					if (debounce(3) == 1) col_number = 3;				
										
			}
			else
			{
				col_number = -1;
				UARTprintf(".");
				return ('\0');
			}
			
			
			if(col_number >= 0)
			{
				for(i = 0; i <= 3; i++ ){
							// Set all rows to 1 except for the row that is being scanned
							SET_BITS( GPIOB -> ODR,(0x01UL << (1*0)) );

							SET_BITS( GPIOB -> ODR,(0x01UL << (1*1)) );

							SET_BITS( GPIOB -> ODR,(0x01UL << (1*2)) );
					
							SET_BITS( GPIOB -> ODR,(0x01UL << (1*3)) );
								
							CLEAR_BITS( GPIOB -> ODR,(0x01UL << (1*i)) );
					
							Wait_ms(1);
							
							if( ((GPIOB -> IDR) & (IDRMask4 << (col_number))) == 0 ){	// If column is reading pressed, we know we have found the pressed button
							
									row_number = i;
									UARTprintf("%c\n", key_legend[row_number][col_number]);
									return(key_legend[row_number][col_number]);
					
							}
				}
			}
}

int debounce(uint8_t col){
	int i;
	uint8_t counter = 0;
	
	for(i = 0; i < 10; i++){
		if( ((GPIOB -> IDR) & (IDRMask4 << (col))) == 0 ) counter++; // If button is pressed, increase counter
		else if (counter > 0) counter--;														 // Otherwise, decrease counter
		Wait_ms(1);
	}

	if (counter > 5) return(1);	// If read button presses is great than 5, return true
	else return(0);							// Otherwise, return false
	
	
}



