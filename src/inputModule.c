#include <stdio.h>
#include <stdlib.h>

FILE *in_file, *out_file;
char temp[SUB_LENGTH], order_file[] = "orders.txt";

// --------------------------------------------------------------------------------

int addPERIOD(char from[], char to[]) {
    struct Date start = constructDate(from);
    struct Date end = constructDate(to);
    
    if (!isValidDate(start) || !isValidDate(end))   // if invalid date, return -1
        return -1;
    
    return dateToDays(start, end);
}

// --------------------------------------------------------------------------------

void addORDER(char data[]) {
    out_file = fopen(order_file, "a");  // open write file
    
    if (out_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    // parse data into seperate items if needed
    
    fprintf(out_file, "%s\n", data);
    fclose(out_file);
}

// --------------------------------------------------------------------------------

void addBATCH(char batch_file[]) {
    in_file = fopen(batch_file, "r");   // open read file
    
    if (in_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    while (fgets(temp, CMD_LENGTH, in_file)) {  // ask user command) != EOF) {
        temp[strlen(temp) - 1] = 0;     // remove new line from temp
        addORDER(temp);
    }
    
    fclose(in_file);
}
