
#include "execute.h"

void execute(char **args, int i){
    
    pid_t pid, wpid;
    int status;

    if(i > 0){
        args[i] = NULL; // Set the location of '&' to NULL in order to pass it to execvp()
    }

    pid = fork();

    if(pid == 0) // fork success. child initiated          
    {
        execv(args[0], args);
        perror("!mysh");
        if(i == 0)
            exit(1);
    }

    if(i == 0) {
        // If not a background process, wait till it finishes execution.
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } 
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    else {
        // For a background process, do not wait indefinitely, but return to prompt
        fprintf(stderr, "child process...");
        wpid = waitpid(-1, &status, WNOHANG);
    }
}
