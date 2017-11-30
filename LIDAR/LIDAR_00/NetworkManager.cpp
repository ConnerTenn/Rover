
#include "NetworkManager.h"

void NetworkManager::SocketInfo::AllocBuf(int size)
{
	Buf = new char[size];
}

void NetworkManager::SocketInfo::FreeBuf()
{
	delete[] Buf;
}

NetworkManager::NetworkManager(LIDAR *lidar)
{
	Lidar = lidar;
}

NetworkManager::~NetworkManager()
{
	Shutdown();

	Lidar->DebugConsole->Write("Network successfully shutdown\n");
}

void NetworkManager::Initialize(std::string netIP, std::string netPort)
{
	Shutdown();


	//int iResult;
	WSADATA Wsdata;
	struct addrinfo *info = NULL, hints;

	//iResult = InitNetwork("192.168.0.162", "27015"); 

	if (WSAStartup(MAKEWORD(2, 2), &Wsdata) != 0)
	{
		//Program->ConsoleWin.CommandLine->Write("ERROR::WSAStartup Failed\n");
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Program->ConsoleWin.CommandLine->Write("Connecting to: " + NetIP + ":" + NetPort + "\n");
	//Program->ConsoleWin.CommandLine->Write("Starting server:  Port:" + NetPort + "\n");
	// Resolve the local address and port to be used by the server
	if (getaddrinfo(netIP.c_str(), netPort.c_str(), &hints, &info) != 0)
	{
		Lidar->DebugConsole->Write("ERROR::Failed to setup IP\n");
		WSACleanup();
		return;
	}

	SocketData.Socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (SocketData.Socket == INVALID_SOCKET)
	{
		Lidar->DebugConsole->Write("ERROR::Faild to create socket\n");
		freeaddrinfo(info);
		WSACleanup();
		return;
	}

	if (connect(SocketData.Socket, info->ai_addr, (int)info->ai_addrlen) == SOCKET_ERROR)
	{
		Lidar->DebugConsole->Write("ERROR::Faild to connect socket:" + std::to_string(WSAGetLastError()) + "\n");
		closesocket(SocketData.Socket);
		freeaddrinfo(info);
		WSACleanup();
		return;
	}

	Event = WSACreateEvent();
	if (Event == WSA_INVALID_EVENT)
	{
		Lidar->DebugConsole->Write("ERROR::WSACreateEvent failed with error:" + std::to_string(WSAGetLastError()) + "\n");
	}

	if (WSAEventSelect(SocketData.Socket, Event, FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT) == SOCKET_ERROR)
	{
		Lidar->DebugConsole->Write("ERROR::WSAEventSelect() failed with error:" + std::to_string(WSAGetLastError()) + "\n");
	}

	freeaddrinfo(info);

	Lidar->DebugConsole->Write("Network successfully initilized\n");


	//Run = true;
	//NetThread = std::thread(&NetHandler, this, Program);
	Lidar->WindowPtr->AddMessage(MessageData({ WL_UPDATESURFACE, 0, 0 }), true);
}

void NetworkManager::Shutdown()
{
	WSASetEvent(Event);

	if (SocketData.Socket) { closesocket(SocketData.Socket); }

	SocketData.Identifier = "";
	SocketData.IdentifierSize = 0;
	SocketData.DataSize = 0;
	SocketData.RecieveState = 0;
	SocketData.Buf = 0;

	WSACleanup();
}


/*

#include "NetworkManager.h"

void NetworkManager::SocketInfo::AllocBuf(int size)
{
Buf = new char[size];
}

void NetworkManager::SocketInfo::FreeBuf()
{
delete[] Buf;
}

NetworkManager::NetworkManager(LIDAR *lidar)
{
Lidar = lidar;
}

NetworkManager::~NetworkManager()
{
Shutdown();

Lidar->DebugConsole->Write("Network successfully shutdown\n");
}

void NetworkManager::Initialize(std::string netIP, std::string netPort)
{
//int iResult;
WSADATA Wsdata;
struct addrinfo *info = NULL, hints;

//iResult = InitNetwork("192.168.0.162", "27015");

//iResult = WSAStartup(MAKEWORD(2, 2), &Wsdata);
if (WSAStartup(MAKEWORD(2, 2), &Wsdata) != 0)
{
Lidar->DebugConsole->Write("ERROR::WSAStartup Failed\n");
return;
}

ZeroMemory(&hints, sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;
hints.ai_flags = AI_PASSIVE;

Lidar->DebugConsole->Write("Starting server: " + netIP + ":" + netPort + "\n");
//Program->ConsoleWin.CommandLine->Write("Starting server:  Port:" + NetPort + "\n");
// Resolve the local address and port to be used by the server
if (getaddrinfo(netIP.c_str(), netPort.c_str(), &hints, &info) != 0)
{
Lidar->DebugConsole->Write("ERROR::Failed to setup IP\n");
WSACleanup();
return;
}

ListenSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
if (ListenSocket == INVALID_SOCKET)
{
Lidar->DebugConsole->Write("ERROR::Faild to creaete listen socket\n");
freeaddrinfo(info);
WSACleanup();
return;
}

SocketInfo socketInfo;

Event = WSACreateEvent();
if (Event == WSA_INVALID_EVENT)
{
//Program->ConsoleWin.CommandLine->Write("ERROR::WSACreateEvent failed with error:" + std::to_string(WSAGetLastError()) + "\n");
Lidar->DebugConsole->Write("ERROR::WSACreateEvent failed with error:" + std::to_string(WSAGetLastError()) + "\n");
}

// Prepare SocketInfo structure for use
socketInfo.Socket = ListenSocket;

SocketData = socketInfo;

if (WSAEventSelect(ListenSocket, Event, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
{
Lidar->DebugConsole->Write("ERROR::WSAEventSelect failed with error:" + std::to_string(WSAGetLastError()) + "\n");
}

if (bind(ListenSocket, info->ai_addr, (int)info->ai_addrlen) == SOCKET_ERROR)
{
Lidar->DebugConsole->Write("ERROR::Faild to bind socket:" + std::to_string(WSAGetLastError()) + "\n");
closesocket(ListenSocket);
freeaddrinfo(info);
WSACleanup();
return;
}

freeaddrinfo(info);

if (listen(ListenSocket, 5) == SOCKET_ERROR)
{
Lidar->DebugConsole->Write("ERROR::Listen failed with error:" + std::to_string(WSAGetLastError()) + "\n");
closesocket(ListenSocket);
WSACleanup();
return;
}


Lidar->DebugConsole->Write("Network successfully initilized\n");

Lidar->WindowPtr->AddMessage(MessageData({ WL_UPDATESURFACE, 0, 0 }), true);
}

void NetworkManager::Shutdown()
{
WSASetEvent(Event);

if (SocketData.Socket) { closesocket(SocketData.Socket); }

SocketData.Identifier = "";
SocketData.IdentifierSize = 0;
SocketData.DataSize = 0;
SocketData.RecieveState = 0;
SocketData.Buf = 0;

WSACleanup();
}



*/