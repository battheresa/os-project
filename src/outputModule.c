#include <stdio.h>
#include <stdlib.h>

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

// data[] = array of Orders according to the the schedule
void printREPORT(char filename[]) {
    report_file = fopen(filename, "a");    // open write file
    
    if (report_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    // fork 3 child {
    // one format schedule
    // one format analysis
    // one format performance
    // }
    
    // for ... {
    // parent read from child one by one
    // parent write to report_file line by line
    // }
    
    // for ... {
    // close pipes
    // collect child
    // }
    
    //fprintf(report_file, "%d %s\n", time, data);
    fclose(report_file);
}

