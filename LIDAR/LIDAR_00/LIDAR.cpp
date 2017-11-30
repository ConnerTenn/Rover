
#include "LIDAR.h"

LIDAR::LIDAR(HINSTANCE hInstance, INT iCmdShow)
{
	WindowInfo windowInfo;

	windowInfo.Name = "LIDAR_00";
	windowInfo.Dimensions = { 1000,600 };
	windowInfo.ClearColour = Colour(0, 0, 0);
	windowInfo.HInstance = hInstance;
	windowInfo.ICmdShow = iCmdShow;
	windowInfo.Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;//WS_POPUP;//
	windowInfo.Callback = LIDAR::Callback;
	windowInfo.CallbackPtr = this;
	WindowPtr = new Window(&windowInfo);

	WindowPtr->Create();
	WindowPtr->CreateTimer(5, 0x01, -1);
	WindowPtr->AddMessage(MessageData({ WL_UPDATESURFACE, 0, 0 }), true);


	ConsoleInfo consoleInfo;

	consoleInfo.Dimensions = { 50,60 };
	consoleInfo.ParentWindow = WindowPtr;
	consoleInfo.TextFont = Font("Consolas", 8, Colour(200, 200, 255));
	DebugConsole = new Console(&consoleInfo);

	Network = new NetworkManager(this);
	Network->Initialize("169.254.13.237", "2111");


	WindowPtr->Draw();
}

LIDAR::~LIDAR()
{
	delete Network;
	delete DebugConsole;
	delete WindowPtr;
}

void LIDAR::Run()
{
	WindowPtr->AddMessage(MessageData({ WL_UPDATESURFACE, 0, 0 }), true);

	DebugConsole->Write("FLOW::Begin Main Loop\n");
	while (!WindowPtr->State.Quit)
	{
		WindowPtr->PreMsg();
		Window::GetMsg();
		WindowPtr->UpdateMsg();
	}
}

void LIDAR::Callback(Window *window, MessageData message, void *data)
{
	LIDAR *Lidar = (LIDAR *)data;

	Lidar->GraphicCallback(message);
}

void LIDAR::GraphicCallback(MessageData message)
{
	Int2 mousePos = WindowPtr->Input->MouseState.GetPos();

	if (message == WL_UPDATESURFACE)
	{
		WindowPtr->WindowSurface->Clear();
		DebugConsole->Draw(WindowPtr->WindowSurface, { 0,0 });
		WindowPtr->WindowSurface->DrawRectangle(DebugConsole->TextSurface->GetRect(), Colour(255, 255, 255));
		WindowPtr->Draw();

	}
}



