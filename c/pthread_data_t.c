// pthread_data_t.c

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 10

typedef struct thread_data
{
    int thread_no;
}thread_data_t;

pthread_key_t g_key;

void show_thread_data();

void *thread_func_datasaver(void *arg);

void free_thread_data(void *arg);

int main(void)
{
    pthread_t thrd[THREAD_COUNT];
    thread_data_t *data;
    int ret;
    int i;
    
    pthread_key_create(&g_key, free_thread_data);
    
    for(i = 1; i <= THREAD_COUNT; i++) {
        data = (thread_data_t*)malloc(sizeof(thread_data_t));
        data->thread_no = i;
        ret = pthread_create(&thrd[i], NULL, thread_func_datasaver, (void*)data);
        if(ret != 0) {
            printf("Create thread failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    sleep(1);
    
    
    for(i = 1; i <= THREAD_COUNT; i++) {
        ret = pthread_join(thrd[i], NULL);
        if(ret != 0) {
            printf("Join thread failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    pthread_key_delete(g_key);
    
    exit(EXIT_SUCCESS);
}

void show_thread_data()
{
    thread_data_t *data = pthread_getspecific(g_key);
    printf("Thread %d.\n", data->thread_no);
}

void* thread_func_datasaver(void *arg)
{
    thread_data_t *data = (thread_data_t*)arg;
    printf("Start thread %d.\n", data->thread_no);
    pthread_setspecific(g_key, data);
    show_thread_data();
    printf("End thread %d.\n", data->thread_no);
    pthread_exit(NULL);
}

void free_thread_data(void *arg)
{
    thread_data_t *data = (thread_data_t*)arg;
    printf("Free thread %d.\n", data->thread_no);
    free(data);
}