#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "edd.C"
//#include "sjf2.c" // have main

#define READ 0
#define WRITE 1
#define ALGORITHM 2

void orderToString(char plant, Order ord, char str[]) {
    char arrival[SUB_LENGTH];
    dateToString(daysToDate(ord.arrival_date), arrival);
    
    sprintf(str, "%c %s %s %d %s %d %s", plant, ord.order_number, ord.product_name, ord.quantity, arrival, ord.finish_date, ord.due_date);
}

void runPLS(char algthm[]) {
    int total = totalOrders();
    
    char *plant_X[total];
    char *plant_Y[total];
    char *plant_Z[total];
    char *rejected[total];
    
    for (int i = 0; i < total; i++) {
        plant_X[i] = malloc(sizeof(char) * ORD_LENGTH);
        plant_Y[i] = malloc(sizeof(char) * ORD_LENGTH);
        plant_Z[i] = malloc(sizeof(char) * ORD_LENGTH);
        rejected[i] = malloc(sizeof(char) * ORD_LENGTH);
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
            
            if (i == 0) {
                // call schedule EDD
                // save to array of string
                // write string to parent
            }
            else {
                // call schedule SJF
                // save to array of Order
                // write string to parent
            }
            
            // write 'done' to parent
            
            // exec format report
            // analyze module write raw data file
            
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
            exit(0);
        }
        // parent process
        else if (pid > 0) {
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
            
            // read string order from child
            
            // read 'done' from child
            // write string to child (another program)
            
            close(fd_P[i][WRITE]);  // close write from parent
            close(fd_C[i][READ]);   // close read from child
            cid = wait(NULL);
            printf("Parent: child %d is collected\n", cid);
        }
    }
}
