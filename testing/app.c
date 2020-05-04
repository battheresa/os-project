#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    
    int pid, cid;
    
    for (int i = 0; i < 2; i++) {
        pid = fork();
        
        // error
        if (pid < 0) {
            printf("fork error\n");
            exit(1);
        }
        // child process
        else if (pid == 0) {
            if (i == 0) {
                printf("child %d run gcc\n", i);
                char *command[]={"gcc", "-o", "sub", "sub.c", NULL};
                execvp("gcc", command);
            }
            else if (i == 1) {
                printf("child %d run sub.c\n", i);
                char *input[]={"./sub", "testing", NULL};
                execvp("./sub", input);
            }
            
            exit(127); // if execv fails
        }
        // parent process
        else {
            cid = wait(NULL);
            printf("child %d collected\n", cid);
        }
    }
    
    return 0;
}

