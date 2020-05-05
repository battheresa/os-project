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

void runPLS(char algthm[], bool create_report) {
    if (!create_report) {
        memcpy(chosen, algo, sizeof(algthm));
        exit(0);
    }
    
    char *plant_X[ORD_LENGTH];
    char *plant_Y[ORD_LENGTH];
    char *plant_Z[ORD_LENGTH];
    char *rejected[ORD_LENGTH];
    
    // malloc
    
    int fd[2];
    int fd_P[ALGORITHM][2];    // pipe from parent to child
    int fd_C[ALGORITHM][2];    // pipe from child to parent
    
    int pid, cid;
    
    for (int i = 0; i < ALGORITHM; i++) {
        // create pipe from parent to child
        if (pipe(fd_P[i]) < 0) {
            printf("Pipe from parent to child error\n");
            exit(1);
        }

        // create pipe from child to parent
        if (pipe(fd_C[i]) < 0) {
            printf("Pipe from child to parent error\n");
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
                // save to array of string
            }
            else {
                // call schedule SJF
                // save to array of string
            }
            
            // write all array of string to parent
            
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
        }
        // parent process
        else if (pid > 0) {
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
        }
    }
    
    // read from EDD
    // read from SJF
    
    // write to outputModule.c as single huge string (piping between 2 files)
    
    // clsoe pipes and collect child
    for (int i = 0; i < ALGORITHM; i++) {
        close(fd_P[i][WRITE]);  // close write from parent
        close(fd_C[i][READ]);   // close read from child
        cid = wait(NULL);
        printf("Parent: child %d is collected\n", cid);
    }
}
