/*
 * LMS1xx.h
 *
 *  Created on: 09-08-2010
 *  Author: Konrad Banachowicz
 */

#ifndef LMS1XX_H_
#define LMS1XX_H_

#include "LIDAR.h"
#include <string>
//#include <stdint.h>


struct ScanConfig 
{

	int scaningFrequency; // 1/100 Hz
	int angleResolution; // 1/10000 degree
	int startAngle; // 1/10000 degree
	int stopAngle; // 1/10000 degree
};

struct ScanDataConfig 
{

	//Defines which output channel is activated.
	int outputChannel;

	//Defines whether remission values are output.
	bool remission;

	//Defines whether the remission values are output with 8-bit or 16bit resolution.
	int resolution;

	//Defines which output channel is activated.
	int encoder;

	//Defines whether position values are output.
	bool position;

	//Determines whether the device name is to be output.
	bool deviceName;
	
	bool timestamp;

	//Defines which scan is output.
	//01 every scan. 02 every 2nd scan. 50000 every 50000th scan
	int outputInterval;
};


struct scanData {

	//Number of samples in dist1.
	int dist_len1;

	//Radial distance for the first reflected pulse
	uint16_t dist1[1082];

	//Number of samples in dist2.
	int dist_len2;

	//Radial distance for the second reflected pulse
	uint16_t dist2[1082];

	//Number of samples in rssi1.
	int rssi_len1;

	//Remission values for the first reflected pulse
	uint16_t rssi1[1082];

	//Number of samples in rssi2.
	int rssi_len2;

	//Remission values for the second reflected pulse
	uint16_t rssi2[1082];
};

enum Status
{
	undefined = 0,
	initialisation = 1,
	configuration = 2,
	idle = 3,
	rotated = 4,
	in_preparation = 5,
	ready = 6,
	ready_for_measurement = 7
};

class LMS1xx 
{
public:
	bool Connected;
	bool Debug;

	int SockDescription;
	LIDAR *Lidar;

	LMS1xx(LIDAR *lidar);
	~LMS1xx();

	void Connect(std::string host, int port = 2111);
	void Disconnect();

	//After receiving this command LMS1xx unit starts spinning laser and measuring.
	void Start();
	
	//After receiving this command LMS1xx unit stop spinning laser and measuring.
	void Stop();

	Status GetStatus();

	//Increase privilege level, giving ability to change device configuration.
	void Login();

	
	ScanConfig GetScanConfig() const;

	void SetScanConfig(const ScanConfig &config);

	//Set format of scan message returned by device.
	void SetScanDataConfig(const ScanDataConfig &config);

	//Start or stop continuous data acquisition.
	void scanContinous(bool start);

	//Receive single scan message.
	void getData(scanData& data);

	//Save Parameters in the EEPROM of the LMS and will also be available after the device is switched off and on again.
	void saveConfig();

	void startDevice();
};


#endif /* LMS1XX_H_ */

