#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

FILE *in_file, *out_file, *invalid_file;
char temp_in[SUB_LENGTH];

// --------------------------------------------------------------------------------

void addPERIOD(char from[], char to[]) {
    Date start = constructDate(from);
    Date end = constructDate(to);
    
    if (!isValidDate(start) || !isValidDate(end))   // if invalid date, return -1
        return;
    
    setPeriod(start, end);
}

// --------------------------------------------------------------------------------

void addORDER(char data[], int time) {
    out_file = fopen(order_path, "a");  // open write file
    invalid_file = fopen(invalid_path, "a");  // open write file
    
    if (out_file == NULL || invalid_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    // parse data into seperate items if needed
    char temp_in[SUB_LENGTH];
    int index = indexOf(data, ' ', 0, strlen(data));
    substring(data, temp_in, index + 1, index + 11);
    
    Date date = constructDate(temp_in);
    if (isBefore(start_period, date, true))
        fprintf(invalid_file, "%s\n", data);
    else
        fprintf(out_file, "%d %s\n", time, data);
    
    fclose(out_file);
    fclose(invalid_file);
}

// --------------------------------------------------------------------------------

void addBATCH(char batch_file[], int time) {
    char batch_path[SUB_LENGTH] = "_resources/";
    strcat(batch_path, batch_file);
    
    in_file = fopen(batch_path, "r");   // open read file
    
    if (in_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    while (fgets(temp_in, CMD_LENGTH, in_file)) {  // while not EOF
        temp_in[strlen(temp_in) - 1] = 0;     // remove new line from temp
        addORDER(temp_in, time);
    }
    
    fclose(in_file);
}
