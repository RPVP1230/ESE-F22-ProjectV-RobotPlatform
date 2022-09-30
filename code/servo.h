// Servo.h Servo header file
// By: Ben Allen, Swen Augustine
// February 5, 2022
// Function definitions

#define TIMER_FREQ 1000000UL 
#define COUNT_PER 0.02

#include <stdint.h>

void servo_init(void);
uint16_t RCServo_SetAngle( int16_t angle );



