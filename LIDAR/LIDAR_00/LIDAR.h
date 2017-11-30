
class LIDAR;

#pragma once

#include "NetworkManager.h"
#include <WinLib.h>

class LIDAR
{
public:

	Window *WindowPtr;
	Console *DebugConsole;
	NetworkManager *Network;

	LIDAR(HINSTANCE hInstance, INT iCmdShow);
	~LIDAR();

	void Run();

	static void Callback(Window *window, MessageData message, void *data);
	void GraphicCallback(MessageData message);

};
