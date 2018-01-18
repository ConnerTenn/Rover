
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
	char *buf = new char[1 + 16 + 1];
	*(char *)(buf + offset) = L_start; offset += sizeof(u8);
	ArrCpy(buf, "sMN LMCstartmeas", 16); offset += sizeof(u8) * 16;
	*(char *)(buf + offset) = L_end; offset += sizeof(u8);

	Network->Send(buf, offset);

	Network->Recv(buf, offset);

	std::string a; a = buf;
	Lidar->DebugConsole->Write(a + "\n");
}

