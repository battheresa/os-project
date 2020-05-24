#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

FILE *report_file, * raw_file;

char *temp_out[SUB_LENGTH];
char buf_read[ORD_LENGTH];
char buf_write[ORD_LENGTH];
char thick_line[] = "==========================================================================================";
char thin_line[] = "------------------------------------------------------------------------------------------";


// reset temp_out to empty
void reset() {
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < SUB_LENGTH; j++)
            temp_out[i][j] = 0;
}


// write the schedule of each plant to report file
void writeSchedule(char *data[], char period[], int limits[], int length) {
    fprintf(report_file, "\n%s\n\n", thick_line);
    fprintf(report_file, "Schedule Report\n");
    fprintf(report_file, "Period %s\n\n\n", period);
    
    char current = 'N';
    char today_date[SUB_LENGTH];
    int plant_now = 0, line = 0, today_num;
    
    while (line < length) {
        reset();
        
        if (data[line][0] == current) {    // if first char in schedule is the same plant
            dateToString(daysToDate(today_num), today_date);
            
            substring(data[line], data[line], 2, strlen(data[line]));  // remove plant and new line
            split(data[line], temp_out, " ");
            
            if (strcmp(temp_out[0], "NA") == 0)
                fprintf(report_file, "%s\t\t\tNA\t\t\t\t\tNA\t\t\t\t\tNA\t\t\t\tNA\n", today_date);
            else
                fprintf(report_file, "%s\t\t\t%s\t\t\t\t%s\t\t\t%s\t\t\t\t%s\n", today_date, temp_out[0], temp_out[1], temp_out[2], temp_out[3]);
            
            today_num++;
            line++;
        }
        else {
            current = data[line][0];
            today_num = 0;
            
            if (line != 0)
                fprintf(report_file, "\n%s\n\n", thin_line);
            
            fprintf(report_file, "Plant %c (%d per day):\n\n", current, limits[plant_now++]);
            fprintf(report_file, "Date\t\t\t\tOrder Number\t\tProduct Name\t\tQuantity\t\tDue Date\n");
        }
    }
}


// write accepted and rejected orders to report file
void writeAnalysis(char *accepted[], char *rejected[], int length_acct, int length_rejt) {
    fprintf(report_file, "\n%s\n\n", thick_line);
    fprintf(report_file, "Analysis Report\n\n\n");
    
    fprintf(report_file, "Orders Accepted (total of %d orders):\n\n", length_acct);
    fprintf(report_file, "Order Number\t\tProduct Name\t\tQuantity\t\tArrival\t\t\tFinish\t\t\tDuration\n");
    for (int i = 0; i < length_acct; i++) {
        reset();
        split(accepted[i], temp_out, " ");
        
        if (strlen(temp_out[2]) < 4)
            fprintf(report_file, "%s\t\t\t\t%s\t\t\t%s\t\t\t\t%s\t\t%s\t\t%s\n", temp_out[0], temp_out[1], temp_out[2], temp_out[3], temp_out[4], temp_out[5]);
        else
            fprintf(report_file, "%s\t\t\t\t%s\t\t\t%s\t\t\t%s\t\t%s\t\t%s\n", temp_out[0], temp_out[1], temp_out[2], temp_out[3], temp_out[4], temp_out[5]);
    }
    
    fprintf(report_file, "\n%s\n\n", thin_line);
    
    fprintf(report_file, "Orders Rejected (total of %d orders):\n\n", length_rejt);
    fprintf(report_file, "Order Number\t\tProduct Name\t\tQuantity\t\tDue Date\n");
    for (int i = 0; i < length_rejt; i++) {
        reset();
        split(rejected[i], temp_out, " ");
        
        if (strlen(temp_out[2]) < 4)
            fprintf(report_file, "%s\t\t\t\t%s\t\t\t%s\t\t\t\t%s\n", temp_out[0], temp_out[1], temp_out[2], temp_out[3]);
        else
            fprintf(report_file, "%s\t\t\t\t%s\t\t\t%s\t\t\t%s\n", temp_out[0], temp_out[1], temp_out[2], temp_out[3]);
    }
}


// write performance to report file
void writePerformance(char *perfmn_edd[], char *perfmn_sjf[], int length) {
    fprintf(report_file, "\n%s\n\n", thick_line);
    fprintf(report_file, "Performance Report\n\n\n");
    
    fprintf(report_file, "Algorithm EDD (Earliest Due Date):\n\n");
    for (int i = 0; i < length; i++) {
        reset();
        split(perfmn_edd[i], temp_out, " ");
        
        fprintf(report_file, "Plant %s", temp_out[0]);
        fprintf(report_file, "\t\tNumber of days in use\t\t\t\t%s days\n", temp_out[1]);
        fprintf(report_file, "\t\t\tNumber of products produced\t\t\t%s in total\n", temp_out[2]);
        fprintf(report_file, "\t\t\tUtilization of the plant\t\t\t%s\n", temp_out[3]);
    }
    fprintf(report_file, "Overall utilization\t\t\t\t\t\t\t\t%s\n", perfmn_edd[length]);
    
    fprintf(report_file, "\n%s\n\n", thin_line);
    
    fprintf(report_file, "Algorithm SJF (Shortest Jod First):\n\n");
    for (int i = 0; i < length; i++) {
        reset();
        split(perfmn_sjf[i], temp_out, " ");
        fprintf(report_file, "Plant %s", temp_out[0]);
        
        fprintf(report_file, "\t\tNumber of days in use\t\t\t\t%s days\n", temp_out[1]);
        fprintf(report_file, "\t\t\tNumber of products produced\t\t\t%s in total\n", temp_out[2]);
        fprintf(report_file, "\t\t\tUtilization of the plant\t\t\t%s\n", temp_out[3]);
    }
    fprintf(report_file, "Overall utilization\t\t\t\t\t\t\t\t%s\n", perfmn_sjf[length]);
}


void printREPORT(char filename[]) {
    char report_path[CMD_LENGTH] = "_reports/";
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
    for (int i = 0; i < 200; i++) {
        schedule[i] = malloc(sizeof(char) * ORD_LENGTH);
        accepted[i] = malloc(sizeof(char) * ORD_LENGTH);
        rejected[i] = malloc(sizeof(char) * ORD_LENGTH);
    }
    
    for (int i = 0; i < 6; i++)
        temp_out[i] = malloc(sizeof(char) * SUB_LENGTH);
        
    // read first line (report title) and write to report file
    fgets(buf_read, ORD_LENGTH, raw_file);
    buf_read[strlen(buf_read) - 1] = 0;
    fprintf(report_file, "Report of %s Scheduling Algorithm\n", buf_read);
    
    // read period of the schedule
    fgets(period, ORD_LENGTH, raw_file);
    period[strlen(period) - 1] = 0;
    
    // read limits of each plants
    fgets(buf_read, ORD_LENGTH, raw_file);
    buf_read[strlen(buf_read)] = 0;
    
    int num_plants = split(buf_read, temp_out, " ");
    
    // initialize arrays
    int limits[num_plants];
    char *perfmn_edd[CMD_LENGTH];
    char *perfmn_sjf[CMD_LENGTH];
    
    for (int i = 0; i < num_plants + 1; i++) {
        perfmn_edd[i] = malloc(sizeof(char) * CMD_LENGTH);
        perfmn_sjf[i] = malloc(sizeof(char) * CMD_LENGTH);
    }
    
    for (int i = 0; i < num_plants; i++)
        limits[i] = atoi(temp_out[i]);
    
    // read performance of each plant of EDD
    for (int i = 0; i < num_plants + 1; i++) {
        fgets(perfmn_edd[i], ORD_LENGTH, raw_file);
        perfmn_edd[i][strlen(perfmn_edd[i])] = 0;
    }
    
    // read performance of each plant of SJF
    for (int i = 0; i < num_plants + 1; i++) {
        fgets(perfmn_sjf[i], ORD_LENGTH, raw_file);
        perfmn_sjf[i][strlen(perfmn_sjf[i])] = 0;
    }
    
    // read the rest of the file
    while (fgets(buf_read, ORD_LENGTH, raw_file)) {
        buf_read[strlen(buf_read)] = 0; // add EOF
        
        if (buf_read[0] == 'A') {
            memcpy(accepted[num_acct], &buf_read[2], strlen(buf_read) - 3);    // save to rejected string array
            //printf("accepted[%d]: %s\n", num_acct, accepted[num_acct]);
            num_acct++;
        }
        else if (buf_read[0] == 'R') {
            memcpy(rejected[num_rejt], &buf_read[2], strlen(buf_read) - 3);    // save to rejected string array
            //printf("rejected[%d]: %s\n", num_rejt, rejected[num_rejt]);
            num_rejt++;
        }
        else {
            memcpy(schedule[num_schd], &buf_read[0], strlen(buf_read) - 1);    // save to schedule string array
            //printf("schedule[%d]: %s\n", num_schd, schedule[num_schd]);
            num_schd++;
        }
    }
    
    // write to file
    writeSchedule(schedule, period, limits, num_schd);
    writeAnalysis(accepted, rejected, num_acct, num_rejt);
    writePerformance(perfmn_edd, perfmn_sjf, num_plants);
    
    // close both files
    fclose(report_file);
    fclose(raw_file);
}
