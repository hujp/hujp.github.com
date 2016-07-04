// pthread_mutex_t.c

#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

char buffer[1024];
pthread_mutex_t rlock;

void *counter(void *args);

int main(void)
{
    int ret;
    char *retstr = NULL;
    pthread_t thrd;
    
    ret = pthread_mutex_init(&rlock, NULL);
    if(ret != 0) {
        printf("mutex init failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_create(&thrd, NULL, counter, NULL);
    if(ret != 0) {
        printf("thread create failed.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("input some words:\n");
    
    while(1) {
        pthread_mutex_lock(&rlock);
        scanf("%s", buffer);
        pthread_mutex_unlock(&rlock);
        if(strncmp("end", buffer, 3) == 0){
            break;
        }
        sleep(1);
    }
    
    ret = pthread_join(thrd, (void*)retstr);
    if(ret != 0) {
        printf("thread join failed.\n");
        exit(EXIT_FAILURE);
    }
    
    pthread_mutex_destroy(&rlock);
    
    exit(EXIT_SUCCESS);
}

void *counter(void *args)
{
    sleep(1);

    while(1) {
        pthread_mutex_lock(&rlock);

        if(strncmp("end", buffer, 3) == 0) {
            pthread_mutex_unlock(&rlock);
            break;
        }

        // 1. nothing input, nothing output.
        // 2. main thread may not overwrite the buffer.
        if(strlen(buffer) != 0) {
            printf("you input %d character(s).\n", (int)strlen(buffer));
        }

        buffer[0] = 0;

        pthread_mutex_unlock(&rlock);
        sleep(1);
    }
    
    pthread_exit(NULL);
}