/*
TO-DO
    -> work on INTERACTIVE mode and BATCH mode
        -> INTERACTIVE mode is currently working, but i still have to make BATCH mode
    ->wild cards 
    ->extensions 
        -> HOME
            -> when typing cd without a command, the HOME directory is set (DONE)
            -> if arg contains ~before a slash, replace with HOME directory
        ->directory wild cards
*/

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "read.h"
#include "parse.h"
#include "execute.h"
#include "IOredirect.h"
#include "piper.h"

//builtins
int check_builtins(char**args){
    if(strcmp(args[0],"cd")==0){
        if(args[1]==NULL){
            chdir(getenv("HOME"));
        }
        if(chdir(args[1])!=0){
            perror("!mysh");
        }
    } else if(strcmp(args[0],"pwd")==0){

        char *cwd = malloc(sizeof(char)*1024);
        getcwd(cwd,sizeof(cwd));
        printf("%s\n",cwd);
        return 1;
    } else if (strcmp(args[0],"exit")==0) {
        printf("mysh: exiting \n");
        exit(0);
    }
    return 0;
}

int file_exists(char *arg){
    struct stat buffer;
    return stat(arg, &buffer);
}

//search directories for command
char *search_cmd_subdir(char **args){
    char *directories[] = {
        "/usr/local/sbin",
        "/usr/local/bin",
        "/usr/sbin",
        "/usr/bin",
        "/sbin",
        "/bin",
    };

    for(int x=0; x<6;x++){
        char *executable = malloc(sizeof(char *)*1024);

        strcpy(executable, directories[x]);
        strcat(executable,"/"); 
                
        if(file_exists(strcat(executable, args[0]))==0){
            return executable;
        }
        executable = NULL;
        free(executable);
    }
    return 0;
}

int main(){
    printf("Welcome to mysh!\n");
    char *options[4] = {"<",">","|","&"};

    //main loop
    while(1){
        int j,k,i = 0;
        printf("mysh> ");
        char *line = read_line();
        char **tokens = parse_line(line);

        //check (or) search for builtin commands
        if(check_builtins(tokens) == 1){
            j=1;
        } else if(check_builtins(tokens)==0){
            tokens[0] = search_cmd_subdir(tokens);
        }

        //check for redireciton (or) pipes
        while(tokens[i]!=NULL){
            for(k = 0; k < 4; k++) {
                if(strcmp(tokens[i],options[k]) == 0){
                    break;
                }
            }
            if(k < 4){
                j = 1;
                if(k < 3 && tokens[i+1] == NULL) {   
                    fprintf(stderr, "!mysh: parameter missing\n"); 
                    break;
                } if(k < 2){                         
                    printf("redirecting\n"); //debugging line
                    myshell_IOredirect(tokens, i, k);
                } else if(k == 2){
                    printf("piping\n"); //debugging line
                    myshell_pipe(tokens, i); 
                } else if(k == 3){
                    execute(tokens, i); 
                } break;
            } i++;
        }
        //if no redirect or pipes, execute command
        if(j == 0){
            //printf("executing\n"); //debugging line
            execute(tokens, 0);
        }
        free(tokens);
        free(line);
    }
    return 0;
}



