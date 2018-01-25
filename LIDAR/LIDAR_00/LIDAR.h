
class LIDAR;

#pragma once

#include "Globals.h"
#include "NetworkManager.h"
#include "LMS1xx.h"
#include <WinLib.h>

class LIDAR
{
public:

	Window *WindowPtr;
	Console *DebugConsole;
	NetworkManager *Network;
	LMS1xx *LMSInterface;

	LIDAR(HINSTANCE hInstance, INT iCmdShow);
	~LIDAR();

	void Run();

	static void Callback(Window *window, MessageData message, void *data);
	void GraphicCallback(MessageData message);

};
