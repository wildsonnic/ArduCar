#include "Parameters.h"
#include <NewPing.h>
#include <Servo.h>

Servo servoSonar; //create a new servo object.
NewPing sonar(12, 11, 200); // NewPing setup of pins and maximum distance(trigger pin, echo pin, max_distance).

void setup(){
	Serial.begin(115200);
	DDRD = DDRD | B11111100;            //set pins 2-7 as outputs:
	PORTD = DDRD & B00000011;           //start pins 2-7  in LOW state:
	servoSonar.attach(9, 800, 2800);    //used to calibrate my servo and set center position correctly.
	servoSonar.write(90);               //start servo in center position.
	delay(1000);
}

void loop(){
	readData();	            //read incomming data through serial port:
	if(AUTONOMOUS)
		autonomous();	    //set the system autonomous( this can be changed on the go).
	else{
		move(SPEED, DIR);	//set the system manual.
		if(look){
			sonarCheck();	//the user can tell the system to chek the distances.
			look = false;
		}
	}

}
