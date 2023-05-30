#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>

void sample(const char str[])
{
    int k = 0;
    for (int i = 0; i < 5;i++)
    {
        Sleep(1000);
        printf("result :%s %d \n", str,k);
        k++;
    }
}

int main()
{
    pthread_t thread1,thread2;
    const char str[] = "this is const char str[]";
    int a = 1;
    pthread_create(&thread1, NULL, (void *)sample, &str);
    pthread_join(thread1, NULL);
    for (int i = 0; i < 20;i++)
    {
        Sleep(1000);
        printf("%s %d \n", "this is main", a);
        a++;
    }

    return 0;
}