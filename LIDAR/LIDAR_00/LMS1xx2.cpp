/*
 * LMS1xx.cpp
 *
 *  Created on: 09-08-2010
 *  Author: Konrad Banachowicz
 */

#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
//#include <unistd.h>

#include "LMS1xx2.h"

#include <ws2tcpip.h>
#include <winsock2.h>
//#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "Ws2_32.lib")

LMS1xx::LMS1xx(LIDAR *lidar) :
	Connected(false), Debug(false), Lidar(lidar)
{
}

LMS1xx::~LMS1xx() 
{

}

void LMS1xx::Connect(std::string host, int port) 
{
	if (!Connected) 
	{
		SockDescription = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (SockDescription) 
		{
			struct sockaddr_in stSockAddr;
			int Res;
			stSockAddr.sin_family = PF_INET;
			stSockAddr.sin_port = htons(port);
			Res = inet_pton(AF_INET, host.c_str(), &stSockAddr.sin_addr);

			int ret = connect(SockDescription, (struct sockaddr *) &stSockAddr, sizeof stSockAddr);
			if (!ret) 
			{
				Connected = true;
			}
		}
	}
}

void LMS1xx::Disconnect() 
{
	if (Connected) 
	{
		closesocket(SockDescription);
		Connected = false;
	}
}

void LMS1xx::Start() 
{
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sMN LMCstartmeas", 0x03);

	send(SockDescription, buf, strlen(buf), 0);

	int len = recv(SockDescription, buf, 100, 0);
	//	if (buf[0] != 0x02)
	//		std::cout << "invalid packet recieved" << std::endl;
	//	if (debug) {
	//		buf[len] = 0;
	//		std::cout << buf << std::endl;
	//	}
}

void LMS1xx::Stop() 
{
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sMN LMCstopmeas", 0x03);

	send(SockDescription, buf, strlen(buf), 0);

	int len = recv(SockDescription, buf, 100, 0);
	//	if (buf[0] != 0x02)
	//		std::cout << "invalid packet recieved" << std::endl;
	//	if (debug) {
	//		buf[len] = 0;
	//		std::cout << buf << std::endl;
	//	}
}

Status LMS1xx::GetStatus() 
{
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sRN STlms", 0x03);

	send(SockDescription, buf, strlen(buf), 0);

	int len = recv(SockDescription, buf, 100, 0);
	//	if (buf[0] != 0x02)
	//		std::cout << "invalid packet recieved" << std::endl;
	//	if (debug) {
	//		buf[len] = 0;
	//		std::cout << buf << std::endl;
	//	}
	int ret;
	sscanf((buf + 10), "%d", &ret);
	//ret = *(int *)(buf+10)

	return (Status) ret;
}

void LMS1xx::Login()
{
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sMN SetAccessMode 03 F4724744", 0x03);

	send(SockDescription, buf, strlen(buf), 0);

	int len = recv(SockDescription, buf, 100, 0);
	//	if (buf[0] != 0x02)
	//		std::cout << "invalid packet recieved" << std::endl;
	//	if (debug) {
	//		buf[len] = 0;
	//		std::cout << buf << std::endl;
	//	}
}

ScanConfig LMS1xx::GetScanConfig() const 
{
	ScanConfig cfg;
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sRN LMPscancfg", 0x03);

	send(SockDescription, buf, strlen(buf), 0);

	int len = recv(SockDescription, buf, 100, 0);
	//	if (buf[0] != 0x02)
	//		std::cout << "invalid packet recieved" << std::endl;
	//	if (debug) {
	//		buf[len] = 0;
	//		std::cout << buf << std::endl;
	//	}

	sscanf(buf + 1, "%*s %*s %X %*d %X %X %X", &cfg.scaningFrequency, &cfg.angleResolution, &cfg.startAngle, &cfg.stopAngle);
	return cfg;
}

void LMS1xx::SetScanConfig(const ScanConfig &cfg) 
{
	char buf[100];
	sprintf(buf, "%c%s %X +1 %X %X %X%c", 0x02, "sMN mLMPsetscancfg", cfg.scaningFrequency, cfg.angleResolution, cfg.startAngle, cfg.stopAngle, 0x03);

	send(SockDescription, buf, strlen(buf), 0);

	int len = recv(SockDescription, buf, 100, 0);

	buf[len - 1] = 0;
}

void LMS1xx::SetScanDataConfig(const scanDataCfg &cfg) 
{
	char buf[100];
	sprintf(buf, "%c%s %02X 00 %d %d 0 %02X 00 %d %d 0 %d +%d%c", 0x02,
			"sWN LMDscandatacfg", cfg.outputChannel, cfg.remission ? 1 : 0,
			cfg.resolution, cfg.encoder, cfg.position ? 1 : 0,
			cfg.deviceName ? 1 : 0, cfg.timestamp ? 1 : 0, cfg.outputInterval, 0x03);
	if (debug)
	{
		printf("%s\n", buf);
	}
	write(sockDesc, buf, strlen(buf));

	int len = read(sockDesc, buf, 100);
	buf[len - 1] = 0;
}

void LMS1xx::scanContinous(int start) {
	char buf[100];
	sprintf(buf, "%c%s %d%c", 0x02, "sEN LMDscandata", start, 0x03);

	write(sockDesc, buf, strlen(buf));

	int len = read(sockDesc, buf, 100);

	if (buf[0] != 0x02)
		printf("invalid packet recieved\n");

	if (debug) {
		buf[len] = 0;
		printf("%s\n", buf);
	}

	if (start = 0) {
		for (int i = 0; i < 10; i++)
			read(sockDesc, buf, 100);
	}
}

void LMS1xx::getData(scanData& data) {
	char buf[20000];
	fd_set rfds;
	struct timeval tv;
	int retval, len;
	len = 0;

	do {
		FD_ZERO(&rfds);
		FD_SET(sockDesc, &rfds);

		tv.tv_sec = 0;
		tv.tv_usec = 50000;
		retval = select(sockDesc + 1, &rfds, NULL, NULL, &tv);
		if (retval) {
			len += read(sockDesc, buf + len, 20000 - len);
		}
	} while ((buf[0] != 0x02) || (buf[len - 1] != 0x03));

	//	if (debug)
	//		std::cout << "scan data recieved" << std::endl;
	buf[len - 1] = 0;
	char* tok = strtok(buf, " "); //Type of command
	tok = strtok(NULL, " "); //Command
	tok = strtok(NULL, " "); //VersionNumber
	tok = strtok(NULL, " "); //DeviceNumber
	tok = strtok(NULL, " "); //Serial number
	tok = strtok(NULL, " "); //DeviceStatus
	tok = strtok(NULL, " "); //MessageCounter
	tok = strtok(NULL, " "); //ScanCounter
	tok = strtok(NULL, " "); //PowerUpDuration
	tok = strtok(NULL, " "); //TransmissionDuration
	tok = strtok(NULL, " "); //InputStatus
	tok = strtok(NULL, " "); //OutputStatus
	tok = strtok(NULL, " "); //ReservedByteA
	tok = strtok(NULL, " "); //ScanningFrequency
	tok = strtok(NULL, " "); //MeasurementFrequency
	tok = strtok(NULL, " ");
	tok = strtok(NULL, " ");
	tok = strtok(NULL, " ");
	tok = strtok(NULL, " "); //NumberEncoders
	int NumberEncoders;
	sscanf(tok, "%d", &NumberEncoders);
	for (int i = 0; i < NumberEncoders; i++) {
		tok = strtok(NULL, " "); //EncoderPosition
		tok = strtok(NULL, " "); //EncoderSpeed
	}

	tok = strtok(NULL, " "); //NumberChannels16Bit
	int NumberChannels16Bit;
	sscanf(tok, "%d", &NumberChannels16Bit);
	if (debug)
		printf("NumberChannels16Bit : %d\n", NumberChannels16Bit);
	for (int i = 0; i < NumberChannels16Bit; i++) {
		int type = -1; // 0 DIST1 1 DIST2 2 RSSI1 3 RSSI2
		char content[6];
		tok = strtok(NULL, " "); //MeasuredDataContent
		sscanf(tok, "%s", content);
		if (!strcmp(content, "DIST1")) {
			type = 0;
		} else if (!strcmp(content, "DIST2")) {
			type = 1;
		} else if (!strcmp(content, "RSSI1")) {
			type = 2;
		} else if (!strcmp(content, "RSSI2")) {
			type = 3;
		}
		tok = strtok(NULL, " "); //ScalingFactor
		tok = strtok(NULL, " "); //ScalingOffset
		tok = strtok(NULL, " "); //Starting angle
		tok = strtok(NULL, " "); //Angular step width
		tok = strtok(NULL, " "); //NumberData
		int NumberData;
		sscanf(tok, "%X", &NumberData);

		if (debug)
			printf("NumberData : %d\n", NumberData);

		if (type == 0) {
			data.dist_len1 = NumberData;
		} else if (type == 1) {
			data.dist_len2 = NumberData;
		} else if (type == 2) {
			data.rssi_len1 = NumberData;
		} else if (type == 3) {
			data.rssi_len2 = NumberData;
		}

		for (int i = 0; i < NumberData; i++) {
			int dat;
			tok = strtok(NULL, " "); //data
			sscanf(tok, "%X", &dat);

			if (type == 0) {
				data.dist1[i] = dat;
			} else if (type == 1) {
				data.dist2[i] = dat;
			} else if (type == 2) {
				data.rssi1[i] = dat;
			} else if (type == 3) {
				data.rssi2[i] = dat;
			}

		}
	}

	tok = strtok(NULL, " "); //NumberChannels8Bit
	int NumberChannels8Bit;
	sscanf(tok, "%d", &NumberChannels8Bit);
	if (debug)
		printf("NumberChannels8Bit : %d\n", NumberChannels8Bit);
	for (int i = 0; i < NumberChannels8Bit; i++) {
		int type = -1;
		char content[6];
		tok = strtok(NULL, " "); //MeasuredDataContent
		sscanf(tok, "%s", content);
		if (!strcmp(content, "DIST1")) {
			type = 0;
		} else if (!strcmp(content, "DIST2")) {
			type = 1;
		} else if (!strcmp(content, "RSSI1")) {
			type = 2;
		} else if (!strcmp(content, "RSSI2")) {
			type = 3;
		}
		tok = strtok(NULL, " "); //ScalingFactor
		tok = strtok(NULL, " "); //ScalingOffset
		tok = strtok(NULL, " "); //Starting angle
		tok = strtok(NULL, " "); //Angular step width
		tok = strtok(NULL, " "); //NumberData
		int NumberData;
		sscanf(tok, "%X", &NumberData);

		if (debug)
			printf("NumberData : %d\n", NumberData);

		if (type == 0) {
			data.dist_len1 = NumberData;
		} else if (type == 1) {
			data.dist_len2 = NumberData;
		} else if (type == 2) {
			data.rssi_len1 = NumberData;
		} else if (type == 3) {
			data.rssi_len2 = NumberData;
		}
		for (int i = 0; i < NumberData; i++) {
			int dat;
			tok = strtok(NULL, " "); //data
			sscanf(tok, "%X", &dat);

			if (type == 0) {
				data.dist1[i] = dat;
			} else if (type == 1) {
				data.dist2[i] = dat;
			} else if (type == 2) {
				data.rssi1[i] = dat;
			} else if (type == 3) {
				data.rssi2[i] = dat;
			}
		}
	}

}

void LMS1xx::saveConfig() {
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sMN mEEwriteall", 0x03);

	write(sockDesc, buf, strlen(buf));

	int len = read(sockDesc, buf, 100);
	//	if (buf[0] != 0x02)
	//		std::cout << "invalid packet recieved" << std::endl;
	//	if (debug) {
	//		buf[len] = 0;
	//		std::cout << buf << std::endl;
	//	}
}

void LMS1xx::startDevice() {
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sMN Run", 0x03);

	write(sockDesc, buf, strlen(buf));

	int len = read(sockDesc, buf, 100);
	//	if (buf[0] != 0x02)
	//		std::cout << "invalid packet recieved" << std::endl;
	//	if (debug) {
	//		buf[len] = 0;
	//		std::cout << buf << std::endl;
	//	}
}