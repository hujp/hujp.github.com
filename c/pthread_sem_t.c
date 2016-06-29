#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sem_t sem;

void *thread_func(void *args);

char buffer[1024];

int main(void)
{
    pthread_t thrd;
    int ret;
    
    ret = sem_init(&sem, 0, 0);
    if(ret != 0) {
        printf("semaphore init failed!\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_create(&thrd, NULL, thread_func, NULL);
    if(ret != 0) {
        printf("thread create failed!\n");
        exit(EXIT_FAILURE);
    }
    
    printf("please input text, end with 'end':\n");
    
    while(scanf("%s", buffer)) {
        sem_post(&sem);
        if(strncmp("end", buffer, 3) == 0) {
            break;
        }
    }
    
    ret = pthread_join(thrd, NULL);
    if(ret != 0) {
        printf("thread join failed!\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
    
}

void *thread_func(void *args)
{
    sem_wait(&sem);
    
    while(strncmp("end", buffer, 3) != 0) {
        printf("you input %lu character(s).\n", strlen(buffer));
        sem_wait(&sem);
    }
    
    pthread_exit(NULL);
}
