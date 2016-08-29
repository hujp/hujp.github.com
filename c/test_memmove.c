#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char str_src[10] = "abcdefg";
    char *str_dst = str_src + 1;
    char *str_ret;
    
    //str_ret = (char*)memcpy(str_dst, str_src, strlen(str_src) + 1 - 1);
    str_ret = (char*)memmove(str_dst, str_src, strlen(str_src) + 1 - 1);
    
    printf("str_src: %s\n", str_src);
    printf("str_dst: %s\n", str_dst);
    printf("str_ret: %s\n", str_ret);
    
    exit(EXIT_SUCCESS);
}