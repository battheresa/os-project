#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utility.c"

FILE *report_file, * raw_file;

int items = 10;
char buf_read[ORD_LENGTH];
char buf_write[ORD_LENGTH];
char thick_line[] = "\n================================================================================\n\n";
char thin_line[] = "\n--------------------------------------------------------------------------------\n\n";


// write the schedule for each plant
void writeSchedule(char *data[], char period[], int limits[], int length) {
    fprintf(report_file, "%s", thick_line);
    fprintf(report_file, "Schedule Report\n");
    fprintf(report_file, "Period from %s\n\n", period);
    
    char *temp[SUB_LENGTH];
    
    for (int i = 0; i < items; i++)
        temp[i] = malloc(sizeof(char) * SUB_LENGTH);
    
    char current = 'N';
    int plant_now = 0, line = 0;
    
    while (line < length) {
        if (data[i][0] == current) {    // if first char in schedule is the same plant
            substring(data[line], buf_write, 2, strlen(data[line]));  // remove plant
            
            // TODO: write orders
            // split order
            // format them
            
            line++;
        }
        else {
            current = data[i][0];
            fprintf(buf_write, "%s", thin_line);
            fprintf(buf_write, "Plant %c (%d per day):\n\n", current, limits[plant_now++]);
            fprintf(buf_write, "Date\t\t\tOrder Number\t\tProduct Name\t\tQuantity\t\tDue Date");
        }
    }
    
}


// write accepted and rejected orders
void writeAnalysis(char *accepted, char *rejected, int length_acct, int length_rejt) {
    fprintf(report_file, "%s", thick_line);
    fprintf(report_file, "Analysis Report\n\n");
    
    fprintf(report_file, "Orders Accepted (total of %d orders):\n\n", length_acct);
    fprintf(report_file, "Product Name\t\tQuantity\t\tStart\t\t\tFinish\t\t\tDuration\n");
    for (int i = 0; i < num_acct; i++) {
        
        // TODO: write orders
        
    }
    
    fprintf(report_file, "Orders Rejected (total of %d orders):\n\n", length_rejt);
    fprintf(report_file, "Order Number\t\tProduct Name\t\tQuantity\t\tDue Date\n");
    for (int i = 0; i < num_rejt; i++) {
        
        // TODO: write orders
        
    }
}


// write performance
void writePerformance(char *perfmn_edd[], char *perfmn_sjf[], int length) {
    fprintf(report_file, "%s", thick_line);
    fprintf(report_file, "Performance Report\n\n");
    
    fprintf(report_file, "Algorithm EDD (Earliest Due Date):\n\n");
    for (int i = 0; i < length; i++) {
        fprintf(report_file, "Plant %c", strtok(perfmn_edd[i], " "));
        fprintf(report_file, "\t\tNumber of days in use\t\t\t\t%s days\n", strtok(NULL, " "));
        fprintf(report_file, "\t\t\tNumber of products produced\t\t\t%s in total\n", strtok(NULL, " "));
        fprintf(report_file, "\t\t\tUtilization of the plant\t\t\t%s %%\n", strtok(NULL, " "));
    }
    
    fprintf(report_file, "%s", thin_line);
    fprintf(report_file, "Algorithm SJF (Earliest Due Date):\n\n");
    for (int i = 0; i < length; i++) {
        fprintf(report_file, "Plant %c", strtok(perfmn_sjf[i], " "));
        fprintf(report_file, "\t\tNumber of days in use\t\t\t\t%s days\n", strtok(NULL, " "));
        fprintf(report_file, "\t\t\tNumber of products produced\t\t\t%s in total\n", strtok(NULL, " "));
        fprintf(report_file, "\t\t\tUtilization of the plant\t\t\t%s %%\n", strtok(NULL, " "));
    }
}


//int main() {
void printREPORT(char filename[]) {
    char report_path[SUB_LENGTH] = "_reports/";
    strcat(report_path, filename);
    
    report_file = fopen(report_path, "w");
    raw_file = fopen(raw_path, "r");
    
    if (report_file == NULL || raw_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    int num_schd = 0;
    int num_acct = 0;
    int num_rejt = 0;
    
    char period[CMD_LENGTH];
    char *schedule[ORD_LENGTH];
    char *accepted[ORD_LENGTH];
    char *rejected[ORD_LENGTH];
    
    // allocate space for orders in arrays
    for (int i = 0; i < total * 4; i++) {
        schedule[i] = malloc(sizeof(char) * ORD_LENGTH);
        accepted[i] = malloc(sizeof(char) * ORD_LENGTH);
        rejected[i] = malloc(sizeof(char) * ORD_LENGTH);
    }
    
    // read first line (report title) and write to report file
    fgets(buf_read, ORD_LENGTH, raw_file);
    buf_read[strlen(buf_read) - 1] = 0;
    fprintf(report_file, "%s\n", buf_read);
    
    // read period of the schedule
    fgets(period, ORD_LENGTH, raw_file);
    period[strlen(period) - 1] = 0;
    
    // read second line (number of plants)
    fgets(buf_read, ORD_LENGTH, raw_file);
    buf_read[strlen(buf_read) - 1] = 0;
    
    int num_plants = atoi(buf_read);
    int plants_limit[num_plants];
    char *perfmn_edd[num_plants];
    char *perfmn_sjf[num_plants];
    
    // read plants per day
    fgets(buf_read, ORD_LENGTH, raw_file);
    buf_read[strlen(buf_read) - 1] = 0;
    
    char *token = strtok(buf_read, " ");
    while (token != NULL) {
        plants_limit[i] = atoi(token);
        token = strtok(NULL, " ");
    }
    
    // read performance of each plant of EDD
    for (int i = 0; i < num_plants; i++) {
        fgets(perfmn_edd[i], ORD_LENGTH, raw_file);
        perfmn_edd[i][strlen(perfmn_edd[i]) - 1] = 0;
    }
    
    // read performance of each plant of SJF
    for (int i = 0; i < num_plants; i++) {
        fgets(perfmn_sjf[i], ORD_LENGTH, raw_file);
        perfmn_sjf[i][strlen(perfmn_sjf[i]) - 1] = 0;
    }
    
    // read the rest of the file
    while (fgets(buf_read, ORD_LENGTH, raw_file) != NULL) {
        buf_read[strlen(buf_read) - 1] = 0; // add EOF
        
        if (buf_read[0] == 'A') {
            memcpy(accepted[num_acct], &buf_read[2], strlen(buf_read));    // save to rejected string array
            printf("accepted[%d]: %s\n", num_acct, accepted[num_acct++]);
        }
        else if (buf_read[0] == 'R') {
            memcpy(rejected[num_rejt], &buf_read[2], strlen(buf_read));    // save to rejected string array
            printf("rejected[%d]: %s\n", num_rejt, rejected[num_rejt++]);
        }
        else {
            memcpy(schedule[num_schd], &buf_read[0], strlen(buf_read));    // save to schedule string array
            printf("schedule[%d]: %s\n", num_schd, schedule[num_schd++]);
        }
    }
    
    // write to file
    writeSchedule(schedule, period, plants_limit, num_schd);
    writeAnalysis();
    writePerformance();
    
    // close both files
    fclose(report_file);
    fclose(raw_file);
}


/*
Report of XXX (...) scheduling

========================================================================================== (90)

Schedule Report
Period from [period range]

------------------------------------------------------------------------------------------ (90)
 
Plant X (300 per day):
           3t                  2t                  2t              2t
Date            Order Number        Product Name        Quantity        Due Date
YYYY-MM-DD      P_XXX               Product_A           XXXX            YYYY-MM-DD

------------------------------------------------------------------------------------------ (90)

Plant Y (400 per day):
 
...
 
========================================================================================== (90)
  
Analysis Report
 
------------------------------------------------------------------------------------------ (90)
 
Orders Accepted (total of xxx orders):
               2t              2t             3t              3t
Product Name        Quantity        Start           Finish          Duration
P_XXX               XXXX            YYYY-MM-DD      YYYY-MM-DD      XXXX

------------------------------------------------------------------------------------------ (90)
 
Orders Rejected (total of xxx orders):
               2t                  2t               2t
Order Number        Product Name        Quantity        Due Date
P_XXX               Product_A           XXXX            YYYY-MM-DD

========================================================================================== (90)

Performance Report
 
Earliest Due Date (EDD) Scheduling:
         2t                          4t  3t  3t
Plant_X     Number of days in use               XXX days
            Number of products produced         XXX (in total)
            Utilization of the plant            XX.XX %
            
Overall utilization                             XX.XX %
 
Shortest Job First (SJF) Scheduling:
 
...

 */
