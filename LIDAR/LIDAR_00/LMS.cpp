
#include "LMS.h"

LMS::LMS(LIDAR *lidar, NetworkManager *network)
{
	Lidar = lidar;
	Network = network;
}

LMS::~LMS()
{

}

void LMS::Start()
{
	u8 offset = 0; 
	char *buf = new char[8 + 8*23 + 8];
	*(char *)(buf + offset) = L_start; offset += sizeof(u8);
	ArrCpy(buf, "sMN LMCstartmeas", 23); offset += sizeof(u8) * 23;
	*(char *)(buf + offset) = L_end; offset += sizeof(u8);

}

