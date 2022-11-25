// Keypad Implementation

#include "Keypad.h"

// List of the functions you need for this lab:
void clearAllRowOutputs(void);
void setAllRowOutputs(void);
void driveSelectedRowLow( uint8_t column );
int16_t readColumnInput( uint8_t column );
char keyPressLoopup( int16_t keyPressed );
uint8_t debounce( uint8_t column );


// Main Functions
////////////////////////
// 1. Keypad Initialization Sequence
//    R1->PB.0		R2->PB.1		R3->PB.2		R4->PB.3
// 	  C1->PB.4		C2->PB.5		C3->PB.6		C4->PB.7

void Keypad_Init(void)
{
	ENABLE_GPIO_CLOCK(B);
		
	// Configure keypad GPIOs
	// Row should be OUT
	GPIO_MODER_SET(B, 0, GPIO_MODE_OUT);
	GPIO_MODER_SET(B, 1, GPIO_MODE_OUT);
	GPIO_MODER_SET(B, 2, GPIO_MODE_OUT);
	GPIO_MODER_SET(B, 3, GPIO_MODE_OUT);
	
	// Row GPIO OUT should be Open Drain
	GPIO_OTYPER_SET(B, 0, GPIO_OTYPE_OD);
	GPIO_OTYPER_SET(B, 1, GPIO_OTYPE_OD);
	GPIO_OTYPER_SET(B, 2, GPIO_OTYPE_OD);
	GPIO_OTYPER_SET(B, 3, GPIO_OTYPE_OD);
	
	// No need for PUPD
	GPIO_PUPDR_SET(B, 0, GPIO_PUPD_NO);
	GPIO_PUPDR_SET(B, 1, GPIO_PUPD_NO);
	GPIO_PUPDR_SET(B, 2, GPIO_PUPD_NO);
	GPIO_PUPDR_SET(B, 3, GPIO_PUPD_NO);
	
		
	
	// Column should be IN
	GPIO_MODER_SET(B, 4, GPIO_MODE_IN);
	GPIO_MODER_SET(B, 5, GPIO_MODE_IN);
	GPIO_MODER_SET(B, 6, GPIO_MODE_IN);
	GPIO_MODER_SET(B, 7, GPIO_MODE_IN);
	
	// Set to Pull-Up
	GPIO_PUPDR_SET(B, 4, GPIO_PUPD_PU);
	GPIO_PUPDR_SET(B, 5, GPIO_PUPD_PU);
	GPIO_PUPDR_SET(B, 6, GPIO_PUPD_PU);
	GPIO_PUPDR_SET(B, 7, GPIO_PUPD_PU);
	
	// No need for Output Type selection - because these pins are for input!
	
}


// BEWARE OF THE IO SETTINGS FOR EVERY PIN!!
//  > Input or Output Mode?
//  > Push-Pull or Open Drain?
//  > Pull up or no?
//

// 2. Keypad Scanning Function
//    The main challenge of this lab.  Refer to the provided flowchart to develop your own algorithm!
int8_t getKey(void)
{
	uint8_t row_index, column_index;
	char pressed = '\0';
	
	// clear row output
	clearAllRowOutputs();
	
	// first, OD ground every row and check column signal 
	//  - if 0 is read, at least one of the buttons along the column is pressed. 
	for(column_index = 0; column_index < 4; column_index++)
	{
		if( readColumnInput(column_index) == 0 )
		{
			if( debounce(column_index) )
			{
				pressed = 1;
				break;
			}
		}
	}
	
	
	// if a button press is detected, find out which one along the column it is located.
	if( pressed == 1 )
	{
		for( row_index = 0; row_index < 4; row_index++)
		{
			driveSelectedRowLow(row_index);
			Delay_ms(1);  // give it some time to change
			
			if( readColumnInput(column_index) == 0 )
			{
				clearAllRowOutputs();
				return 4 * row_index + column_index;  
			}
		}
	}
	
	// otherwise, if no keypress is detected, return -1;
	clearAllRowOutputs();
	return -1;
	
}


// Helper Functions
////////////////////////

// 1. Clearing the output to ALL 4 Rows (i.e. Row scaning all disconnect)
void clearAllRowOutputs(void)
{
	CLEAR_BITS( GPIOB -> ODR, 1UL << 0); //in OD, 1 is on (GND), 0 is off (disconnected)
	CLEAR_BITS( GPIOB -> ODR, 1UL << 1);
	CLEAR_BITS( GPIOB -> ODR, 1UL << 2);
	CLEAR_BITS( GPIOB -> ODR, 1UL << 3);
}

// 2. Setting the output to ALL 4 Rows to HI (i.e. Row scanning reset)
void setAllRowOutputs(void)
{
	SET_BITS( GPIOB -> ODR, 1UL << 0); //in OD, 1 is on (GND), 0 is off (disconnected)
	SET_BITS( GPIOB -> ODR, 1UL << 1);
	SET_BITS( GPIOB -> ODR, 1UL << 2);
	SET_BITS( GPIOB -> ODR, 1UL << 3);
}

// 3. Drive selected row LO (i.e. set all rows to HI except the target row to LO)
void driveSelectedRowLow( uint8_t column )
{
	setAllRowOutputs();
	
	switch(column)
	{
		case 0:
			CLEAR_BITS( GPIOB -> ODR, 1UL << 0); 
			break;
		
		case 1:
			CLEAR_BITS( GPIOB -> ODR, 1UL << 1); 
			break;
		
		case 2:
			CLEAR_BITS( GPIOB -> ODR, 1UL << 2);
			break;
		
		case 3:
			CLEAR_BITS( GPIOB -> ODR, 1UL << 3);
			break;
		
		default:
			break;
	}
}

// 4. Read the scaning result from the selected column pin
int16_t readColumnInput( uint8_t column )
{
	switch(column)
	{
		case 0:
			return GPIOB -> IDR & (1UL << 4);  // will return 0 if pressed detected - pulled low by OD
					
		case 1:
			return GPIOB -> IDR & (1UL << 5);
					
		case 2:
			return GPIOB -> IDR & (1UL << 6);
					
		case 3:
			return GPIOB -> IDR & (1UL << 7);
					
		default:
			return -1;			
	}
}

// 5. Translate the scanned key to the corresponding ASCII character
//    (May also need a lookup table)
char keyPressLoopup(int16_t keyPressed)
{
	if( keyPressed > 15 || keyPressed < 0 )
	{
		return '\0';  // returning NULL to be consistent with UART, so to let the two controls coexist
	}
	else
	{
		return keyPressMap[keyPressed];
	}
}

// 6. Button Debounce (additional software debounce)
uint8_t debounce(uint8_t column)
{
//    5.1 Deploy a counter
			uint8_t counter = 0;
	
//    5.2 Continuously check the input status for 10ms with 1ms check interval (use Precision Delay from Lab 2)
			for(int i = 0; i < DEBOUNCE_COUNT; i++)
			{

//    5.3 For every button pressed read, increment counter by 1
				if( readColumnInput(column) == 1 && counter > 0 )  //readColumnInput should be called everytime, because IDR is volatile
				{
					counter--;
				}
				if( readColumnInput(column) == 0 )
				{
					counter++;
				}
				Delay_ms(1);
			}
	

//    5.4 After 10 consecutive checks:
// 			- if counter exceeds certain threshold number (your choice), confirm the button press.
//			- otherwise, confirm with NO button press.
			if( counter >= DEBOUNCE_THRESHOLD )
			{
				return 1;
			}
			else
			{
				return 0;
			}
			
}

