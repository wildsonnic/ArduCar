
//Function to move the motor:
void move(int16_t _y, int16_t _x){
	int16_t X = _x;
	int16_t Y = _y;
	int16_t leftSpeed  = 0;
	int16_t rightSpeed = 0;


	//limit the speed value for X:
	if(X > 100) 
		X = 100;
	else if(X < -100)
		X = -100;

	//limit the speed value for Y:
	if(Y > 100) 
		Y = 100;
	else if(Y < -100)
		Y = -100;


//move the robot according to the mixing of the controls X and Y,
//where X its used for turning and Y for going forward and backward.
	if(Y >= 0){
		//apply the speed as a percentage value:
		Y = abs(X);
		Y= map(X, 0, 100, 0, 255);
		X = map(Y, -100, 100, -200, 200);
		
		//mix speed and turning:
		leftSpeed =  (Y + abs(X));
		rightSpeed = (Y + abs(X) - wheelOffset);

		//limit values:
		if(leftSpeed > 255) leftSpeed = 255;
		else if(leftSpeed < 0) leftSpeed = 0;
		if(rightSpeed > 255) rightSpeed = 255;
		else if(rightSpeed < 0) rightSpeed = 0;

		if(X > 5){
			//set right motors to move backward:
			PORTD &= ~_BV(PD2);	//set pin 2 LOW.
			PORTD |=  _BV(PD4);	//set pin 4 HIGH;
			//set left motors to move forward:
			PORTD |=  _BV(PD5);	//set pin 5 HIGH.
			PORTD &= ~_BV(PD7);	//set pin 7 LOW;
		}
		else if(X < -5){
			//set right motors to move forward:
			PORTD |=  _BV(PD2);	//set pin 2 HIGH.
			PORTD &= ~_BV(PD4);	//set pin 4 LOW;
			//set left motors to move backward:
			PORTD &= ~_BV(PD5);	//set pin 5 LOW.
			PORTD |=  _BV(PD7);	//set pin 7 HIGH;
		}
		else{
			//set right motors to move forward:
			PORTD |=  _BV(PD2);	//set pin 2 HIGH.
			PORTD &= ~_BV(PD4);	//set pin 4 LOW;
			//set left motors to move forward:
			PORTD |=  _BV(PD5);	//set pin 5 HIGH.
			PORTD &= ~_BV(PD7);	//set pin 7 LOW;
		}
		analogWrite(leftPWM,  leftSpeed);
		analogWrite(rightPWM, rightSpeed);
	}
	else if(Y < 0){
		//apply the speed as a percentage value:
		Y = abs(Y); //change to positive.
		Y = map(Y, 0, 100, 0, 255);
		X = map(X, -100, 100, -200, 200);

		//mix speed and turning:
		leftSpeed =  (Y + abs(X));
		rightSpeed = (Y + abs(X) - wheelOffset);

		//limit values:
		if(leftSpeed > 255) leftSpeed = 255;
		else if(leftSpeed < 0) leftSpeed = 0;
		if(rightSpeed > 255) rightSpeed = 255;
		else if(rightSpeed < 0) rightSpeed = 0;
		
		if(X > 5){
			//set right motors to move backward:
			PORTD |=  _BV(PD2);	//set pin 2 HIGH.
			PORTD &= ~_BV(PD4);	//set pin 4 LOW;
			//set left motors to move forward:
			PORTD &= ~_BV(PD5);	//set pin 5 LOW.
			PORTD |=  _BV(PD7);	//set pin 7 HIGH;
		}
		else if(X < -5){
			//set right motors to move backward:
			PORTD &= ~_BV(PD2);	//set pin 2 LOW.
			PORTD |=  _BV(PD4);	//set pin 4 HIGH;
			//set left motors to move forward:
			PORTD |=  _BV(PD5);	//set pin 5 LOW.
			PORTD &= ~_BV(PD7);	//set pin 7 HIGH;
		}
		else{
			//set right motors to move backward:
			PORTD &= ~_BV(PD2);	//set pin 2 HIGH.
			PORTD |=  _BV(PD4);	//set pin 4 LOW;
			//set left motors to move backward:
			PORTD &= ~_BV(PD5);	//set pin 5 HIGH.
			PORTD |=  _BV(PD7);	//set pin 7 LOW;
		}
		analogWrite(leftPWM,  leftSpeed);
		analogWrite(rightPWM, rightSpeed);
	}
	else if(X == 0 && Y == 0){
		//set right motors to stop:
		PORTD &= ~_BV(PD2);	//set pin 2 LOW.
		PORTD &= ~_BV(PD4);	//set pin 4 LOW;

		//set left motors to stop:
		PORTD &= ~_BV(PD5);	//set pin 5 LOW.
		PORTD &= ~_BV(PD7);	//set pin 7 LOW;

		analogWrite(leftPWM,  0);
		analogWrite(rightPWM, 0);
	}
}



/*read input data directly form the serial port:
taken in part from: Kristian Lauszus, TKJElectronics/BalancingRobotArduino
https://github.com/TKJElectronics/BalancingRobotArduino/blob/master/BalancingRobotArduino.ino#L217-L281*/
void readData(){
	if(Serial.available() > 0){
		char input[20];
		uint8_t i = 0;
		while(true){
			input[i] = Serial.read();
			delayMicroseconds(100);
			if(input[i] == -1)
				return;
			if(input[i] == ';')
				break;
			i++;
		}
		if(input[0] == 'S'){
			strtok(input, ":");
			SPEED = atoi(strtok(NULL, ","));    //Y
			DIR   = atoi(strtok(NULL, ","));    //X
			look = atoi(strtok(NULL, ","));     
			AUTONOMOUS = atoi(strtok(NULL, ";"));

		}
	}
}


//Function used to filter the readings from the ultrasonic sensors:
uint16_t readDistance(uint8_t filter){
	static uint16_t distance;
	uint16_t newDistance = sonar.getDistance(CM);
	static uint16_t lastDistance;
	static uint16_t counter;

	if(newDistance == lastDistance){
		counter++;
	}
	else{
		counter = 0;
		lastDistance = newDistance;
	}

	if(counter > filter){
	distance = newDistance;
	counter = 0;
	}
	return distance;
}



//Function to move the servo holding the ultrasonic sensor
void sonarCheck(){
//move servo to the left and read the distance:
	for(uint8_t i=90; i< 179; i++){
		servoSonar.write(i);
		delayMicroseconds(3000);
	}
	leftDistance = sonar.getDistance(CM);
	delay(100);

//move the servo to the right and read the distance:
	for(uint8_t i=179; i> 1; i--){
		servoSonar.write(i);
		delayMicroseconds(3000);
	}
	rightDistance = sonar.getDistance(CM);
	delay(100);

//move the servo to the center and read the distance:
	for(uint8_t i=1; i< 90; i++){
		servoSonar.write(i);
		delayMicroseconds(3000);
	}
	centerDistance = sonar.getDistance(CM);
	delay(100);

}



//Function to make the robot run autonomously evading the objects.
void autonomous(){
	uint16_t distance = sonar.getDistance(CM); //get distance

//if the robot detects something near, stop and check the area:
	if(distance > 0 && distance < 50){
		move(0,0);
		delay(50);
		sonarCheck();
		//move to the left if it is the best option:
		if( (leftDistance > 50) && (leftDistance > rightDistance) && (leftDistance >  centerDistance) ){
			//move left;
			move(100, -75);
			delay(250);	//to give time to move to the left.
		}
		//move to the right if it is the best option:
		else if(rightDistance > 50 && rightDistance > leftDistance && rightDistance >  centerDistance){
			//move right;
			move(100, 75);
			delay(250);	//to give time to move to the left.
		}
		//move forward if it is the best option:
		else if( (centerDistance > 50) && (centerDistance > leftDistance) && (centerDistance >  rightDistance) ){
			//move forward;
			move(85, 0);
			delay(200);	//to give time to move to the left.
		}
		//move backward if all the readings are equal:
		else if( (centerDistance > 0 && centerDistance < 50) && 
		         (leftDistance > 0 && leftDistance < 50) && 
		         (rightDistance > 0 && rightDistance <  50) ){
			//move backward;
			move(-60, 10);
			delay(500);	//to give time to move to the left.
		}
		//move backward if there is nowhere to go ahead::
		else if( (centerDistance == rightDistance) && (centerDistance == leftDistance) ||
				 (rightDistance == leftDistance) && (rightDistance == centerDistance) ||
				 (leftDistance == centerDistance) && (leftDistance == rightDistance) ){
			//move backward;
			move(-60, 10);
			delay(500);	//to give time to move to the left.
		}
	}
	//but if you sense nothing continue forward:
	else if(distance == 0 || distance > 50){
		move(60, 0);
	}

}




