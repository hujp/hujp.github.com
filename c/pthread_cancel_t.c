// pthread_cancel_t.c

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)

void *thread_func_cancelable(void *args);

int main(void)
{
    pthread_t thrd_cancelable;
    int ret;
    char *retstr = NULL;
    
    ret = pthread_create(&thrd_cancelable, NULL, thread_func_cancelable, NULL);
    if(ret != 0) {
        printf("Create thread failed.\n");
        exit(EXIT_FAILURE);
    }
    
    sleep(2);
    
    ret = pthread_cancel(thrd_cancelable);
    if(ret != 0) {
        printf("Cancel thread faild.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_join(thrd_cancelable, (void*)retstr);
    if(ret != 0) {
        printf("Join thread failed.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Thread canceled.\n");
    
    exit(EXIT_SUCCESS);
}

void *thread_func_cancelable(void *args)
{
    int ret;
    int i;
    
    ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if(ret != 0) {
        printf("Set thread cancel state failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    if(ret != 0) {
        printf("Set thread cancel type failed.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("%lu is running...\n", gettid());
    
    for(i = 0; i < 10; i++) {
        printf("%lu is still running...\n", gettid());
        sleep(1);
    }
    
    pthread_exit(NULL);
}