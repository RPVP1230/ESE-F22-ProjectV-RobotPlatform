// Main.c
// By: Ben Allen, Swen Augustine
// Jaunuary 29, 2022
// Provide the main base implementation

#include "SysClock.h"
#include "LEDHeader1.h"
#include "UART.h"
#include "stepper.h"
#include "utility.h"
#include "servo.h"
#include "keypad.h"
#include "Systick.h"
#include "Interupt.h"
#include "LCD.h"
#include "Drive.h"


int main(void)
{
	char c, d;
	int count = 0, sw = 0, angle = 0, result, speed1 = 0, speed2 = 0;
	
	System_Clock_Init(); 			// Configures clock to 72MHz
	SystemCoreClockUpdate(); 	// Update Routine
	
	
	//Initialize LED, UART2, and Stepper Motor
	//LED_Init();
	UART2_Init();
	Stepper_Init();
  //servo_init();
	//keypad_init();
		//Sys_Tick_Init();
		//SysTick_Handler();
	trigger();
	EXTI15_10_IRQHandler();
	//LCDinit();
	Drive_Init();

	
	LCDprintf("I DID IT MOM! ARE YOU PROUD???");
	
	//User instruction screen
	UARTprintf("========Motor Demo============\n");
	UARTprintf("[0] Stop\n");
	UARTprintf("[1] Full Step CW\n");
	UARTprintf("[2] Full Step CCW\n");
	UARTprintf("[3] Half Step CW\n");
	UARTprintf("[4] Half Step CCW\n");
	UARTprintf("[5] Servo +45 degrees\n");
	UARTprintf("[6] Servo -45 degrees\n");
	UARTprintf("[7] Servo neutral\n");
	UARTprintf("[W] Forward\n");
	UARTprintf("[S] Reverse\n");
	UARTprintf("[A] Hard Left\n");
	UARTprintf("[D] Hard Right\n");
	UARTprintf("[E] Stop\n");
	UARTprintf("[V] Left + 10%\n");
	UARTprintf("[B] Right + 10%\n");
	UARTprintf("[N] Left - 10%\n");
	UARTprintf("[M] Right - 10%\n");
	UARTprintf("[E] Stop\n");
	UARTprintf("========Motor Demo============\n");
	c = UARTgetc();						//User must input something before continuing
	stepperSetting(0,0,0,0);	//Ensure in stop mode
	
	for(;;)					//infinite loop
	{
		
		if (sw != 0) c = UARTgetc_NP();	//If in stop mode, do not get a new character
		UARTprintf("%c\n", c);
		
		if (c != '\0'){	//If there is an input, check directive
			if (c == '1'){
					//Full Step CW
					count = 0;
					sw = 1;
			}
			else if (c == '2'){
					//Full Step CCW
					count = 0;
					sw = 2;
			}
			else if (c == '3'){
					//Half Step CW
					count = 0;
					sw = 3;
			}
			else if (c == '4'){
					//Half Step CCW
					count = 0;
					sw = 4;
			}
			else if (c == '5'){
					sw = 5;
			}
			else if (c == '6'){
				  sw = 6;				
			}
			else if (c == '7'){
					sw = 7;
			}
			else if (c == 'w'){
					sw = 8;
			}
			else if (c == 's'){
					sw = 9;
			}
			else if (c == 'a'){
					sw = 10;
			}
			else if (c == 'd'){
					sw = 11;
			}
			else if (c == 'e'){
					sw = 12;
			}
				else if (c == 'v'){
					sw = 13;
			}
			else if (c == 'b'){
					sw = 14;
			}
			else if (c == 'n'){
					sw = 15;
			}
			else if (c == 'm'){
					sw = 16;
			}
			else if (c == '0'){
					//stop, set all pins to off
					stepperSetting(0,0,0,0);
					sw = 0;
					//Print user instructions again
					UARTprintf("========Stepper Demo============\n");
					UARTprintf("[0] Stop\n");
					UARTprintf("[1] Full Step CW\n");
					UARTprintf("[2] Full Step CCW\n");
					UARTprintf("[3] Half Step CW\n");
					UARTprintf("[4] Half Step CCW\n");
					UARTprintf("[5] Servo +45 degrees\n");
					UARTprintf("[6] Servo -45 degrees\n");
					UARTprintf("[7] Servo neutral\n");
					UARTprintf("[W] Forward\n");
					UARTprintf("[S] Reverse\n");
					UARTprintf("[A] Hard Left\n");
					UARTprintf("[D] Hard Right\n");
					UARTprintf("[E] Stop\n");
					UARTprintf("[V] Left + 10%\n");
					UARTprintf("[B] Right + 10%\n");
					UARTprintf("[N] Left - 10%\n");
					UARTprintf("[M] Right - 10%\n");
					UARTprintf("[E] Stop\n");
					UARTprintf("========Stepper Demo============\n");
					c = UARTgetc();		//stop mode, hold until new input
			}			
			
		}
		
		//Select Stepper pins accordingly
		if (c != '0' && c != '5' && c != '6'){
			if (count == 0)				stepperSetting(1,0,0,0);
			else if (count == 1)	stepperSetting(1,0,1,0);
			else if (count == 2)	stepperSetting(0,0,1,0);
			else if (count == 3)	stepperSetting(0,1,1,0);
			else if (count == 4)	stepperSetting(0,1,0,0);
			else if (count == 5)	stepperSetting(0,1,0,1);
			else if (count == 6)	stepperSetting(0,0,0,1);
			else if (count == 7)	stepperSetting(1,0,0,1);
		}
		
		//FS forwards
		if (sw == 1) count +=2;
		//FS backwards 
		else if (sw == 2) count -=2;
		//HS forwards
		else if (sw == 3) count++;
		//HS backwards
		else if (sw == 4) count--;
		count &= 0x07;	// set count to copy the mask of 0x07, ensuring that it stays within 0 to 7 range
		
		
		//Note: Having the SetAngle functions inside of the if loop
		//			will make sure that the servo is only being adjusted
		//			when the user wants to be adjusting it
		
		if (sw == 5){
			//Increase angle
			if(angle <= 45) angle++;
			result = RCServo_SetAngle(angle);
			UARTprintf("angle: %d\n", result);
		}
		else if (sw == 6){
			//Decrease angle
			if(angle >= -45) angle--;
			result = RCServo_SetAngle(angle);
			UARTprintf("angle: %d\n", result);
		}	
		else if (sw == 7){
			angle = 0;
			result = RCServo_SetAngle(angle);
			UARTprintf("angle: %d\n", result);
		}
		
		if (sw == 8){
			//forward
			DriveSetting(1,0,0,1,0,0);
			
		}
		if (sw == 9){
			//backward
			DriveSetting(0,1,0,0,1,0);

		}
		if (sw == 10){
			//hard left
		DriveSetting(0,1,0,1,0,0);
	
		}
		if (sw == 11){
			//hard right
			
				DriveSetting(1,0,0,0,1,0);

		}
		if (sw == 12){
			//stop
			DriveSetting(0,0,1,0,0,1);

		}
			//FS forwards
		if (sw == 13) speed1 +=1;
		//FS backwards 
		else if (sw == 14) speed2 +=1;
		//HS forwards
		else if ((sw == 15)&&(speed1 >0)) speed1 -=1;
		//HS backwards
		else if ((sw == 16)&& (speed2 >0)) speed2 -=1;
		
//LED_Toggle();
		//d = keypad_scan();
		UARTprintf("%c\n", keypad_scan());
		Wait_ms(100);		// 50ms delay
		

		
//		UARTprintf("count: %d\n", count);
		Drive(speed1);
		
	}
	
	

}




