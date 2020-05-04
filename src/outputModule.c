#include <stdio.h>
#include <stdlib.h>

FILE *report_file;

void formatSchedule(char data[], char formated[]) {
    
}

void formatAnalysis(char data[], char formated[]) {
    
}

void formatPerformance(char data[], char formated[]) {
    
}

// data[] = array of Orders according to the the schedule
void printREPORT(char filename[], char data[]) {
    report_file = fopen(filename, "a");    // open write file
    
    if (report_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    // fork 3 child
    // for ... {
    // one format schedule
    // one format analysis
    // one format performance
    // }
    
    // for ... {
    // parent read from child one by one
    // parent write to report_file
    // }
    
    // for ... {
    // close pipes
    // collect child
    // }
    
    //fprintf(report_file, "%d %s\n", time, data);
    fclose(report_file);
}

