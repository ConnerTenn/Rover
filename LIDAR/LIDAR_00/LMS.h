
struct ScanConfig;
struct ScanDataConfig;
struct ScanData;
class LMS;

#pragma once

#include "LIDAR.h"

//const char L_start = 0x2;
//const char L_end = 0x3;
//const char L_sMN[3] = { 0x73, 0x4D, 0x4E };

struct ScanConfig
{
	int Frequency; // 1/100 Hz
	int Resolution; // 1/10000 degree
	int StartAngle; // 1/10000 degree
	int StopAngle; // 1/10000 degree
};

struct ScanDataConfig
{
	int OutputChannel; //Defines which output channel is activated.
	bool Remission; //Defines whether remission values are output.
	int Resolution; //Defines whether the remission values are output with 8-bit or 16bit resolution.
	int Encoder; //Defines which output channel is activated.
	bool Position; //Defines whether position values are output.
	bool DeviceName; //Determines whether the device name is to be output.
	bool Timestamp;
	int OutputInterval; //Defines which scan is output.  01 every scan. 02 every 2nd scan. 50000 every 50000th scan
};

struct ScanData
{
	int DistSize1; //Number of samples in dist1.
	uint16_t Dist1[1082]; //Radial distance for the first reflected pulse

	
	int DistSize2; //Number of samples in dist2.
	uint16_t Dist2[1082]; //Radial distance for the second reflected pulse

	int RssiSize1; //Number of samples in Rssi1.
	uint16_t Rssi1[1082]; //Remission values for the first reflected pulse

	
	int RssiSize2; //Number of samples in rssi2.
	uint16_t Rssi2[1082]; //Remission values for the second reflected pulse
};

enum Status
{
	undefined = 0,
	initialisation = 1,
	configuration = 2,
	idle = 3,
	rotated = 4,
	inPreparation = 5,
	ready = 6,
	readyForMeasurement = 7
};

class LMS
{
public:
	LIDAR *Lidar;
	NetworkManager *Network;
	
	LMS(LIDAR *lidar, NetworkManager *network);
	~LMS();

	void Send(char *buf, int size);
	void Recv(char *buf, int size);

	void Start(); //starts spinning laser and measuring.
	void Stop(); //stops spinning laser and measuring.

	void Login(); //Increase privilege level, giving ability to change device configuration.

	void StartDevice();
	
	Status GetStatus();

	void SetScanConfig(ScanConfig config);
	ScanConfig GetScanConfig();
	void SetScanDataConfig(ScanDataConfig config);
	//ScanDataConfig GetScanDataConfig();
	void SaveConfig();
	void ScanContinous(bool continous);
	void GetData(ScanData *data);
};

