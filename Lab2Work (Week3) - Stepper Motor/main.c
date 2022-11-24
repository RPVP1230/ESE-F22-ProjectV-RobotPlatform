/////////////////////////
/// LED Test Program
/////////////////////////

#include "stm32f303xe.h"
#include "LED.h"
#include "SysClock.h" // This is Zhu-provided header file to ensure we've got F303 system clock configured to 72MHz.  
											// Without this, your F303 won't work properly
#include "UART.h"
#include "Stepper.h"
#include "utility.h"

int main(void)
{	
	int8_t stepType = STEPPER_STEP_STOP;
		
	// These two steps are F303 specific clock setup procedure as provided by Zhu.  
	// Use them as is.
	System_Clock_Init();
	SystemCoreClockUpdate();
	
	
	LED_init();
	UART2_Init();
	Stepper_Init();

	LED_OFF();
	
	UARTprintf( "Scott's Stepper Demo\n" );
	UARTprintf( "====================\n" );
	UARTprintf( "0) Stop\n" );
	UARTprintf( "1) Clockwise, Full Step\n" );
	UARTprintf( "2) Counter-Clockwise, Full Step\n" );
	UARTprintf( "3) Clockwise, Half Step\n" );
	UARTprintf( "4) Counter-Clockwise, Half-Step\n" );
	UARTprintf( "CMD> \n" );
	
	for(;;)
	{
		//Program Loop
		switch( UARTgetcNB() )
		{
			case '0':
				stepType = STEPPER_STEP_STOP;
				UARTprintf("CMD Stop");
				break;
			
			case '1':
				stepType = STEPPER_STEP_CW_FULL;
				UARTprintf("CMD CW F");
				break;
			
			case '2':
				stepType = STEPPER_STEP_CCW_FULL;
				UARTprintf("CMD CCW F");
				break;
			
			case '3': 
				stepType = STEPPER_STEP_CW_HALF;
				UARTprintf("CMD CW H");
				break;
			
			case '4':
				stepType = STEPPER_STEP_CCW_HALF;
				UARTprintf("CMD CCW H");
				break;
			
			default:
				break;
		}
		
		Stepper_Step(stepType);
		
		Delay_ms(50);
		Toggle_LED();
		
	}
}
