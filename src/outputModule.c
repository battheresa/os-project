#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

FILE *report_file;

void formatSchedule(char data[], char formated[]) {
    // save to array of string
    // write back line by line
}

void formatAnalysis(char data[], char formated[]) {
    // save to array of string
    // write back line by line
}

void formatPerformance(char data[], char formated[]) {
    // save to array of string
    // write back line by line
}

void printREPORT(char filename[]) {
    report_file = fopen(filename, "w");    // open write file
    
    if (report_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    // write to file
    //fprintf(report_file, "%d %s\n", time, data)
    
    fclose(report_file);
}

