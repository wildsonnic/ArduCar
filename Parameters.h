#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_
#include <Arduino.h>

//define right motor driver outputs:
#define	IN1	      PIND2 //to Arduino pin 2
#define IN2	      PIND4 //to Arduino pin 4
#define rightPWM  PIND3//to Arduino pin 3

//define left motor driver outputs:
#define	IN3	    PIND5 //to Arduino pin 5
#define IN4	    PIND7 //to Arduino pin 7
#define leftPWM PIND6 //to Arduino pin 6



//Commands used to move the motors:
enum Command{
	forward,
	backward,
	left,
	right,
	stop
};


//some of the wheels may run faster than others, this is used to balance.
uint8_t wheelOffset = 7;

//TEMP VARS:
int SPEED = 0;
int DIR   = 0;




//Ultrasonic sensor variables:
uint16_t leftDistance   = 0;
uint16_t rightDistance  = 0;
uint16_t centerDistance = 0;

bool look       = false;  //to activate the ultrasonic sensor and check the area when running on manual.
bool AUTONOMOUS = true; // to change between autonomous and manual mode.

#endif
