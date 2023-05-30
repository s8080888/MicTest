#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct potocol
{
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char d;
} potocol;
HANDLE hSerial;
int main()
{
    // OPEN SERIAL PORT AND SET INITAL UART PARAMETERS
    //=================================================
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    fprintf(stderr, "Opening serial port...");
    hSerial = CreateFile("\\\\.\\COM7", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Error\n");
        return 1;
    }
    else
    {
        fprintf(stderr, "OK\n");
    }

    // Set device parameters (115200 baud, 1 start bit, 1 stop bit, no parity)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error getting device state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (SetCommTimeouts(hSerial, &timeouts) == 0)
    {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    // SETUP AND SEND DATA FROM UART
    //==============================
    int VarNum = 8;

    char str[15], buffer[15];
    
    sprintf(str, "%ld", VarNum);
    DWORD bytes_written, bytesRead, total_bytes_written = 0;
    fprintf(stderr, "Sending bytes...");
    if (!WriteFile(hSerial, &str, sizeof(str), &bytes_written, NULL))
    {
        fprintf(stderr, "Error\n");
        CloseHandle(hSerial);
        return 1;
    }
    fprintf(stderr, "%d bytes written\n", bytes_written);
    do
    {
        BOOL bOk = ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
        if (bOk && (bytesRead > 0))
        {
            buffer[bytesRead] = '\0';
        }
    } while (bytesRead == 0);
    int i = 0;
    while (buffer[i] != '\0')
    {
        i++;
        printf("index: %d ,result: %x \n",i, buffer[i]);
    }

    // CLOSE SERIAL PORT AND EXIT MAIN FUNCTION
    //=========================================
    fprintf(stderr, "Closing serial port...");

    if (CloseHandle(hSerial) == 0)
    {
        fprintf(stderr, "Error\n");
        return 1;
    }
    fprintf(stderr, "OK\n");
    return 0;
}