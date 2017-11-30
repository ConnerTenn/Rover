

/*

Motor Control
In1 | In2 || M1 | M2
-----|-----||----|----
L  |  L  || L  | L
H  |  L  || H  | L
L  |  H  || L  | H
H  |  H  || H  | H

Port L is the output lines.

Drive Motor:
Port : Pin : Connection
L0   : 49  : In1A
L1   : 48  : In2A
L4   : 45  : ENA

Catapult Launch:
Port : Pin : Connection
L6   : 43  : In1B
L7   : 42  : In2B
L5   : 44  : ENB

*/

//using port registers
#define In1A 0b00000001
#define In2A 0b00000010
#define ENA  6

#define In1B 0b01000000
#define In2B 0b10000000
#define ENB  0b00010000

#define Toggle 0b00001000

//alters port L to contain the bits of value.
//ony modifies the bits specified by the mask
void SetPort(char mask, char value)
{
	//char buf = PORTL;
	PORTL = (PORTL & ~mask) | (mask & value);
}

//allows for granular control of the speed using pwm
//speed range: 0 --> 255 --> 510
//setting full to true dissables granular speed control and sets the motor at max speed
void MotorSpeed(unsigned short speed, bool full = false)
{
	//set motor off
	SetPort(In1A | In2A, In1A | In2A);

	if (!full)
	{
		//use pwm to control speed
		analogWrite(ENA, speed > 255 ? speed - 255 : 255 - speed);
	}
	else
	{
		//set the pin to constantly high; max speed
		digitalWrite(ENA, 1);
	}

	//set direction of rotation based on sepeed parameter
	if (speed > 255)
	{
		SetPort(In1A | In2A, In1A); //forward
	}
	else if (speed < 255)
	{
		SetPort(In1A | In2A, In2A); //backward
	}
	else
	{
		SetPort(In1A | In2A, 0); //motor controler dissabled
	}
}

void Release()
{
	SetPort(ENB | In1B | In2B, ENB | In1B); //set the release motor to turn
	delay(400);
	//SetPort(ENB | In1B | In2B, ENB | In2B);
	//delay(500);
	SetPort(ENB | In1B | In2B, 0); //set the release motor to stop turning
}

void setup()
{
	//set correct pins to output
	DDRL = 0xFF;
	DDRA = ~Toggle; //toggle pin is set to input via bitwise not
	pinMode(13, OUTPUT);

	//Serial.begin(9600);
}

//Code to complete the hurdles portion of the competition
void Driving()
{
	MotorSpeed(510, true);
	delay(5400);
	MotorSpeed(0, true);
	delay(5400);
	MotorSpeed(255, true);
}

//Code to complete the archery portion of the competition
void Shooting()
{
	MotorSpeed(510, true);
	delay(3000);
	MotorSpeed(255, true);
	delay(1000);
	Release();
	//delay(1000);
	//MotorSpeed(0, true);
	//delay(2000);
	//MotorSpeed(255, true);
}

void loop()
{

	//Check the state of the Toggle pin.
	//If the pin is pulled to VCC, the  do Driving, else do Shooting.
	if ((Toggle & PINA) == Toggle)
	{
		Driving();
	}
	else
	{
		Shooting();
	}

	//end of code. Restict from looping back and re-running the code
	while (1) {}
}



