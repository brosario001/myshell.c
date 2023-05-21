#include "read.h"

#define BUF_SIZE 1000

int line_size=0;

char* read_line(){
    int buffer_size = BUF_SIZE;
    char *line = malloc(sizeof(char)*buffer_size);
    int i=0;
    int c;

    while(1){
        c = getchar();
        if(c == EOF || c=='\n'){
            line[i] = '\0';
            line_size = i;
            return line;
        } else{
            line[i] = c;
        }
        i++;

        if(i>buffer_size){
            buffer_size += BUF_SIZE;
            line = realloc(line, buffer_size);
        }
    }
}

int getSize(){
    return line_size;
}