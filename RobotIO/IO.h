
#ifndef _IO_H_
#define _IO_H_

//Include in this file?
class Network
{

};

class I2C
{
	void Read(char *buff, u32 len);
	void Write(char *buff, u32 len);
};

class PWM
{
	int pin;
	//u16 Value;

	void Write();
	u16 Read();
};

class GPIO
{
	int pin;
	bool Value;
};

class Sensor
{

};

//Redundant
class Motor
{
		
};

#endif


