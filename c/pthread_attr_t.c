// pthread_attr_t.c

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int thread_finished = 0;
char *message = "Hello, World!";

void *thrd_func_printer(void *args);

int main(void)
{
    pthread_attr_t attr;
    pthread_t thrd_printer;
    int ret;
    
    ret = pthread_attr_init(&attr);
    if(ret != 0) {
        printf("Init attribute failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(ret != 0) {
        printf("Set detached failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_create(&thrd_printer, &attr,
            thrd_func_printer, (void*)message);
    if(ret != 0) {
        printf("Create thread failed.\n");
        exit(EXIT_FAILURE);
    }
    
    pthread_attr_destroy(&attr);
    
    while(thread_finished == 0) {
        printf("Waiting for thread finish...\n");
        sleep(1);
    }
    
    printf("Thread finished.\n");
    
    exit(EXIT_SUCCESS);
}

void *thrd_func_printer(void *args)
{
    printf("Message: %s", (char*)args);
    
    sleep(3);
    thread_finished = 1;
    pthread_exit(NULL);
}