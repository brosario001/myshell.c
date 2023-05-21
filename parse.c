#include "parse.h"
#include "read.h"

#define BUF_SIZE 16

char *wildcard(char *args){
    char *directories[] = {
        "/usr/local/sbin",
        "/usr/local/bin",
        "/usr/sbin",
        "/usr/bin",
        "/sbin",
        "/bin",
        "."
    };

    int match_flag=0;

    char *front_half = malloc(sizeof(args)/2 + 1);
    char *back_half = malloc(sizeof(args)/2 + 1 );

    int args_position=0;
    int front_half_position=0;
    int back_half_position=0;

    while(args[args_position]!='*'){ 
        front_half[front_half_position]=args[args_position];
        front_half_position++;
        args_position++;
    }

    args_position++; //+1 to skip over *

    while(args[args_position]!=NULL){
        back_half[back_half_position]=args[args_position];
        back_half_position++;
        args_position++;
    }
    
    //loop through list of directories to check
    for(int d=0;d<7;d++){

        DIR *directory;
        struct dirent *file;
        directory = opendir(directories[d]);

        if(directory){
            while ((file = readdir(directory)) != NULL && match_flag==0) {
                
                char *tmp_front_compare = malloc(sizeof(char) * (front_half_position) );
                char *tmp_back_compare = malloc(sizeof(char) * (back_half_position) );

                if(sizeof(tmp_back_compare)>sizeof(char *) || sizeof(tmp_front_compare)>sizeof(char *)){
                    free(tmp_back_compare);
                    free(tmp_front_compare);
                }

                for(int i=0; i<front_half_position;i++){    
                    tmp_front_compare[i] = file->d_name[i];
                }

                for(int i=0; i<back_half_position;i++){
                    if(d==6){
                        tmp_back_compare[i] = file->d_name[(file->d_namlen)-(back_half_position)+i-2];
                    } else{
                        tmp_back_compare[i] = file->d_name[(file->d_namlen)-(back_half_position)+i];
                    } 
                }
                
                if(strcmp(tmp_front_compare,front_half)==0 && strcmp(tmp_back_compare, back_half)==0){
                    printf("%s\n",file->d_name);
                    match_flag=1;
                    return file->d_name;
                }
                free(tmp_front_compare);
                free(tmp_back_compare);
                
            }
            closedir(directory);
        }
    }
    free(front_half);
    free(back_half);
}

char **parse_line(char *line){

    int position_line=0;
    int position_tokens=0;
    int tokens_buf_size = BUF_SIZE;
    int wildcard_flag;
    int j;
    
    char **tokens = malloc(sizeof(char*)*tokens_buf_size);

    while(line[position_line]!='\0'){
        j=0;
        wildcard_flag=0;
        char *token=calloc(1,getSize());
        
        //if white space, move to next position
        if(line[position_line]==' '){
            position_line++;
        }
        //while the current car is not a white space, nor a terminantor, add that to token
        while((line[position_line]!=' ') && (line[position_line]!='\0')){

            //check if wild card
            if(line[position_line]=='*'){
                wildcard_flag=1;
            }

            token[j]=line[position_line];
            position_line++;
            j++;
        }
        //if wild card was detected pass to wildcard()
        if(wildcard_flag==1){ 
            token = wildcard(token);
        }

        //add token to tokens[];
        tokens[position_tokens] = token;
        position_tokens++;

        //increase buffer size in case of overflow
        if(position_tokens>BUF_SIZE){
            tokens_buf_size += BUF_SIZE;
            realloc(tokens, tokens_buf_size);
        }
        token = NULL;
        free(token);
    }
    return tokens;
}