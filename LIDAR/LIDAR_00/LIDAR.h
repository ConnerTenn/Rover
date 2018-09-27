
class LIDAR;

#pragma once

#include "Globals.h"
#include "NetworkManager.h"
#include "LMS.h"
#include <WinLib.h>

class LIDAR
{
public:

	Window *WindowPtr;
	Console *DebugConsole;
	NetworkManager *Network;
	LMS *LMSInterface;

	ScanData *Data;

	LIDAR(HINSTANCE hInstance, INT iCmdShow);
	~LIDAR();

	void Run();

	static void Callback(Window *window, MessageData message, void *data);
	void GraphicCallback(MessageData message);

};
