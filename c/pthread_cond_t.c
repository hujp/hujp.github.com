// pthread_cond_t.c

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 26

pthread_mutex_t g_mutex;
pthread_cond_t g_cond;
char g_buf[BUFFER_SIZE];

void *thread_func_producer(void *arg);
void *thread_func_consumer(void *arg);

int main(void)
{
    pthread_t thrd_producer;
    pthread_t thrd_consumer;
    int ret;
    
    ret = pthread_mutex_init(&g_mutex, NULL);
    if(ret != 0) {
        printf("Init mutex failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_cond_init(&g_cond, NULL);
    if(ret != 0) {
        printf("Init condition failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_create(&thrd_producer, NULL, thread_func_producer, NULL);
    if(ret != 0) {
        printf("Create producer failed.\n");
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&thrd_consumer, NULL, thread_func_consumer, NULL);
    if(ret != 0) {
        printf("Create consumer failed.\n");
        exit(EXIT_FAILURE);
    }
    
    sleep(1);
    
    ret = pthread_join(thrd_producer, NULL);
    if(ret != 0) {
        printf("Join producer failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_join(thrd_consumer, NULL);
    if(ret != 0) {
        printf("Join consumer failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_cond_destroy(&g_cond);
    if(ret != 0) {
        printf("Destroy condition failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_mutex_destroy(&g_mutex);
    if(ret != 0) {
        printf("Destroy mutex failed.\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}

void *thread_func_producer(void *arg)
{
    int i;
    
    printf("Producer running...\n");
    
    pthread_mutex_lock(&g_mutex);
    for(i = 0; i < BUFFER_SIZE; i++) {
        g_buf[i] = (char)('A' + i);
    }
    pthread_cond_signal(&g_cond);
    pthread_mutex_unlock(&g_mutex);
    
    printf("Producer exit.\n");
    
    pthread_exit(NULL);
}

void *thread_func_consumer(void *arg)
{
    int i;
    
    printf("Consumer running...\n");
    
    pthread_mutex_lock(&g_mutex);
    pthread_cond_wait(&g_cond, &g_mutex);
    for(i = BUFFER_SIZE - 1; i >= 0; i--) {
        printf("%c", g_buf[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&g_mutex);
    
    printf("consumer exit.\n");
    
    pthread_exit(NULL);
}