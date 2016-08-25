#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int fd_pipes[2];
    pid_t pid;
    // return value from common system call
    int i_ret = 0;
    int i_bytes_read = 0;
    char str_buf_array[BUFSIZ + 1];
    char* str_data = "Hello, PIPE!";
    
    memset(str_buf_array, '\0', sizeof(str_buf_array));
    
    if(pipe(fd_pipes) == 0) {
        pid = fork();
       
        if(pid == -1) {
            printf("fork error.\n");
            exit(EXIT_FAILURE);
        } else if(pid == 0) {
            // chaild process
            i_bytes_read = read(fd_pipes[0], str_buf_array, BUFSIZ);
           
            if(i_bytes_read < 0) {
                printf("read error.\n");
                exit(EXIT_FAILURE);
            }
           
            str_buf_array[i_bytes_read] = '\0';
            printf("%d : %d bytes read - %s.\n",
                getpid(), i_bytes_read, str_buf_array);
            exit(EXIT_SUCCESS);
        } else if(pid > 0) {
            // father process
            i_ret = write(fd_pipes[1], str_data, strlen(str_data));
            
            if(i_ret == -1) {
                printf("write error.\n");
                exit(EXIT_FAILURE);
            }
            
            printf("%d : %d bytes write - %s.\n",
                getpid(), i_ret, str_data);
            exit(EXIT_SUCCESS);
        }
    }
    
    printf("pipe error.\n");
    exit(EXIT_FAILURE);
}