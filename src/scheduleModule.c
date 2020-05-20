#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//#include "scheduleUtility.c"
//#include "scheduleEDD.c"
//#include "scheduleSJF.c"

#define READ 0
#define WRITE 1
#define ALGORITHM 2

char buf_read[ORD_LENGTH];
char buf_write[ORD_LENGTH];


void convertSchedule(Order ord[], char indicator, int pipe) {
    //for (int i = 0; i < length; i++) {
    int i = 0;
    while (ord[i] != 0) {   // check EOF (?)
        if (ord[i].quantity == -1) {
            sprintf(buf_write, "NA");
            write(pipe, buf_write, ORD_LENGTH); // write heading to parent
            continue;
        }
        
        sprintf(buf_write, "%c %s %s %d %s", indicator, ord[i].order_number, ord[i].product_name, ord[i].quantity, ord[i].due_date);
        write(pipe, buf_write, ORD_LENGTH); // write heading to parent
        i++;
    }
}


void convertAccpeted(Order ord[], char indicator, int pipe) {
    Date arr, fin;
    int duration;
    char arrival[SUB_LENGTH], finish[SUB_LENGTH];
    
    //for (int i = 0; i < length; i++) {
    int i = 0;
    while (ord[i] != 0) {   // check EOF (?)
        arr = daysToDate(ord[i].arrival_date);
        fin = daysToDate(ord[i].finish_date);
        duration = dateToDays(arr, fin);
        
        dateToString(arr, arrival);
        dateToString(fin, finish);
        
        sprintf(buf_write, "%c %s %s %d %s %s %d", indicator, ord[i].order_number, ord[i].product_name, ord[i].quantity, arrival, finish, duration);
        write(pipe, buf_write, ORD_LENGTH); // write heading to parent
        i++;
    }
}


void convertRejected(Order ord[], char indicator, int pipe) {
    //for (int i = 0; i < length; i++) {
    int i = 0;
    while (ord[i] != 0) {   // check EOF (?)
        sprintf(buf_write, "%c %s %s %d %s", indicator, ord[i].order_number, ord[i].product_name, ord[i].quantity, ord[i].due_date);
        write(pipe, buf_write, ORD_LENGTH); // write heading to parent
        i++;
    }
}


void writeSchedule(char *data[]) {
    int i = num_plants + 1;
    while (data[i] != 0)    // check EOF (?)
        fprintf(raw_file, "%s\n", data[i++]);
}


void writeRaw(char algthm[], char *data_edd[], char *data_sjf[]) {
    FILE *raw_file = fopen(raw_path, "w");
    
    // write algorithm name for title of the report
    fprintf(raw_file, "%s\n", algthm);
    
    // write the period
    char start[SUB_LENGTH], end[SUB_LENGTH];
    start = dateToString(start_period);
    end = dateToString(end_period);
    fprintf(raw_file, "from %s to %s\n", start, end);
    
    // write the limit of each plant
    for (int i = 0; i < num_plants; i++)
        sprintf(buf_write, "%d ", plants_limit[i]);
    fprintf(raw_file, "%s\n", buf_write);
    
    // write performance of EDD
    for (int i = 0; i < num_plants; i++)
        fprintf(raw_file, "%s\n", data_edd[i]);
    
    // write performance of SJF
    for (int i = 0; i < num_plants; i++)
        fprintf(raw_file, "%s\n", data_sjf[i]);
    
    // write schedule of the chosen algorithm
    if (strcmp(algthm, "EDD") == 0)
        writeSchedule(data_edd);
    else
        writeSchedule(data_sjf);
    
    // close file
    close(raw_file);
}


void runPLS(char algthm[]) {
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
            
            // write performance of each plant to parent
            int numerator = 0, denominator = 0;
            int total_days = dateToDays(start_period, end_period);
            
            for (int i = 0; i < num_plants; i++) {
                numerator += plants_produced[i];
                denominator += plants_limit[i];
                write(fd_C[i][WRITE], "%c %d %d %.2f", plants_code[i], plants_days_use[i], plants_produced[i], (float) plants_produced[i] / (float) (total_days * plants_limit[i]));
            }
            
            // write overall performance to parent
            write(fd_C[i][WRITE], "%.2f", (float) numerator / (float) denominator);
            
            // write schedule
            if ((i == 0 && strcmp(algthm, "EDD") == 0) || (i == 1 && strcmp(algthm, "SJF") == 0))) {
                convertSchedule(plant_X, plants_code[0], 100, fd_C[i][WRITE]);
                convertSchedule(plant_Y, plants_code[1], 100, fd_C[i][WRITE]);
                convertSchedule(plant_Z, plants_code[2], 100, fd_C[i][WRITE]);
                convertAccpeted(finished, 'A', num_finished, fd_C[i][WRITE]);
                convertRejected(unfinished, 'R', num_unfinidhed, fd_C[i][WRITE]);
            }
            
            write(fd_C[i][WRITE], "done", ORD_LENGTH);  // write 'done' to parent
            
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
            exit(0);
        }
        // parent process
        else if (pid > 0) {
            close(fd_P[i][READ]);   // close read from parent
            close(fd_C[i][WRITE]);  // close wtire from child
            
            char *data_edd[ORD_LENGTH], *data_sjf[ORD_LENGTH];
            for (int i = 0; i < 100; i++) {
                data_edd[i] = malloc(sizeof(char) * ORD_LENGTH);
                data_sjf[i] = malloc(sizeof(char) * ORD_LENGTH);
            }
            
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
            
            writeRaw(data_edd, data_sjf);
            
            close(fd_P[i][WRITE]);  // close write from parent
            close(fd_C[i][READ]);   // close read from child
            cid = wait(NULL);
            printf("Parent: child %d is collected\n", cid);
        }
    }
}
