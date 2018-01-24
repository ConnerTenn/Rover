
#include "LMS.h"

LMS::LMS(LIDAR *lidar, NetworkManager *network)
{
	Lidar = lidar;
	Network = network;
}

LMS::~LMS()
{

}

void LMS::Login()
{
	std::string outstr;
	u64 offset = 0;
	char *buf = new char[1 + 29 + 1 + 1];
	//*(char *)(buf + offset) = L_start; offset += sizeof(u8);
	//ArrCpy(buf, "sMN LMCstartmeas", 16); offset += sizeof(u8) * 16;
	//*(char *)(buf + offset) = L_end; offset += sizeof(u8);
	ArrCpy(buf, (char *)&L_start, sizeof(u8), &offset);
	ArrCpy(buf, "sMN SetAccessMode 03 F4724744", sizeof(u8) * 29, &offset);
	ArrCpy(buf, (char *)&L_end, sizeof(u8), &offset);
	ArrCpy(buf, "\0", sizeof(u8), &offset); offset--;

	outstr = buf;
	Lidar->DebugConsole->Write("Sending:" + outstr + "\n");

	Network->Send(buf, offset);

	{
		offset = 0;

		ArrCpy(buf, (char *)&L_start, sizeof(u8), &offset);
		ArrCpy(buf, "sAN SetAccessMode 1", sizeof(u8) * 19, &offset);
		ArrCpy(buf, (char *)&L_end, sizeof(u8), &offset);
		ArrCpy(buf, "\0", sizeof(u8), &offset); offset--;

		outstr = buf;
		Lidar->DebugConsole->Write("Sending:" + outstr + "\n");

		Network->Send(buf, offset);
	}

	Network->Recv(buf, 100);

	outstr = buf;
	Lidar->DebugConsole->Write("Recieved:" + outstr + "\n");

	delete[] buf;
}

void LMS::Start()
{
	std::string outstr;
	u64 offset = 0; 
	char *buf = new char[1 + 16 + 1 + 1];
	//*(char *)(buf + offset) = L_start; offset += sizeof(u8);
	//ArrCpy(buf, "sMN LMCstartmeas", 16); offset += sizeof(u8) * 16;
	//*(char *)(buf + offset) = L_end; offset += sizeof(u8);
	ArrCpy(buf, (char *)&L_start, sizeof(u8), &offset);
	ArrCpy(buf, "sMN LMCstartmeas", sizeof(u8) * 16, &offset);
	ArrCpy(buf, (char *)&L_end, sizeof(u8), &offset);
	ArrCpy(buf, "\0", sizeof(u8), &offset); offset--;

	Network->Send(buf, offset);

	outstr = buf;
	Lidar->DebugConsole->Write("Sending:" + outstr + "\n");

	buf[0] = 0; buf[1] = 0;

	Network->Recv(buf, 100);

	outstr = buf;
	Lidar->DebugConsole->Write("Recieved:" + outstr + "\n");

	delete[] buf;
}

void LMS::Stop()
{
	std::string outstr;
	u64 offset = 0;
	char *buf = new char[1 + 15 + 1 + 1];
	//*(char *)(buf + offset) = L_start; offset += sizeof(u8);
	//ArrCpy(buf, "sMN LMCstartmeas", 16); offset += sizeof(u8) * 16;
	//*(char *)(buf + offset) = L_end; offset += sizeof(u8);
	ArrCpy(buf, (char *)&L_start, sizeof(u8), &offset);
	ArrCpy(buf, "sMN LMCstopmeas", sizeof(u8) * 15, &offset);
	ArrCpy(buf, (char *)&L_end, sizeof(u8), &offset);
	ArrCpy(buf, "\0", sizeof(u8), &offset); offset--;

	Network->Send(buf, offset);

	outstr = buf;
	Lidar->DebugConsole->Write("Sending:" + outstr + "\n");

	Network->Recv(buf, 100);

	outstr = buf;
	Lidar->DebugConsole->Write("Recieved:" + outstr + "\n");

	delete[] buf;
}

