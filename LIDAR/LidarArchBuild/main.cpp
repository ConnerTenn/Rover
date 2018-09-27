
#include <iostream>
#include "LMS1xx.h"


typedef uint8_t u8;


LMS1xx Lidar;

int main()
{
	Lidar.connect("169.254.236.214", 2111);
	std::cout << "Connected:" << (Lidar.isConnected() ? "True" : "False") << "\n";
	
	return 0;
}
