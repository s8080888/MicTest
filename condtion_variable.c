#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>

pthread_cond_t tid_cd1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t tid_mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid1,tid2;
int flag = 1;
int count = 0;
void test1()
{
    while(1)
    {
        pthread_mutex_lock(&tid_mutex1);

        int m_count = rand();
        count += m_count;
        printf("test 1 product the count: %d.   ", count);
        Sleep(500);

        if (count % 2 == 0)
        {
            printf("Count is odd.\n\n");
            flag = 1;
        }
        else{
            printf("Count not odd \n");
            flag = 0;
        }

        pthread_mutex_unlock(&tid_mutex1);
        pthread_cond_signal(&tid_cd1);
        Sleep(50);
    }
}

void test2()
{
    while(1)
    {
        pthread_mutex_lock(&tid_mutex1);
        while(flag == 0)
        // if(flag == 0)
            pthread_cond_wait(&tid_cd1, &tid_mutex1);

        printf("test 2 Thread start ...\nwait 3s\n");
        Sleep(3000);
        count = rand();
        printf("test 2 sum: %d. \n\n", count);

        pthread_mutex_unlock(&tid_mutex1);
        Sleep(100);
    }
}

int main()
{
    printf("start ... \n");
    pthread_create(&tid1, NULL, (void *)test1, NULL);
    pthread_create(&tid2, NULL, (void *)test2, NULL);
    // pthread_detach(tid2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}