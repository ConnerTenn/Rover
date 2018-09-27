
#ifndef _IO_H_
#define _IO_H_

//Include in this file?
class Network
{
	int Read(char *buff, u32 len);
	int Write(char *buff, u32 len);
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
	//Contains different interface objects based on type of sensor
};

//Redundant? 
class Motor
{
		
};

#endif


