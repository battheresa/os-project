#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char buf_read[100];
    
    int fd_P[3][2];
    int fd_C[3][2];
    
    int pid, cid;
    
    for (int i = 0; i < 3; i++) {
        pipe(fd_P[i]);
        pipe(fd_C[i]);
        
        pid = fork();
        
        // error
        if (pid < 0) {
            printf("fork error\n");
            exit(1);
        }
        // child process
        else if (pid == 0) {
            close(fd_P[i][1]);
            close(fd_C[i][0]);
            
            dup2(fd_P[i][0], STDIN_FILENO);
            close(fd_P[i][0]);
            
            dup2(fd_C[i][1], STDOUT_FILENO);
            close(fd_C[i][1]);
            
            execl("./sub", NULL);
            exit(127); // if execv fails
        }
        // parent process
        else {
            close(fd_P[i][0]);
            close(fd_C[i][1]);
            
            if (i == 0)
                write(fd_P[i][1], "Hello World!", 100);
            else if (i == 1)
                write(fd_P[i][1], "Hola!", 100);
            else if (i == 2)
                write(fd_P[i][1], "Ni hao!", 100);
            
            int n = read(fd_C[i][0], buf_read, 100);
            buf_read[n] = 0;
            
            printf("app: %s\n", buf_read);
            
            close(fd_P[i][1]);
            close(fd_C[i][0]);
            
            cid = wait(NULL);
            printf("child %d collected\n", cid);
        }
    }
    
    return 0;
}

