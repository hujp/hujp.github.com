#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    char *src = "abcdefg";
    char *p2;
    char *p3;
    
    // bad 1
    //p2 = (char*)malloc(strlen(src) - 1);
    //p3 = (char*)memcpy(p2, src, strlen(src));
    //printf("p2=%s\np3=%s", p2, p3);
    //free(p2);
    
    // bad 2
    //p2 = src + 1;
    //p3 = (char*)memcpy(p2, src, strlen(src));
    //printf("p2=%s\np3=%s", p2, p3);
    //free(p2);
    
    // bad 3
    //char p1[10] = {'a',"b","c","d","e","f","g"};
    //p2 = p1 + 1;
    //p3 = (char*)memcpy(p2, p1, 10);
    //printf("p2=%s\np3=%s", p2, p3);
    
    p2 = NULL;
    p3 = NULL;
    exit(EXIT_SUCCESS);
}