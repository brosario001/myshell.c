#include "IOredirect.h"

void myshell_IOredirect(char **args, int k, int ioMode){
    pid_t pid, wpid;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd, status = 0;
    
    pid = fork();
    if (pid == 0) {
        // Child process
        if(ioMode == 0)   // Input mode
            fd = open(args[k+1], O_RDONLY, mode);
        else              // Output mode
            fd = open(args[k+1], O_WRONLY|O_CREAT|O_TRUNC, mode);
        if(fd < 0)
            fprintf(stderr, "File error");
        else {
        
            dup2(fd, ioMode);   // Redirect input or output according to ioMode
            close(fd);          // Close the corresponding pointer so child process can use it
            args[k] = NULL;
            args[k+1] = NULL;

            if (execvp(args[0], args) == -1) {
                perror("!mysh");
            }
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0) { // Error forking process
        perror("!mysh");
    } 
    else {
        // Parent process. Wait till it finishes execution
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } 
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }        
}
