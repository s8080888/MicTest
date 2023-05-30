#include "TestAPI.h"
#include <Windows.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

TPotocol potocol;
uchar ReceiveBuffer[15];
DWORD WrittenBytes, ReadBytes;
HANDLE Serial;
DCB dcbSerialParams = {0};
COMMTIMEOUTS timeouts = {0};

clock_t timer_start, timer_end;

void SerialPortInit(const char port[])
{
	/* Open Serial port and Set inital UART Paramters */
	Serial = CreateFile(port,						  // port Name
						GENERIC_READ | GENERIC_WRITE, // Read / Write
						0,							  // No Share
						NULL,						  // No Security
						OPEN_EXISTING,				  // Open existing port only
						0,							  // Non Overlapped I/O
						NULL);						  // Null for comm Device.
	if (Serial == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error\n");
	}
	else
	{
		fprintf(stderr, "Open Serial Port.\n");
	}

	/* Ser device parameters (115200 baud, 1 start bit and 1 stop bit, no parity) */
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(Serial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error getting device state.\n");
		CloseHandle(Serial);
		return;
	}

	dcbSerialParams.BaudRate = CBR_115200;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (SetCommState(Serial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error setting device parameters.\n");
	}
	else
	{
		fprintf(stderr, "Set UART paramters OK.\n");
	}

	// Set COM port timeout settings
	timeouts.ReadIntervalTimeout = 5;
	timeouts.ReadTotalTimeoutConstant = 5;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 5;
	timeouts.WriteTotalTimeoutMultiplier = 1;

	if (SetCommTimeouts(Serial, &timeouts) == 0)
	{
		fprintf(stderr, "Error setting timeouts. \n");
		CloseHandle(Serial);
		return;
	}
	else
	{
		fprintf(stderr, "Set COM port timeout OK.\n");
	}
	printf("\n");
}
void Writecommand(uchar Opcode, uchar Length, uchar index, uchar Node, uchar Value)
{
	potocol.Opcode = Opcode;
	potocol.Length = Length;
	potocol.index = index;
	potocol.Node = Node;
	potocol.Value = Value;

	// fprintf(stderr, "Sending ... \n");
	if (!WriteFile(Serial, &potocol, sizeof(potocol), &WrittenBytes, NULL))
	{
		fprintf(stderr, "Written Error. \n");
		CloseHandle(Serial);
		return;
	}
	timer_start = clock();
	// fprintf(stderr, "send %d bytes data over.\n", WrittenBytes);
	// printf("\n");
}

void ReadCommand()
{
	Emethod Opcode;
	char node;
	char* status;

	PurgeComm(Serial, PURGE_RXABORT);
	do
	{
		BOOL book = ReadFile(Serial, ReceiveBuffer, sizeof(ReceiveBuffer) - 1,
							 &ReadBytes, NULL);
		if (book && (ReadBytes > 0))
		{
			ReceiveBuffer[ReadBytes] = '\0';
		}
	} while (ReadBytes == 0);

	timer_end = clock();
	Opcode = ReceiveBuffer[0];
	node = ReceiveBuffer[3];


	switch(Opcode)
	{
	case EGetConnectStatus:
		return;

	case EGetMuteStatus:
		status = ReceiveBuffer[4] == 1 ? "Mute" : "UnMute";
		printf("Mic %d : %s", node, status);
		break;

	case EGetBatteryLevel:
		printf("Mic %d Battery : %d %%", node, ReceiveBuffer[4] * 25);
		break;

	case EGetTxGain:
		printf("Mic %d Tx Gain : %d", node, ReceiveBuffer[4]);
		break;

	case EGetBDAddress:
		printf("Mic %d BD Address :\n", node);
		for (int i = 4; i < ReadBytes; i++)
		{
			printf("%d ,", ReceiveBuffer[i]);
		}
		break;

	case EGetRXVolume:
		printf("Mic RX Volume : %d", ReceiveBuffer[4]);
		break;

	case EGetFWversion:
		printf("Mic %d FW Version : %d.%d", ReceiveBuffer[4], ReceiveBuffer[5]);
		break;

	case EGetSOSstatus:
		printf("Mic %d SOS status : %d", node, ReceiveBuffer[4]);
		break;
	default:
		printf("Unknow Opcode.");
		return;
	}
	printf("\n");
	int mesc = ((timer_end - timer_start) * 1000) / CLOCKS_PER_SEC;
	// printf("Time taken %d second %d millisecond. \n", mesc / 1000, mesc % 1000);
}
void TimeDelay(int time)
{
	Sleep(time);
}

unsigned char GetMicConnectionStatus(uchar node)
{
	Writecommand(READ, 0x02, 0x01, node, 0x00);
	ReadCommand();
}
unsigned char GetMicMuteStauts(uchar node)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(READ, 0x02, 0x02, node, 0x00);
	ReadCommand();
}
unsigned char GetMicBatteryLevel(uchar node)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(READ, 0x02, 0x03, node, 0x00);
	ReadCommand();
}
unsigned char GetMicTxGain(uchar node)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(READ, 0x02, 0x04, node, 0x00);
	ReadCommand();
}
unsigned char GetMicBDaddress(uchar node)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(READ, 0x02, 0x05, node, 0x00);
	ReadCommand();
}
unsigned char GetRxVolume()
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(READ, 0x02, 0x06, 0x00, 0x00);
	ReadCommand();
}
unsigned char GetSOSstatus(uchar node)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(READ, 0x02, 0x07, node, 0x00);
	ReadCommand();
}
unsigned char GetFWversion(uchar node)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(READ, 0x02, 0x08, node, 0x00);
	ReadCommand();
}

void SendSOSAlarm()
{
	printf("--- Called Function: %s ---\n", __func__);
}
void SetMicMuteStatus(uchar node, uchar mute)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(Control, 0x03, 0x01, node, mute);
	Sleep(500);
	GetMicMuteStauts(node);
}
void SetMicTxGain(uchar node, uchar Gain)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(Control, 0x03, 0x02, node, Gain);
	Sleep(500);
	GetMicTxGain(node);
}
void SetRxVolume(uchar Gain)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(Control, 0x03, 0x03, 0x00, Gain);
	Sleep(500);
	GetRxVolume();
}
void SetSOSSignal(uchar node, uchar status)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(Control, 0x03, 0x04, node, status);
}

void SetSOSstatus(uchar node, uchar status)
{
	printf("--- Called Function: %s ---\n", __func__);
	Writecommand(Control, 0x03, 0x04, node, status);
	Sleep(500);
	GetSOSstatus(node);
}