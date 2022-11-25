/////////////////////////
/// LED Test Program
/////////////////////////

#include "stm32f303xe.h"
#include "LED.h"
#include "SysClock.h" // This is Zhu-provided header file to ensure we've got F303 system clock configured to 72MHz.  
											// Without this, your F303 won't work properly
#include "UART.h"
#include "Stepper.h"
#include "Servo.h"
#include "utility.h"
#include "Keypad.h"

void PrintMainMenu(void);

int main(void)
{	
	int8_t stepType = STEPPER_STEP_STOP;
	int16_t servoAngle = 0;	
	char inputCommand = '\0';
	char keyPressLoopup( int16_t keyPressed );
	
	// These two steps are F303 specific clock setup procedure as provided by Zhu.  
	// Use them as is.
	System_Clock_Init();
	SystemCoreClockUpdate();
	
	
	LED_init();
	UART2_Init();
	Stepper_Init();
	RCServo_Init();
	Keypad_Init();
	
	RCServo_SetAngle(servoAngle);

	LED_OFF();
	PrintMainMenu();
	
	
	for(;;)
	{
		inputCommand = UARTgetcNB();
		
		//added this to allow keypad and UART inputs to coexist, with UART input prioritizing over Keypad
		if( inputCommand == '\0' )
		{
			inputCommand = keyPressLoopup(getKey());
			
			if( inputCommand != '\0' )
			{
				UARTprintf( "[Keypad Input]: %c\n", inputCommand );
			}
			
		}			
		
		
		//Program Loop
		switch( inputCommand )
		{
			case '0':
				stepType = STEPPER_STEP_STOP;
			UARTprintf("[Stepper CMD]: Stop\n");
				break;
			
			case '1':
				stepType = STEPPER_STEP_CW_FULL;
				UARTprintf("[Stepper CMD]: CW F\n");
				break;
			
			case '2':
				stepType = STEPPER_STEP_CCW_FULL;
				UARTprintf("[Stepper CMD]: CCW F\n");
				break;
			
			case '3': 
				stepType = STEPPER_STEP_CW_HALF;
				UARTprintf("[Stepper CMD]: CW H\n");
				break;
			
			case '4':
				stepType = STEPPER_STEP_CCW_HALF;
				UARTprintf("[Stepper CMD]: CCW H\n");
				break;
			
			case '8':
				servoAngle += SERVO_ANGLE_STEP;
				if(servoAngle > SERVO_ANGLE_MAX)
					servoAngle = SERVO_ANGLE_MAX;
				
				UARTprintf("[Servo CMD]: Angle Set at: %d degrees, PWM Pulse Width: %d\n", servoAngle, RCServo_SetAngle(servoAngle));							
				
				break;
			
			case '9':
				servoAngle -= SERVO_ANGLE_STEP;
				if(servoAngle < SERVO_ANGLE_MIN)
					servoAngle = SERVO_ANGLE_MIN;
				
				UARTprintf("[Servo CMD]: Angle Set at: %d degrees, PWM Pulse Width: %d\n", servoAngle, RCServo_SetAngle(servoAngle));							
				
				break;
				
			case 'D':
				PrintMainMenu();
				break;
							
			default:
				break;
		}
		
		Stepper_Step(stepType);
		
		Delay_ms(50);
		Toggle_LED();
		
	}
}


void PrintMainMenu(void)
{
	UARTprintf( "Scott's Stepper and Servo Demo\n" );
	UARTprintf( "====================\n" );
	UARTprintf( "0) Stop\n" );
	UARTprintf( "1) Clockwise, Full Step\n" );
	UARTprintf( "2) Counter-Clockwise, Full Step\n" );
	UARTprintf( "3) Clockwise, Half Step\n" );
	UARTprintf( "4) Counter-Clockwise, Half-Step\n" );
	UARTprintf( "8) Raise Servo by 5 Degrees\n" );
	UARTprintf( "9) Lower Servo by 5 Degrees\n" );	
	UARTprintf( "CMD> " );
}	

