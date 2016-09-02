#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME_LEN 256
#define HASH_SIZE 62

typedef struct _filename_hash_node {
    char filename[FILENAME_LEN];
    struct _filename_hash_node *next;
} filename_hash_node, *p_filename_hash_node;

p_filename_hash_node hash_table[HASH_SIZE];

void filename_hash_init();
static int hash_filename(char* filename);
char* filename_hash_find(char* filename);
void filename_hash_insert(char* filename);
void filename_hash_remove(char* filename);

int main(void)
{
    char* filenames[] = {
        "abc",
        "123",
        "Acde",
        "!wa",
        "--d",
        "++1",
        NULL
    };
    int i;
    
    filename_hash_init();
    
    for(i = 0; filenames[i] != NULL; i++) {
        filename_hash_insert(filenames[i]);
    }
    
    printf("after insert.\n");
    printf("\tfind abc: %s\n", filename_hash_find("abc"));
    printf("\tfind -abc: %s\n", filename_hash_find("-abc"));
    printf("\tfind 123: %s\n", filename_hash_find("123"));
    printf("\tfind 456: %s\n", filename_hash_find("456"));
    printf("\tfind !wa: %s\n", filename_hash_find("!wa"));
    printf("\tfind wa: %s\n", filename_hash_find("wa"));
    
    filename_hash_remove("abc");
    
    printf("\nafter remove abc.\n");
    printf("\tfind abc: %s\n", filename_hash_find("abc"));
    printf("\tfind 123: %s\n", filename_hash_find("123"));
    
    exit(EXIT_SUCCESS);
}

void
filename_hash_init()
{
    int idx;
    
    for(idx = 0; idx < HASH_SIZE; idx++) {
        hash_table[idx] = NULL;
    }
}

static int
hash_filename(
    char* filename
    )
{
    int idx = 0;
    
    while(idx < strlen(filename)) {
        if('0' <= *(filename + idx) && *(filename + idx) <= '9') {
            return *(filename + idx) - '0';
        } else if ('A' <= *(filename + idx) && *(filename + idx) <= 'Z') {
            return *(filename + idx) - 'A' + 10;
        } else if ('a' <= *(filename + idx) && *(filename + idx) <= 'z') {
            return *(filename + idx) - 'a' + 36;
        } else {
            idx++;
        }
    }
    
    return 0;
}

char*
filename_hash_find(
    char* filename
    )
{
    int idx = hash_filename(filename);
    p_filename_hash_node pnode = hash_table[idx];
    
    while(pnode != NULL) {
        if(strcmp(pnode -> filename, filename) == 0) {
            return filename;
        } else {
            pnode = pnode -> next;
        }
    }
    
    return NULL;
}

void
filename_hash_insert(
    char* filename
    )
{
    int idx; 
    p_filename_hash_node pnode;
    p_filename_hash_node nnode;
    
    if(filename_hash_find(filename) != NULL) {
        return;
    }
    
    idx = hash_filename(filename);
    pnode = hash_table[idx];
    nnode = (p_filename_hash_node)malloc(sizeof(filename_hash_node));
    strcpy(nnode->filename, filename);
    nnode->next = pnode;
    hash_table[idx] = nnode;
}

void
filename_hash_remove(
    char* filename
    )
{
    int idx = hash_filename(filename);
    p_filename_hash_node pnode = hash_table[idx];
    p_filename_hash_node pnode_pre = pnode;
    
    while(pnode != NULL) {
        if(strcmp(pnode -> filename, filename) == 0) {
            //if(pnode -> next == NULL) {
            //    if(pnode == hash_table[idx]) {
            //        hash_table[idx] = NULL;
            //    } else {
            //        pnode_pre -> next = NULL;
            //    }
            //} else {
            //    if(pnode == hash_table[idx]) {
            //        hash_table[idx] = pnode -> next;
            //    } else {
            //        pnode_pre -> next = pnode -> next;
            //    }
            //}
            if(pnode == hash_table[idx]) {
                hash_table[idx] = pnode -> next;
            } else {
                pnode_pre -> next = pnode -> next;
            }
            
            free(pnode);
            pnode = NULL;
        } else {
            if(pnode != hash_table[idx]) {
                pnode_pre = pnode;
            }
            
            pnode = pnode -> next;
        }
    }
}