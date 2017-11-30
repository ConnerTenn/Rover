
#include "LIDAR.h"


typedef uint8_t u8;

LIDAR *Lidar;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	Lidar = new LIDAR(hInstance, iCmdShow);
	Lidar->Run();
	delete Lidar;
}
