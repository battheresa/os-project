#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

char path[] = "_reports";
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
    strcat(path, filename);
    report_file = fopen(path, "w");    // open write file
    
    if (report_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    // read from raw data file
    // write to file
    //fprintf(report_file, "%d %s\n", time, data)
    
    fclose(report_file);
}

/*
========================================================================================== (90)

Plant X (300 per day)
[period range]

Date            Product Name        Order Number        Quantity        Due Date
YYYY-MM-DD      Product_A           P_XXX               XXXX            YYYY-MM-DD

------------------------------------------------------------------------------------------ (90)

Plant Y (400 per day)
[period range]
 
...
 
========================================================================================== (90)
  
Analysis Report
       
------------------------------------------------------------------------------------------ (90)
  
Algorithm: EDD (Earliest Due Date)
 
Order Accepted (total of xxx orders):

Product Name        Quantity        Start           Finish          Duration        Plant
P_XXX               XXXX            YYYY-MM-DD      YYYY-MM-DD      XXXX            XXXX

Order Rejected (total of xxx orders):
  
Product Name        Order Number    Quantity        Due Date
P_XXX               P_XXX           XXXX            YYYY-MM-DD
  
------------------------------------------------------------------------------------------ (90)
  
Algorithm: SJF (Shortest Job First)

...

========================================================================================== (90)

Performance Report
 
Algorithm: EDD (Earliest Due Date)

Plant_X:    Number of days in use               XXX days
            Number of products produced         XXX (in total)
            Utilization of the plant            XX.XX %

Overal utilization                              XX.XX %
 
------------------------------------------------------------------------------------------ (90)

Algorithm: SJF (Shortest Job First)
 
...
 
========================================================================================== (90)
*/
