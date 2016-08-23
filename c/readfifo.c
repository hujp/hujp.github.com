#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int main(void)
{
    const char* str_pipe_name = "/tmp/my_fifo";
    const char* str_data_file = "/tmp/readfromfifo.txt";
    int fd_pipe = 0;
    int fd_data = 0;
    char str_buf_array[PIPE_BUF + 1];
    // return value from read system call
    int i_bytes_read = 0;
    // sum value of recived bytes from named pipe
    int i_bytes_recived = 0;
    // return value from common system call
    int i_ret = 0;
    
    if(access(str_pipe_name, F_OK) == -1) {
        i_ret = mkfifo(str_pipe_name, 0777);
        
        if(i_ret != 0) {
            printf("create pipe failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("%d opening pipe...\n", getpid());
    fd_pipe = open(str_pipe_name, O_RDONLY);
    
    if(fd_pipe == -1) {
        printf("open pipe failed.\n");
        exit(EXIT_FAILURE);
    }
    
    fd_data = open(str_data_file, O_WRONLY | O_CREAT, 0644);
    
    if(fd_data == -1) {
        printf("open data file failed.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("%d opened pipe.\n", getpid());
    
    do {
        i_bytes_read = read(fd_pipe, str_buf_array, PIPE_BUF);
        if(i_bytes_read > 0) {
            i_bytes_recived += i_bytes_read;
            str_buf_array[i_bytes_read] = '\0';
            i_ret = write(fd_data, str_buf_array, i_bytes_read);
            
            if(i_ret == -1) {
                printf("write error.\n");
                exit(EXIT_FAILURE);
            }
        }
    } while (i_bytes_read > 0);
    
    close(fd_pipe);
    close(fd_data);
    exit(EXIT_SUCCESS);
}