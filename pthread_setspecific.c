#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
pthread_key_t key;
pthread_t thid1;
pthread_t thid2;

#define DATA05  0x05
#define DATA04  0x04
#define DATA03  0x03

void* thread2(void* arg)
{
    printf("thread:%lu is running\n", 2lu);
    pthread_setspecific(key, (void *)(uintptr_t)DATA05);
    printf("thread:%lu return %p\n", 2lu, pthread_getspecific(key));
}

void* thread1(void* arg)
{
    printf("thread:%lu is running\n", 1lu);
    pthread_setspecific(key, (void *)(uintptr_t)DATA04);
    pthread_create(&thid2, NULL, thread2, NULL);
    printf("thread:%lu return %p\n", 1lu, pthread_getspecific(key));
}

int main()
{
    printf("main thread:%lu is running\n", pthread_self());
    pthread_key_create(&key, NULL);
    pthread_create(&thid1, NULL, thread1, NULL);
    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);
    pthread_setspecific(key, (void *)(uintptr_t)DATA03);
    printf("thread:%lu return %p\n", pthread_self(), pthread_getspecific(key));
    pthread_key_delete(key);
    printf("main thread exit\n");
    return 0;
}