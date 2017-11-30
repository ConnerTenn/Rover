
class NetworkManager;

#pragma once

#include <ws2tcpip.h>
#include <winsock2.h>
//#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "Ws2_32.lib")

#include "LIDAR.h"
#include <WinLibTypes.h>
#include <vector>
#include <string>

class NetworkManager
{
public:

	LIDAR *Lidar;
	SOCKET ListenSocket = INVALID_SOCKET;
	WSAEVENT Event;

	struct SocketInfo
	{
		SOCKET Socket;
		std::string Identifier;
		int IdentifierSize = 0;
		int DataSize = 0;
		int RecieveState = 0;
		char *Buf = 0;

		void AllocBuf(int size);
		void FreeBuf();
	} SocketData;

	NetworkManager(LIDAR *Lidar);
	~NetworkManager();

	void Initialize(std::string netIP, std::string netPort);

	void Shutdown();
};


/*
class NetworkManager;

#pragma once

#include <ws2tcpip.h>
#include <winsock2.h>
//#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "Ws2_32.lib")

#include <vector>
#include <thread>

#define NW_CLOSE 0xAA02
#define NW_RECEIVE 0xAA03
#define NW_MESSAGE 0xAA04

struct MessageString
{
	std::string *String = 0;

	void Alloc(std::string str);
	void Free();
};

class NetworkManager
{
public:
	std::string NetIP;
	std::string NetPort;
	std::thread NetThread;
	bool Run = false;

	//SOCKET ConnectSocket = INVALID_SOCKET;

	WSAEVENT Event;
	struct SocketInfo
	{
		SOCKET Socket;
		std::string Identifier;
		int IdentifierSize = 0;
		int DataSize = 0;
		int RecieveState = 0;
		char *Buf = 0;

		void AllocBuf(int size);
		void FreeBuf();
	} SocketData;

	NetworkManager(DnD::DnDProgram *program);
	~NetworkManager();

	void Initialize();
	void Shutdown();
	//void ShutdownSocket(SOCKET socket);
	//static void Listen(NetworkManager *network, DnD::DnDProgram *program);
	static void NetHandler(NetworkManager *network, DnD::DnDProgram *program);

	void Receive(int eventIndex, std::string identifier, char *data, int size);

	void WriteMessage(std::string message);

	bool CreateSocketInformation();
	void FreeSocketInformation();

	bool Receive(SOCKET socket, char *buffer, int size);
	bool ReceiveIdentifier(SOCKET socket, std::string &identifier, int &dataSize);
	bool Receive2(SOCKET socket, char *buffer, int size);
	bool ReceiveIdentifier2(SOCKET socket, std::string &identifier, int &dataSize);

	bool Send(SOCKET socket, char *buffer, int size);
	bool Send(SOCKET socket, std::string identifier, char *buffer, int size);
	bool Send2(SOCKET socket, std::string identifier, char *buffer, int size);
	//void Send(DnD::Player *player, std::string identifier, char * buffer, int size);

};
*/