#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "scheduleUtility.c"
#include "scheduleEDD.c"
#include "scheduleSJF.c"

#define READ 0
#define WRITE 1
#define ALGORITHM 2

char buf_read[ORD_LENGTH];
char buf_write[ORD_LENGTH];


// format schedule and write back to parent
void convertSchedule(Order ord[], char indicator, int length, int pipe) {
    for (int i = 0; i < length; i++) {
        if (ord[i].quantity == -1) {
            sprintf(buf_write, "%c NA", indicator);
            write(pipe, buf_write, ORD_LENGTH);
            continue;
        }
        
        sprintf(buf_write, "%c %s %s %d %s", indicator, ord[i].order_number, ord[i].product_name, ord[i].quantity, ord[i].due_date);
        write(pipe, buf_write, ORD_LENGTH);
        
        printf("%d = %s\n", i, buf_write);
    }
}


// format accepted orders and write back to parent
void convertAccpeted(Order ord[], char indicator, int length, int pipe) {
    Date arr, fin;
    int duration;
    char arrival[SUB_LENGTH], finish[SUB_LENGTH];
    
    for (int i = 0; i < length; i++) {
        arr = daysToDate(ord[i].arrival_date);
        fin = daysToDate(ord[i].finish_date);
        duration = dateToDays(arr, fin);
        
        dateToString(arr, arrival);
        dateToString(fin, finish);
        
        sprintf(buf_write, "%c %s %s %d %s %s %d", indicator, ord[i].order_number, ord[i].product_name, ord[i].quantity, arrival, finish, duration);
        write(pipe, buf_write, ORD_LENGTH);
    }
}


// format rejected orders and write back to parent
void convertRejected(Order ord[], char indicator, int length, int pipe) {
    for (int i = 0; i < length; i++) {
        sprintf(buf_write, "%c %s %s %d %s", indicator, ord[i].order_number, ord[i].product_name, ord[i].quantity, ord[i].due_date);
        write(pipe, buf_write, ORD_LENGTH);
    }
}


// write schedule to file
void writeData(FILE *file, char *data[], int length) {
    for (int i = plants_num + 1; i < length; i++)
        fprintf(file, "%s\n", data[i]);
}


// write to raw file
void writeRaw(char algthm[], char *data_edd[], char *data_sjf[], int length) {
    FILE *file = fopen(raw_path, "w");
    char temp[MIN_LENGTH];
    
    // write algorithm name for title of the report
    fprintf(file, "%s\n", algthm);
    
    // write the period
    char start[SUB_LENGTH], end[SUB_LENGTH];
    dateToString(start_period, start);
    dateToString(end_period, end);
    fprintf(file, "from %s to %s\n", start, end);
    
    // write the limit of each plant
    buf_write[0] = 0;
    for (int i = 0; i < plants_num; i++) {
        if (i < plants_num - 1)
            sprintf(temp, "%d ", plants_limit[i]);
        else
            sprintf(temp, "%d", plants_limit[i]);
        
        strcat(buf_write, temp);
    }
    fprintf(file, "%s\n", buf_write);
    
    // write performance of EDD
    for (int i = 0; i < plants_num + 1; i++)
        fprintf(file, "%s\n", data_edd[i]);
    
    // write performance of SJF
    for (int i = 0; i < plants_num + 1; i++)
        fprintf(file, "%s\n", data_sjf[i]);
    
    // write schedule of the chosen algorithm
    if (strcmp(algthm, "EDD") == 0)
        writeData(file, data_edd, length);
    else
        writeData(file, data_sjf, length);
    
    // close file
    fclose(file);
}


void runPLS(char algthm[]) {
    int fd_P[ALGORITHM][2];     // pipe from parent to child (another program)
    int fd_C[ALGORITHM][2];     // pipe from child to parent
    
    int pid;
    
    for (int i = 0; i < ALGORITHM; i++) {
        // create pipe from parent to child
        if (pipe(fd_P[i]) < 0 || pipe(fd_C[i]) < 0) {
            printf("Pipe from parent to child error\n");
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
            
            if (i == 0)
                runEDD();
            else
                runSJF();
            
            sprintf(buf_write, "%d", total_order);
            write(fd_C[i][WRITE], buf_write,  ORD_LENGTH);
            
            // write performance of each plant to parent
            int numerator = 0, denominator = 0, buf_int;
            
            sprintf(buf_write, "%d", total_days);
            write(fd_C[i][WRITE], buf_write,  ORD_LENGTH);
            
            while (true) {
                read(fd_P[i][READ], buf_read, ORD_LENGTH);  // read from child
                buf_read[strlen(buf_read)] = 0;    // add EOF
                                
                if (strcmp(buf_read, "exit") == 0)
                    break;
                
                buf_int = atoi(buf_read);
                
                if (buf_int < plants_num) {     // write each plant performance
                    numerator += plants_produced[buf_int];
                    denominator += plants_limit[buf_int];
                    
                    sprintf(buf_write, "%c %d %d %.2f", plants_code[buf_int], plants_days_use[buf_int], plants_produced[buf_int], (float) plants_produced[buf_int] / (float) (total_days * plants_limit[buf_int]) * 100);
                    write(fd_C[i][WRITE], buf_write,  ORD_LENGTH);
                }
                else if (buf_int == plants_num) {   // write overall performances
                    sprintf(buf_write, "%.2f", (float) numerator / (float) (total_days * denominator) * 100);
                    write(fd_C[i][WRITE], buf_write, ORD_LENGTH);
                }
                else if ((i == 0 && strcmp(algthm, "EDD") == 0) || (i == 1 && strcmp(algthm, "SJF") == 0)) {
                    convertSchedule(plantX, plants_code[0], total_days, fd_C[i][WRITE]);
                    convertSchedule(plantY, plants_code[1], total_days, fd_C[i][WRITE]);
                    convertSchedule(plantZ, plants_code[2], total_days, fd_C[i][WRITE]);
                    convertAccpeted(finished, 'A', num_finished, fd_C[i][WRITE]);
                    convertRejected(unfinished, 'R', num_unfinished, fd_C[i][WRITE]);
                }
                else {
                    write(fd_C[i][WRITE], "done", ORD_LENGTH);
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

    int total_days, lines;
    int num_edd = 0, num_sjf = 0;
    char *data_edd[ORD_LENGTH], *data_sjf[ORD_LENGTH];
    
    for (int i = 0; i < ALGORITHM; i++) {
        read(fd_C[i][READ], buf_read, ORD_LENGTH);
        buf_read[strlen(buf_read)] = 0;
        
        total_order = atoi(buf_read);
        
        read(fd_C[i][READ], buf_read, ORD_LENGTH);
        buf_read[strlen(buf_read)] = 0;
        
        total_days = atoi(buf_read);
        lines = (total_days * plants_num) + (plants_num + 1) + total_order;
        
        for (int j = 0; j < lines; j++) {
            if (i == 0)
                data_edd[j] = malloc(sizeof(char) * ORD_LENGTH);
            else
                data_sjf[j] = malloc(sizeof(char) * ORD_LENGTH);
        }
        
        for (int count = 0; count < lines; count++) {
            sprintf(buf_write, "%d", count);
            write(fd_P[i][WRITE], buf_write, ORD_LENGTH);
            
            read(fd_C[i][READ], buf_read, ORD_LENGTH);
            buf_read[strlen(buf_read)] = 0;
            
            if (strcmp(buf_read, "done") == 0)
                break;
            
            if (i == 0) {
                memcpy(data_edd[num_edd], &buf_read[0], ORD_LENGTH);
                //printf("data_edd: %s\n", data_edd[num_edd]);
                num_edd++;
            }
            else {
                memcpy(data_sjf[num_sjf], &buf_read[0], ORD_LENGTH);
                //printf("data_sjf: %s\n", data_edd[num_sjf]);
                num_sjf++;
            }
        }
        
        write(fd_P[i][WRITE], "exit", ORD_LENGTH);
    }
    
    writeRaw(algthm, data_edd, data_sjf, lines);
    
    for (int i = 0; i < ALGORITHM; i++) {
        close(fd_P[i][WRITE]);  // close write from parent
        close(fd_C[i][READ]);   // close read from child
        wait(NULL);
    }
}
