// pthread_sched_t.c

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void *thread_func_shower(void *args);

int main(void)
{
    pthread_t thrd_shower;
    pthread_attr_t attr;
    struct sched_param param;
    int ret;
    
    ret = pthread_attr_init(&attr);
    if(ret != 0) {
        printf("Init attribute failed.\n");
        exit(EXIT_FAILURE);
    }
    
    param.sched_priority = 10;
    
    ret = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if(ret != 0) {
        printf("Set policy failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_attr_setschedparam(&attr, &param);
    if(ret != 0) {
        printf("Set param failed.\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if(ret != 0) {
        printf("Set inherit failed.\n");
        exit(EXIT_FAILURE);
    }
    
#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING)
    ret = pthread_create(&thrd_shower, &attr, thread_func_shower, NULL);
#else
    ret = pthread_create(&thrd_shower, NULL, thread_func_shower, NULL);
#endif
    if(ret != 0) {
        switch(ret) {
            case EAGAIN:
                printf("EAGAIN.\n");
                break;
            case EINVAL:
                printf("EINVAL.\n");
                break;
            case EPERM:
                printf("EPERM.\n");
                break;
            default:
                printf("Unknown!\n");
                break;
        }
        printf("Create thread failed.\n");
        exit(EXIT_FAILURE);
    }
    
    sleep(3);
    
    ret = pthread_join(thrd_shower, NULL);
    if(ret != 0) {
        printf("Join thread failed.\n");
        exit(EXIT_FAILURE);
    }
    
    pthread_attr_destroy(&attr);
    
    exit(EXIT_SUCCESS);
}

void *thread_func_shower(void *args)
{
    int policy;
    struct sched_param param;
    int ret;
    
    ret = pthread_getschedparam(pthread_self(), &policy, &param);
    if(ret != 0) {
        printf("Get schedule parameter failed.\n");
        pthread_exit("Get schedule parameter failed.");
    }
    
    switch(policy) {
        case SCHED_OTHER:
            printf("Get policy other.\n");
            break;
        case SCHED_FIFO:
            printf("Get policy fifo.\n");
            break;
        case SCHED_RR:
            printf("Get policy rr.\n");
            break;
        default:
            printf("Unknown.\n");
            break;
    }
    
    pthread_exit(NULL);
}