#ifndef TEST_H
#define TEST_H
#include <windows.h>

#define true 1
#define false 0

#define READ 0x55
#define Control 0x66

typedef unsigned char uchar;
typedef uchar bool;
typedef struct _TPotocol
{
    uchar Opcode;
    uchar Length;
    uchar index;
    uchar Node;
    uchar Value;
} TPotocol;

typedef enum EMethod
{
    EGetConnectStatus = 0x21,
    EGetMuteStatus,
    EGetBatteryLevel,
    EGetTxGain,
    EGetBDAddress,
    EGetRXVolume,
    EGetSOSstatus,
    EGetFWversion,

    ESendSOSAlarm,
    ESetMuteStatus,
    ESetMicTxGain,
    ESetRxVolume,
    ESetSOSSignal,
    ESetSOSstatus

}Emethod;

typedef enum AutoTest
{
    Connect,
    Mute,
    Tx
} AutoTest;

extern TPotocol potocol;
extern uchar ReceiveBuffer[15];
extern DWORD WrittenBytes, ReadBytes;
extern HANDLE Serial;
extern DCB dcbSerialParams;
extern COMMTIMEOUTS timeouts;
extern uchar user_command;

void auto_test(uchar node);
void Writecommand(uchar Opcode, uchar Length, uchar index, uchar Node, uchar Value);
void ReadCommand();

void SerialPortInit(const char port[]);

unsigned char GetMicConnectionStatus(uchar node);
unsigned char GetMicMuteStauts(uchar node);
unsigned char GetMicBatteryLevel(uchar node);
unsigned char GetMicTxGain(uchar node);
unsigned char GetMicBDaddress(uchar node);
unsigned char GetRxVolume();
unsigned char GetSOSstatus(uchar node);
unsigned char GetFWversion(uchar node);

void SendSOSAlarm();
void SetMicMuteStatus(uchar node, uchar mute);
void SetMicTxGain(uchar node, uchar Gain);
void SetRxVolume(uchar Gain);
void SetSOSSignal(uchar node, uchar status);
void SetSOSstatus(uchar node, uchar status);
void TimeDelay(int time);
uchar Empty(uchar arr[], int size);
#endif /* TEST_H */
