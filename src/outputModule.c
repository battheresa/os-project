#include <stdio.h>
#include <stdlib.h>

FILE *report_file;

void printREPORT(char filename[]) {
    report_file = fopen(filename, "w");    // open write file
    
    if (report_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
        
    // fork 2 child
    // for child call runPLS(algorithm)
    
    //fprintf(report_file, "%d %s\n", time, data);
    
    
    fclose(report_file);
}


/*
       Plant_X     Plant_Y     Plant_Z
0-1:   Prd_A
1-2:   Prd_B       Prd_B       Prd_B
2-3:   Prd_C       Prd_C       Prd_C
3-4:   Prd_D       Prd_D       Prd_D
4-5:   Prd_D       Prd_C       Prd_D
*/
void formatSchedule(char data[], char formated[]) {
    
}


/*
           Waiting time    Turnaround time
Prd_A      0               1
Prd_B      0               1
Prd_C      1               3
Prd_D      0               2
*/
void formatTime(char data[], char formated[]) {
    
}


/*
Plant_X = xx%
Plant_Y = xx%
Plant_Z = xx%
*/
void formatUtilisation(char data[], char formated[]) {
    
}
