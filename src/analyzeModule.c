#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utility.c"

char edd_file[] = "_resources/rawEDD.txt";
char sjf_file[] = "_resources/rawSJF.txt";

FILE *raw_file;

char buf_read[ORD_LENGTH];
char buf_write[ORD_LENGTH];

char *schedule[ORD_LENGTH];
char *accepted[ORD_LENGTH];
char *rejected[ORD_LENGTH];

int num_schd = 0;
int num_rejt = 0;
int num_acct = 0;

int main(int argc, char *arvg[]) {
    char algorithm[] = argv[1];
    int total = atoi(argv[2]);
    int num_plants = atoi(argv[3]);
    
    int per_days[num_plants];
    
    for (int i = 0; i < num_plants; i++)
        per_days[i] = atoi(argv[4 + i]);
    
    for (int i = 0; i < total * 4; i++) {
        schedule[i] = malloc(sizeof(char) * ORD_LENGTH);
        accepted[i] = malloc(sizeof(char) * ORD_LENGTH);
        rejected[i] = malloc(sizeof(char) * ORD_LENGTH);
    }
    
    while (fgets(buf_read, ORD_LENGTH, stdin) != NULL) {
        buf_read[strlen(buf_read) - 1] = 0; // add EOF
        printf("buf_read: %s\n", buf_read);
        
        if (buf_read[0] == 'A') {
            memcpy(accepted[num_acct], &buf_read[0], strlen(buf_read));    // save to rejected string array
            printf("accepted[%d]: %s\n", num_acct, accepted[num_acct++]);
        }
        else if (buf_read[0] == 'R') {
            memcpy(rejected[num_rejt], &buf_read[0], strlen(buf_read));    // save to rejected string array
            printf("rejected[%d]: %s\n", num_rejt, rejected[num_rejt++]);
        }
        else {
            memcpy(schedule[num_schd], &buf_read[0], strlen(buf_read));    // save to schedule string array
            printf("schedule[%d]: %s\n", num_schd, schedule[num_schd++]);
        }
    }
    
    memcpy(schedule[num_schd++], "E", 2);   // indicate ending of the array
    memcpy(accepted[num_acct++], "E", 2);
    memcpy(rejected[num_rejt++], "E", 2);
    
    int fd_P[ALGORITHM][2];     // pipe from parent to child
    int fd_C[ALGORITHM][2];     // pipe from child to parent
    
    int pid, cid;
    
    for (int i = 0; i < 3; i++) {
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
            
            if (i == 0) {   // write schedule of the 3 plants to raw data file
                char current = 'N';
                int line = 0;
                int count = 0;
                
                while (line < num_schd) {
                    if (schedule[line][0] == current) {    // if first char in schedule is the same plant
                        substring(schedule[line], buf_write, 2, strlen(schedule[line]));  // remove plant
                        write(fd_C[i][WRITE], buf_write, ORD_LENGTH);
                        line++;
                    }
                    else {
                        current = schedule[line][0];
                        sprintf(buf_write, "-Plant_%c (%d per day)\n", current, per_days[count++]); // print title
                        write(fd_C[i][WRITE], buf_write, ORD_LENGTH);
                    }
                }
                
                // write "done" to parents
                write(fd_C[i][WRITE], "done", ORD_LENGTH);
            }
            else if (i == 1) {  // calculate total accepted and rejected orders
                // write "-Accepted_Orders (number of accepted orders)"
                sprintf(buf_write, "-Accepted_Orders (%d orders)\n", num_acct);
                write(fd_C[i][WRITE], buf_write, ORD_LENGTH);
                
                for (int j = 0; j < num_rejt; j++) {
                    sprintf(buf_write, "%s\n", accepted[j]);
                    write(fd_C[i][WRITE], buf_write, ORD_LENGTH);
                }
                
                // write "-Rejected_Orders (number of rejected orders)"
                sprintf(buf_write, "-Rejected_Orders (%d orders)\n", num_rejt);
                write(fd_C[i][WRITE], buf_write, ORD_LENGTH);
                
                // write the rejected orders
                for (int j = 0; j < num_rejt; j++) {
                    sprintf(buf_write, "%s\n", rejected[j]);
                    write(fd_C[i][WRITE], buf_write, ORD_LENGTH);
                }
                
                // write "done" to parents
                write(fd_C[i][WRITE], "done", ORD_LENGTH);
            }
            else {  // calculate performance
                char temp[CMD_LENGTH];
                char current = 'N';
                int cur_plant = 0;
                int line = 0;
                int in_use, produced = 0, could_produce = 0;        // for individual plants utilization
                int total_produced = 0, total_could_produce = 0;    // for overal utilization
                    
                // write "-Performance"
                write(fd_C[i][WRITE], "-Performance\n", ORD_LENGTH);
                    
                while (line < num_schd) {
                    could_produce += per_days[cur_plant];    // get could_produced products
                    substring(schedule[line], temp, 2, strlen(schedule[line]));  // remove plant
                        
                    if (strcmp(temp, "NA") == 0)    // if no schdule for that day, skip the rest
                        continue;
                        
                    if (schedule[line][0]) == current) {    // if still the same plants, continue counting
                        // get actual produced amount
                    }
                    else {
                        current = schedule[line][0];
                        
                        // TODO: write to parent
                        // write "Plant_? [days_in_use] [product_produced] [utilisation]"
                        
                        total_produced += produced;
                        total_could_produce += could_produce;
                            
                        cur_plant++;
                        in_use = 0;
                        produced = 0;
                        could_produce = 0;
                    }
                }
                
                // write "Overall [utilisation]"
                sprintf(buf_write, "Overall %.2f\n", (float) total_produced / (float) total_could_produce);
                write(fd_C[i][WRITE], buf_write, ORD_LENGTH);
                    
                // write "done" to parents
                write(fd_C[i][WRITE], "done", ORD_LENGTH);
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
    
    raw_file = (strcmp(algorithm, "EDD") == 0) ? fopen(edd_file, "w") : fopen(sjf_file, "w");
    
    for (int i = 0; i < 3; i++) {
        while ((n = read(fd_C[i][READ], buf_read, ORD_LENGTH)) != 0) {
            buf_read[n] = 0;
            
            if (strcmp(buf_read, "done") == 0)
                break;
            
            printf("Parent received: %s\n", buf_read);
            fprintf(raw_file, "%s\n", buf_read);
        }
    }
    
    // close pipes and collect child
    for (int i = 0; i < 3; i++) {
        close(fd_P[i][WRITE]);  // close write from parent
        close(fd_C[i][READ]);   // close read from child
        cid = wait(NULL);
        printf("Parent: child %d is collected\n", cid);
    }
    
    return 0;
}
