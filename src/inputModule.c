#include <stdio.h>
#include <stdlib.h>

FILE *in_file, *out_file;
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
    out_file = fopen(order_file, "a");  // open write file
    
    if (out_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    // parse data into seperate items if needed
    
    fprintf(out_file, "%d %s\n", time, data);
    fclose(out_file);
}

// --------------------------------------------------------------------------------

void addBATCH(char batch_file[], int time) {
    in_file = fopen(batch_file, "r");   // open read file
    
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
