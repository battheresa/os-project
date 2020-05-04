#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define ALGORITHM 2

char chosen[MIN_LENGTH];

void runPLS() {
    int fd_P[ALGORITHM][2];    // pipe from parent to child
    int fd_C[ALGORITHM][2];    // pipe from child to parent
    
    int pid, cid, parent_id;
    parent_id = getpid();
    
    for (int i = 0; i < ALGORITHM; i++) {
        // create pipe from parent to child
        if (pipe(fd_P[i]) < 0) {
            printf("Pipe from parent to child Error\n");
            exit(1);
        }

        // create pipe from child to parent
        if (pipe(fd_C[i]) < 0) {
            printf("Pipe from child to parent Error\n");
            exit(1);
        }
        
        // fork a child
        pid = fork();
        
        // error
        if (pid < 0) {
            printf("Fork failed\n");
            exit(1);
        }
        // child process
        else if (pid == 0) {
            close(fd_P[i][WRITE]);  // close write from parent
            close(fd_C[i][READ]);   // close read from child
            
            if (i == 0) {
                // call schedule EDD
            }
            else {
                // call schedule SJF
            }
            
            
            
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
        }
        // parent process
        else if (pid > 0) {
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
            
            // do stuff
            
            close(fd_P[i][WRITE]);  // close write from parent
            close(fd_C[i][READ]);   // close read from child
            cid = wait(NULL);
            printf("Parent: child %d is collected\n", cid);
        }
    }
}
