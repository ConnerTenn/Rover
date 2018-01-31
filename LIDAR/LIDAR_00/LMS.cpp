
#include "LMS.h"

LMS::LMS(LIDAR *lidar, NetworkManager *network)
{
	Lidar = lidar;
	Network = network;
}

LMS::~LMS()
{

}

void LMS::Send(char *buf, int size)
{
	if (Network->Send(buf, 100) == -1)
	{
		Lidar->DebugConsole->Write("Error Sending Data\n");
	}
}

void LMS::Recv(char *buf, int size)
{
	if (Network->Recv(buf, 100) == -1)
	{
		Lidar->DebugConsole->Write("Error Recieving Data");
	}
	else
	{
		Lidar->DebugConsole->Write(std::string("Received:") + buf);
	}
}

void LMS::Start()
{
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sMN LMCstartmeas", 0x03);

	Send(buf, strlen(buf));

	Recv(buf, 100);
}

void LMS::Stop()
{
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sMN LMCstopmeas", 0x03);

	Network->Send(buf, strlen(buf));

	Recv(buf, 100);
}

Status LMS::GetStatus()
{
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sRN STlms", 0x03);

	Send(buf, strlen(buf));

	Recv(buf, 100);

	int status;
	sscanf((buf + 10), "%d", &status);

	return (Status)status;
}

void LMS::Login()
{
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sMN SetAccessMode 03 F4724744", 0x03);

	Send(buf, strlen(buf));

	Recv(buf, 100);
}

void LMS::StartDevice()
{
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sMN Run", 0x03);

	Send(buf, strlen(buf));

	Recv(buf, 100);
}

ScanConfig LMS::GetScanConfig()
{
	ScanConfig cfg;
	char buf[100];
	sprintf(buf, "%c%s%c", 0x02, "sRN LMPscancfg", 0x03);

	Send(buf, strlen(buf));

	Recv(buf, 100);

	sscanf(buf + 1, "%*s %*s %X %*d %X %X %X", &cfg.Frequency, &cfg.Resolution, &cfg.StartAngle, &cfg.StopAngle);
	return cfg;
}

void LMS::SetScanConfig(ScanConfig cfg)
{
	char buf[100];
	
	sprintf(buf, "%c%s %X +1 %X %X %X%c", 0x02, "sMN mLMPsetscancfg", cfg.Frequency, cfg.Resolution, cfg.StartAngle, cfg.StopAngle, 0x03);

	Send(buf, strlen(buf));

	Recv(buf, 100);
}

void LMS::SetScanDataConfig(ScanDataConfig cfg)
{
	char buf[100];
	
	sprintf(buf, "%c%s %02X 00 %d %d 0 %02X 00 %d %d 0 %d +%d%c", 0x02,
		"sWN LMDscandatacfg", cfg.OutputChannel, cfg.Remission,
		cfg.Resolution, cfg.Encoder, cfg.Position,
		cfg.DeviceName, cfg.Timestamp, cfg.OutputInterval, 0x03);
	
	Send(buf, strlen(buf));

	Recv(buf, 100);
}

void LMS::SaveConfig()
{
	char buf[100];
	
	sprintf(buf, "%c%s%c", 0x02, "sMN mEEwriteall", 0x03);

	Send(buf, strlen(buf));

	Recv(buf, 100);
}

/*Potential Protocol Issue. Do Not Call*/
void LMS::ScanContinous(bool start)
{
	char buf[100];
	throw("");
	sprintf(buf, "%c%s %d%c", 0x02, "sEN LMDscandata", start, 0x03);

	Send(buf, strlen(buf));

	Recv(buf, 100);

	/*
	#### #### WARNING #### ####
	Possible Unintended Behaviour
	if (start = 0)
	*/
	if (start == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			Recv(buf, 100);
		}
	}
}

void LMS::GetData(ScanData *data)
{
	char buf[20000];
	fd_set rfds; //WinSock struct
	struct timeval tv; //WinSock struct
	int retval, len = 0;

	do
	{
		FD_ZERO(&rfds);
		FD_SET(Network->SocketData.Socket, &rfds);

		tv.tv_sec = 0;
		tv.tv_usec = 50000;
		retval = select(Network->SocketData.Socket + 1, &rfds, NULL, NULL, &tv);
		if (retval)
		{
			len += Network->Recv(buf + len, 20000 - len);
		}
	}
	while ((buf[0] != 0x02) || (buf[len - 1] != 0x03));

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
	for (int i = 0; i < NumberEncoders; i++)
	{
		tok = strtok(NULL, " "); //EncoderPosition
		tok = strtok(NULL, " "); //EncoderSpeed
	}

	tok = strtok(NULL, " "); //NumberChannels16Bit
	int NumberChannels16Bit;
	sscanf(tok, "%d", &NumberChannels16Bit);
	
	Lidar->DebugConsole->Write(std::string("NumberChannels16Bit: ") + std::to_string(NumberChannels16Bit) + "\n");

	for (int i = 0; i < NumberChannels16Bit; i++)
	{
		int type = -1; // 0 DIST1 1 DIST2 2 RSSI1 3 RSSI2
		char content[6];
		tok = strtok(NULL, " "); //MeasuredDataContent
		sscanf(tok, "%s", content);
		if (!strcmp(content, "DIST1"))
		{
			type = 0;
		}
		else if (!strcmp(content, "DIST2"))
		{
			type = 1;
		}
		else if (!strcmp(content, "RSSI1"))
		{
			type = 2;
		}
		else if (!strcmp(content, "RSSI2"))
		{
			type = 3;
		}
		tok = strtok(NULL, " "); //ScalingFactor
		tok = strtok(NULL, " "); //ScalingOffset
		tok = strtok(NULL, " "); //Starting angle
		tok = strtok(NULL, " "); //Angular step width
		tok = strtok(NULL, " "); //NumberData
		int NumberData;
		sscanf(tok, "%X", &NumberData);

		Lidar->DebugConsole->Write(std::string("NumberData: ") + std::to_string(NumberData) + "\n");

		if (type == 0)
		{
			data->DistSize1 = NumberData;
		}
		else if (type == 1)
		{
			data->DistSize2 = NumberData;
		}
		else if (type == 2)
		{
			data->RssiSize1 = NumberData;
		}
		else if (type == 3)
		{
			data->RssiSize2 = NumberData;
		}

		for (int i = 0; i < NumberData; i++)
		{
			int dat;
			tok = strtok(NULL, " "); //data
			sscanf(tok, "%X", &dat);

			if (type == 0)
			{
				data->Dist1[i] = dat;
			}
			else if (type == 1)
			{
				data->Dist2[i] = dat;
			}
			else if (type == 2)
			{
				data->Rssi1[i] = dat;
			}
			else if (type == 3)
			{
				data->Rssi2[i] = dat;
			}

		}
	}

	tok = strtok(NULL, " "); //NumberChannels8Bit
	int NumberChannels8Bit;
	sscanf(tok, "%d", &NumberChannels8Bit);
	
	Lidar->DebugConsole->Write(std::string("NumberChannels8Bit: ") + std::to_string(NumberChannels8Bit) + "\n");

	for (int i = 0; i < NumberChannels8Bit; i++)
	{
		int type = -1;
		char content[6];
		tok = strtok(NULL, " "); //MeasuredDataContent
		sscanf(tok, "%s", content);
		if (!strcmp(content, "DIST1"))
		{
			type = 0;
		}
		else if (!strcmp(content, "DIST2"))
		{
			type = 1;
		}
		else if (!strcmp(content, "RSSI1"))
		{
			type = 2;
		}
		else if (!strcmp(content, "RSSI2"))
		{
			type = 3;
		}
		tok = strtok(NULL, " "); //ScalingFactor
		tok = strtok(NULL, " "); //ScalingOffset
		tok = strtok(NULL, " "); //Starting angle
		tok = strtok(NULL, " "); //Angular step width
		tok = strtok(NULL, " "); //NumberData
		int NumberData;
		sscanf(tok, "%X", &NumberData);

		Lidar->DebugConsole->Write(std::string("NumberData: ") + std::to_string(NumberData) + "\n");

		if (type == 0)
		{
			data->DistSize1 = NumberData;
		}
		else if (type == 1)
		{
			data->DistSize2 = NumberData;
		}
		else if (type == 2)
		{
			data->RssiSize1 = NumberData;
		}
		else if (type == 3)
		{
			data->RssiSize2 = NumberData;
		}
		for (int i = 0; i < NumberData; i++)
		{
			int dat;
			tok = strtok(NULL, " "); //data
			sscanf(tok, "%X", &dat);

			if (type == 0)
			{
				data->Dist1[i] = dat;
			}
			else if (type == 1)
			{
				data->Dist2[i] = dat;
			}
			else if (type == 2)
			{
				data->Rssi1[i] = dat;
			}
			else if (type == 3)
			{
				data->Rssi2[i] = dat;
			}
		}
	}

}



