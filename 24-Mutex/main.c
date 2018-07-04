#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREAD 100

long long num = 0;
pthread_mutex_t mutex;


void *thread_inc(void *arg)
{
    int i = 0;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < 5000; i++)
    {
        num += 1;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_des(void *arg)
{
    int i = 0;
    for(i = 0; i < 5000; i++)
    {
        pthread_mutex_lock(&mutex);
        num -= 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t thread_id[NUM_THREAD];
    int i;

    pthread_mutex_init(&mutex, NULL);

    for(i = 0; i < NUM_THREAD; i++)
    {
        if(i%2 == 0)
        {
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        }
        else
        {
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
        }
    }

    for(i = 0; i < NUM_THREAD; i++)
        pthread_join(thread_id[i], NULL);

    pthread_mutex_destroy(&mutex);

    printf("Result: %lld \n", num);
    return 0;
}
