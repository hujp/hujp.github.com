#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const char *str_pipe_name = "/tmp/my_fifo";
    const char *str_data_file = "/tmp/data.txt";
    int fd_pipe = 0;
    int fd_data = 0;
    const int i_mode_write = O_WRONLY;
    // return value from read system call
    int i_bytes_read = 0;
    // sum of bytes read
    int i_bytes_sent = 0;
    char str_buf_array[PIPE_BUF + 1];
    // return value from common system call
    int i_ret;
    
    if(access(str_data_file, F_OK) == -1) {
        printf("no data to send.\n");
        exit(EXIT_SUCCESS);
    }
    
    if(access(str_pipe_name, F_OK) == -1) {
        i_ret = mkfifo(str_pipe_name, 0777);
        
        if(i_ret != 0) {
            printf("mkfifo failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    fd_data = open(str_data_file, O_RDONLY);
    if (fd_data == -1) {
        printf("open data file failed.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("%d openning fifo...\n", getpid());
    fd_pipe = open(str_pipe_name, i_mode_write);
    if (fd_pipe == -1) {
        printf("open pipe failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("%d opened fifo.\n", getpid());
    
    i_bytes_read = read(fd_data, str_buf_array, PIPE_BUF);
    str_buf_array[i_bytes_read] = '\0';
    while(i_bytes_read > 0) {
        i_ret = write(fd_pipe, str_buf_array, i_bytes_read);
        
        if(i_ret == -1) {
            printf("write failed.\n");
            exit(EXIT_FAILURE);
        }
        
        i_bytes_sent += i_bytes_read;
        i_bytes_read = read(fd_data, str_buf_array, PIPE_BUF);
        str_buf_array[i_bytes_read] = '\0';
    }
    
    close(fd_pipe);
    close(fd_data);
    exit(EXIT_SUCCESS);
}