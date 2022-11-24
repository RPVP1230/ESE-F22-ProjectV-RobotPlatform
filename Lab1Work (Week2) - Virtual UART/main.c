/////////////////////////
/// LED Test Program
/////////////////////////

#include "stm32f303xe.h"
#include "LED.h"
#include "SysClock.h" // This is Zhu-provided header file to ensure we've got F303 system clock configured to 72MHz.  
											// Without this, your F303 won't work properly
#include "UART.h"
#include "utility.h"

int main(void)
{
	char myChar;
	
	// These two steps are F303 specific clock setup procedure as provided by Zhu.  
	// Use them as is.
	System_Clock_Init();
	SystemCoreClockUpdate();
	
	
	LED_init();
	UART2_Init();
	
	
	LED_OFF();
	
	
	for(;;)
	{
		//Program Loop
		UARTprintf( "Scott's UART Demo\n" );
		UARTprintf( "=================\n" );
		UARTprintf( "Put In Something:\n" );
		
		LED_ON();
		
		myChar = UARTgetc();
		
		LED_OFF();
		
		UARTprintf( "You've Entered: %c\n", myChar );
		UARTprintf( "=================\n" );
		UARTprintf( "=======NEXT======\n\n" );
		
	}
}
