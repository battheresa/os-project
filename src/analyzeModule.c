#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utility.c"

FILE *raw_file;
char buf_read[ORD_LENGTH];
char buf_write[ORD_LENGTH];

int main(int argc, char *arvg[]) {
    char algorithm[] = argv[1];
    int total = atoi(argv[2]);
    
    char *data[ORD_LENGTH];
    
    for (int i = 0; i < total * 4; i++)
        data[i] = malloc(sizeof(char) * ORD_LENGTH);
    
    for (int i = 0; true; i++) {
        fgets(buf_read, ORD_LENGTH, stdin)  // read inputs
        
        if (buf_read == NULL)
            break;
        
        buf_read[strlen(buf_read) - 1] = 0; // add EOF
        printf("buf_read: %s\n", buf_read);
        
        memcpy(data[i], &buf_read[0], strlen(buf_read));    // save to data string array
        printf("data[%d]: %s\n", i, data[i]);
    }
    
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
            
            if (i == 0) {   // write to txt file with some titles
                
            }
            else if (i == 1) {  // calculate total accepted and rejected orders
                
            }
            else {  // calculate performance 
                    
            }
            
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
            exit(0);
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
    
    return 0;
}
