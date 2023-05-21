#include "piper.h"

void myshell_pipe(char **args, int i){
    int fpipe[2];
    char **args2 = malloc(sizeof(char *) * (i+1));

    for(int k=0;k<i;k++){
        args2[k]=strdup(args[k]);
    }

    if(pipe(fpipe) == -1) {  // Create a pipe with one input pointer and one output pointer
        perror("PIPE REDIRECT FAILED");
        return;
    }

    if(fork() == 0)   // child 1        
    {
        dup2(fpipe[1], STDOUT_FILENO);   // Redirect STDOUT to output part of pipe     
        close(fpipe[0]);      //close pipe read
        close(fpipe[1]);      //close write pipe

        execvp(args2[0], args2);    // pass the truncated command line as argument
        perror("CHILD_1 EXEC FAILED");
        exit(1);
    }

    if(fork() == 0)   // child 2
    {
        dup2(fpipe[0], STDIN_FILENO);   // Redirect STDIN to Input part of pipe         
        close(fpipe[1]);       //closing pipe write
        close(fpipe[0]);       //close read pipe 

        printf("program: %s\n", args[i+1]);
        execvp(args[i+1], args+i+1);    // pass the second part of command line as argument
        perror("CHILD_2 EXEC FAILED");
        exit(1);
    }

    close(fpipe[0]);
    close(fpipe[1]);
    wait(0);   // Wait for child 1 to finish
    wait(0);   // Wait for child 2
}