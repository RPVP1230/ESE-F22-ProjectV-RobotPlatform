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
#include "lcd.h"
#include "DriveMotor.h"
#include "Encoder.h"

void PrintMainMenu(void);

int main(void)
{	
	int8_t stepType = STEPPER_STEP_STOP;
	int16_t servoAngle = 0;	
	char inputCommand = '\0';
	uint16_t count = 0;
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
	LCD_Init();
	
	DriveMotor_Init();
	Encoder_Init();

	
	RCServo_SetAngle(servoAngle);

	LED_OFF();
	PrintMainMenu();
	
	//LCDputc('A');
	LCDprintf( "\r start \n", inputCommand );
	LCDprintf("\n>Robyn's Robot<");
	
	LCD_HomeCursor();
	
	for(;;)
	{
		inputCommand = UARTgetcNB();
		count++;
		
		//LCDprintf("\r0x%04X", count);
		//LCDprintf( "\r start \n", inputCommand );
		
		/*
		//added this to allow keypad and UART inputs to coexist, with UART input prioritizing over Keypad
		if( inputCommand == '\0' )
		{
			inputCommand = keyPressLoopup(getKey());
			
			if( inputCommand != '\0' )
			{
				UARTprintf( "[Keypad Input]: %c\n", inputCommand );
			}
			
		}		
		*/
		
		//Program Loop
		switch( inputCommand )
		{
			case '0':
				stepType = STEPPER_STEP_STOP; //stepper
				SetMotorSpeed(LEFT_MOTOR, 0); //left drive motor
				SetMotorDir(LEFT_MOTOR, MOTOR_STOP);
				SetMotorSpeed(RIGHT_MOTOR, 0); //right drive motor
				SetMotorDir(RIGHT_MOTOR, MOTOR_STOP);
				UARTprintf("[Stepper CMD]: All Stop\n");
				LCDprintf( "\r %c = All Stop        \n", inputCommand );
				break;
			
			case '1':
				stepType = STEPPER_STEP_CW_FULL;
				UARTprintf("[Stepper CMD]: CW F\n");
				LCDprintf( "\r %c = CW F \n", inputCommand );
				break;
			
			case '2':
				stepType = STEPPER_STEP_CCW_FULL;
				UARTprintf("[Stepper CMD]: CCW F\n");
				LCDprintf( "\r %c = CCW F \n", inputCommand );
				break;
			
			case '3': 
				stepType = STEPPER_STEP_CW_HALF;
				UARTprintf("[Stepper CMD]: CW H\n");
				LCDprintf( "\r %c = CW H \n", inputCommand );
				break;
			
			case '4':
				stepType = STEPPER_STEP_CCW_HALF;
				UARTprintf("[Stepper CMD]: CCW H\n");
				LCDprintf( "\r %c = CCW F \n", inputCommand );
				break;
			
			case '8':
				servoAngle += SERVO_ANGLE_STEP;
				if(servoAngle > SERVO_ANGLE_MAX)
					servoAngle = SERVO_ANGLE_MAX;
				
				UARTprintf("[Servo CMD]: Angle Set at: %d degrees, PWM Pulse Width: %d\n", servoAngle, RCServo_SetAngle(servoAngle));	
				LCDprintf( "\r %c = Servo + \n", inputCommand );				
				
				break;
			
			case '9':
				servoAngle -= SERVO_ANGLE_STEP;
				if(servoAngle < SERVO_ANGLE_MIN)
					servoAngle = SERVO_ANGLE_MIN;
				
				UARTprintf("[Servo CMD]: Angle Set at: %d degrees, PWM Pulse Width: %d\n", servoAngle, RCServo_SetAngle(servoAngle));
				LCDprintf( "\r %c = Servo - \n", inputCommand );
				
				break;
				
			case 'D':
				PrintMainMenu();
				LCDprintf( "\r start             \n", inputCommand );
				break;
			
			case 'y': // L FWD
				SetMotorSpeed(LEFT_MOTOR, 750);
				SetMotorDir(LEFT_MOTOR, MOTOR_FWD);
				UARTprintf("[Drive Motor CMD]: Left FW\n");
				LCDprintf( "\r %c = Lt FW     \n", inputCommand );
				break;
				
			case 'h': // L STOP
				SetMotorSpeed(LEFT_MOTOR, 0);
				SetMotorDir(LEFT_MOTOR, MOTOR_STOP);
				UARTprintf("[Drive Motor CMD]: Left Stop H\n");
				LCDprintf( "\r %c = Lt Stop \n", inputCommand );
				break;
			
			case 'n': // L RWD
				SetMotorSpeed(LEFT_MOTOR, 750);
				SetMotorDir(LEFT_MOTOR, MOTOR_RWD);
				UARTprintf("[Drive Motor CMD]: Left RW\n");
				LCDprintf( "\r %c = Lt RW    \n", inputCommand );
				break;
			
			case 'u': // R FWD
				SetMotorSpeed(RIGHT_MOTOR, 750);
				SetMotorDir(RIGHT_MOTOR, MOTOR_FWD);
				UARTprintf("[Drive Motor CMD]: Right FW\n");
				LCDprintf( "\r %c = Rt	FW    \n", inputCommand );
				break;
			
			case 'j': // R STOP
				SetMotorSpeed(RIGHT_MOTOR, 0);
				SetMotorDir(RIGHT_MOTOR, MOTOR_STOP);
				UARTprintf("[Drive Motor CMD]: Right Stop\n");
				LCDprintf( "\r %c = Rt Stop \n", inputCommand );
				break;
			
			case 'm': // R RWD
				SetMotorSpeed(RIGHT_MOTOR, 750);
				SetMotorDir(RIGHT_MOTOR, MOTOR_RWD);
				UARTprintf("[Drive Motor CMD]: Right RW\n");
				LCDprintf( "\r %c = Rt RW     \n", inputCommand );
				break;
			
			case 'w': // Forward
				SetMotorSpeed(RIGHT_MOTOR, 750);
				SetMotorDir(RIGHT_MOTOR, MOTOR_FWD);
				SetMotorSpeed(LEFT_MOTOR, 750);
				SetMotorDir(LEFT_MOTOR, MOTOR_FWD);
				UARTprintf("[Drive Motor CMD]: Forward\n");
				LCDprintf( "\r %c = Forward   \n", inputCommand );
				break;
			
			case 's': //Reverse
				SetMotorSpeed(RIGHT_MOTOR, 750);
				SetMotorDir(RIGHT_MOTOR, MOTOR_RWD);
				SetMotorSpeed(LEFT_MOTOR, 750);
				SetMotorDir(LEFT_MOTOR, MOTOR_RWD);
				UARTprintf("[Drive Motor CMD]: Reverse  \n");
				LCDprintf( "\r %c = Reverse    \n", inputCommand );
				break;
										
			default:				
				break;
		}
		
		Stepper_Step(stepType);
		
		Delay_ms(50);
		Toggle_LED();
		
		
		
	}
	UARTprintf("LEFT: %6d, RIGHT: %6d\r", leftEncoderPeriod, rightEncoderPeriod);
}


void PrintMainMenu(void)
{
	UARTprintf( "Robot Main Menu \n" );
	UARTprintf( "====================\n" );
	UARTprintf( "0) All Stop\n" );
	UARTprintf( "1) Clockwise, Full Step\n" );
	UARTprintf( "2) Counter-Clockwise, Full Step\n" );
	UARTprintf( "3) Clockwise, Half Step\n" );
	UARTprintf( "4) Counter-Clockwise, Half-Step\n" );
	UARTprintf( "====================\n" );
	UARTprintf( "8) Raise Servo by 5 Degrees\n" );
	UARTprintf( "9) Lower Servo by 5 Degrees\n" );
	UARTprintf( "====================\n" );	
	UARTprintf( "y) Drive Motor Left Forward 750\n" );
	UARTprintf( "h) Drive Motor Left Stop\n" );
	UARTprintf( "n) Drive Motor Left Reverse 250\n" );
	UARTprintf( "u) Drive Motor Right Forward 750\n" );
	UARTprintf( "j) Drive Motor Right Stop\n" );
	UARTprintf( "m) Drive Motor Right Reverse 250\n" );
	UARTprintf( "====================\n" );
	UARTprintf( "w) Drive Motors Forward (Left & Right)\n" );
	UARTprintf( "s) Drive Motors Reverse (Left & Right)\n" );
	UARTprintf( "CMD> \n" );
}	

