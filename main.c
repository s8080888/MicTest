// #include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "TestAPI.h"

#define true 1
#define false 0

pthread_t tid_mian,tid_watchDog;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
uchar *node;
uchar value;

void *WatchStatusDog()
{
    while (1)
    {
        pthread_mutex_lock(&mutex);

        GetMicConnectionStatus(0x01);
        GetMicConnectionStatus(0x02);

        pthread_mutex_unlock(&mutex);
        Sleep(1);
    }
}
void *command()
{
    uchar cmd;
    node = (uchar *)malloc(sizeof(uchar) * 1);
    *node = 1;

    printf("Please Key the cmd:\n");
    printf("key 0: change the node.\n");
    printf("key 1: GetGet MIC Connection Status \n");
    printf("key 2: Get MIC Mute Status \n");
    printf("key 3: Get MIC Battery Level \n");
    printf("key 4: Get MIC TX Gain \n");
    printf("key 5: Get MIC BD Address \n");
    printf("key 6: Get RX Volume \n");
    printf("key 7: Send SOS Alarm \n");
    printf("key 8: Set MIC Mute Status \n");
    printf("key 9: Set MIC TX Gain \n");
    printf("key 10: Set RX Volume \n");
    printf("key 11: Set SOS Signal \n");
    printf("key 12: Exit.\n");
    while(1)
    {
        scanf("%d", &cmd);
        pthread_mutex_lock(&mutex);
        switch(cmd)
        {
            case 0:
                printf("Change the Node 1/2:");
                scanf("%d",node);
                *node = *node >= 2 ? 2 : 1;
                break;
            case 1:
                GetMicConnectionStatus(*node);
                break;
            case 2:
                printf("%d", *node);
                GetMicMuteStauts(*node);
                break;
            case 3:
                GetMicBatteryLevel(*node);
                break;
            case 4:
                GetMicTxGain(*node);
                break;
            case 5:
                GetMicBDaddress(*node);
                break;
            case 6:
                GetRxVolume();
                break;
            case 7:
                printf("Send SOS Alarm");
                break;
            case 8:
                printf("Please enter Mute Status 0/1 \n");
                scanf("%d", &value);
                printf("\n%d, %d", node, value);
                SetMicMuteStatus(*node, value);
                break;
            case 9:
                printf("Please enter MIC TX Gain 0 ~ 10: \n");
                scanf("%d", &value);
                SetMicTxGain(*node, value);
                break;
            case 10:
                printf("Please enter RX Volume 0 ~ 10: \n");
                scanf("%d", &value);
                SetRxVolume(value);
                printf("Send Over. \n");
                break;
            case 11:
                printf("Please Set SOS Signal 0/1: \n");
                scanf("%d", &value);
                SetSOSSignal(*node, value);
                break;
            default:
                fprintf(stderr, "Exit.");
                pthread_exit(&tid_mian);
                pthread_exit(&tid_watchDog);
                break;
            }
        printf("\n---------------------------------------\n");
        printf("\n");
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[])
{
    SerialPortInit("COM7");
    Sleep(500);
    if (pthread_create(&tid_watchDog, NULL, WatchStatusDog, NULL) != 0)
    {
        fprintf(stderr, "Create WatchDog thread Error.");
    }
    if(pthread_create(&tid_mian, NULL, command, NULL) != 0)
    {
        fprintf(stderr, "Create Command thread Error.");
    }

    /* Access and Init Serial */

    // pthread_detach(tid_watchDog);
    pthread_join(tid_mian,NULL);

    return 0;
}