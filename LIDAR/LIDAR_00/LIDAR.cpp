
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

	{
		Data = new ScanData();

		DebugConsole->Write("LMS::Creating LMS\n");
		LMSInterface = new LMS(this, Network);
		//LMSInterface->Stop();
		LMSInterface->Network = Network;
		LMSInterface->Login();
		LMSInterface->Stop();
		
		DebugConsole->Write("LMS::Getting ScanConfig\n");
		ScanConfig config = LMSInterface->GetScanConfig();
		config.Resolution = 5000;
		config.Frequency = 5000;
		DebugConsole->Write("LMS::Setting ScanConfig\n");
		LMSInterface->SetScanConfig(config);

		ScanDataConfig dataConfig;
		dataConfig.DeviceName = false;
		dataConfig.Encoder = 0;
		dataConfig.OutputChannel = 3;
		dataConfig.Remission = true;
		dataConfig.Resolution = 0;
		dataConfig.Position = false;
		dataConfig.OutputInterval = 1;
		DebugConsole->Write("LMS::Setting ScanDataConfig\n");
		LMSInterface->SetScanDataConfig(dataConfig);

		DebugConsole->Write("LMS::Starting\n");
		LMSInterface->Start();
		//scanCfg c = LMSInterface->getScanCfg();

		//c.angleResolution = 5000;
		//c.scaningFrequency = 5000;

		//LMSInterface->setScanCfg(c);

		while (LMSInterface->GetStatus() != readyForMeasurement) { Sleep(1); }
		DebugConsole->Write("LMS::Ready for Measurement\n");

		LMSInterface->ScanContinous(1);

	}

	WindowPtr->Draw();
}

LIDAR::~LIDAR()
{
	delete Data;

	LMSInterface->ScanContinous(0);
	LMSInterface->Stop();

	delete LMSInterface;
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
	else if (message == WM_TIMER && message.WParam() == 1)
	{
		LMSInterface->GetData(Data);
		DebugConsole->Write("LMS::Receive Data\n");
	}
}



