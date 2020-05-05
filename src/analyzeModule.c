#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

FILE *report_file;

int main(int argc, char *arvg[]) {
    char line[ORD_LENGTH];
    
    int fd_P[ALGORITHM][2];     // pipe from parent to child (another program)
    int fd_C[ALGORITHM][2];     // pipe from child to parent
    
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
            
            if (i == 0) {   // format schedule as raw data
                while (fgets(line, ORD_LENGTH, stdin) != NULL) {   // read until EOF
                    // extract schedule part
                    // format schedule into huge string
                    // write back to parent
                }
            }
            else if (i == 1) {  // arrange accepted and rejected request as raw data
                while (fgets(line, ORD_LENGTH, stdin) != NULL) {   // read until EOF
                    // extract analysis part
                    // format analysis into huge string
                    // write back to parent
                }
            }
            else {  // calculate performance
                while (fgets(line, ORD_LENGTH, stdin) != NULL) {   // read until EOF
                    // extract performance part
                    // format performance into huge string
                    // write back to parent
                }
            }
            
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
            exit(0);
        }
        // parent process
        else if (pid > 0) {
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
        }
    }
    
    
    // parent read from each child
    // write to txt as raw data
    
    
    for (int i = 0; i < ALGORITHM; i++) {
        close(fd_P[i][WRITE]);  // close write from parent
        close(fd_C[i][READ]);   // close read from child
        cid = wait(NULL);
        printf("Parent: child %d is collected\n", cid);
    }
    
    return 0;
}
