#include <stdio.h>
#include <stdlib.h>

FILE *in_file, *out_file, *invalid_file;
char temp[SUB_LENGTH];

// --------------------------------------------------------------------------------

int addPERIOD(char from[], char to[]) {
    Date start = constructDate(from);
    Date end = constructDate(to);
    
    if (!isValidDate(start) || !isValidDate(end))   // if invalid date, return -1
        return -1;
    
    setPeriod(start, end);
    
    return dateToDays(start, end);
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
    char temp[SUB_LENGTH];
    int index = indexOf(data, ' ', 0, strlen(data));
    substring(data, temp, index + 1, index + 11);
    
    Date date = constructDate(temp);
    if (end_period.year < date.year || end_period.month < date.month) {
        //printf("invalid order\n");
        fprintf(invalid_file, "%s\n", data);
    }
    else {
        //printf("valid order\n");
        fprintf(out_file, "%d %s\n", time, data);
    }
    
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
    
    while (fgets(temp, CMD_LENGTH, in_file)) {  // while not EOF
        temp[strlen(temp) - 1] = 0;     // remove new line from temp
        addORDER(temp, time);
    }
    
    fclose(in_file);
}
