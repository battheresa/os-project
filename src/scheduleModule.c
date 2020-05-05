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

int total = 10;
char buf_read[ORD_LENGTH];
char buf_write[ORD_LENGTH];

void orderToString(char plant, Order ord, char str[]) {
    char arrival[SUB_LENGTH];
    dateToString(daysToDate(ord.arrival_date), arrival);
    
    sprintf(str, "%c %s %s %d %s %d %s", plant, ord.order_number, ord.product_name, ord.quantity, arrival, ord.finish_date, ord.due_date);
}

void convertAndWrite(Order array[], char indicator, int pipe) {
    for (int i = 0; i < total; i++) {
        orderToString(indicator, array[i], buf_write);   // save orders to array of string
        write(pipe, buf_write, ORD_LENGTH); // write string to parent
    }
}

void runPLS(char algthm[]) {
    char *data_edd[ORD_LENGTH];
    char *data_sjf[ORD_LENGTH];
    
    for (int i = 0; i < total * 4; i++) {
        data_edd[i] = malloc(sizeof(char) * ORD_LENGTH);
        data_sjf[i] = malloc(sizeof(char) * ORD_LENGTH);
    }
    
    int fd_P[ALGORITHM][2];     // pipe from parent to child (another program)
    int fd_C[ALGORITHM][2];     // pipe from child to parent
    
    int pid, cid, n;
    
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
                // TODO: run schedule EDD
            }
            else {
                // TODO: run schedule SJF
            }
            
            /*
            convertAndWrite(plant_X, 'X', fd_C[i][WRITE]);      // write plant_X schedule to plant
            convertAndWrite(plant_Y, 'Y', fd_C[i][WRITE]);      // write plant_Y schedule to plant
            convertAndWrite(plant_Z, 'Z', fd_C[i][WRITE]);      // write plant_Z schedule to plant
            convertAndWrite(unfinished, 'R', fd_C[i][WRITE]);   // write rejected request to parent
            */
            
            write(fd_C[i][WRITE], "done", ORD_LENGTH);  // write 'done' to parent
            
            // TODO: exec analyze module (analyze module write raw data file)
            
            if (i == 0) {
                // TODO: exec analyze module on EDD
                // argv = {algorithm (edd / sjf), total_order}
            }
            else {
                // TODO: exec analyze module on SJF
                // argv = {algorithm (edd / sjf), total_order}
            }
            
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
            exit(0);
        }
        // parent process
        else if (pid > 0) {
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
            
            for (int j = 0; true; j++) {
                n = read(fd_C[i][READ], buf_read, ORD_LENGTH);  // read from child
                buf_read[n] = 0;    // add EOF
                
                if (strcmp(buf_read, "done") == 0)  // read 'done' from child
                    break;
                
                // save to data_edd/data_sjf array
                if (i == 0)
                    memcpy(data_edd[j], &buf_read[0], ORD_LENGTH);
                else
                    memcpy(data_sjf[j], &buf_read[0], ORD_LENGTH);
            }
            
            // TODO: write string to child (another program)
            
            close(fd_P[i][WRITE]);  // close write from parent
            close(fd_C[i][READ]);   // close read from child
            cid = wait(NULL);
            printf("Parent: child %d is collected\n", cid);
        }
    }
}
