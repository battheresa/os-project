#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utility.c"
#include "inputModule.c"
//#include "scheduleModule.c"
#include "outputModule.c"

void pleaseEnter(char str[]) {
    printf("Please enter:\n> ");
    fgets(str, CMD_LENGTH, stdin);  // ask user command
    str[strlen(str) - 1] = 0;       // remove new line from command
    //printf("instruction: %s\n", str);
}

int main() {
    char instruction[CMD_LENGTH];
    char command[CMD_LENGTH];
    int index, duration, arrival_time = 0;
    
    printf("\n~~ WELCOME TO PLS ~~\n");
    pleaseEnter(instruction);   // get instruction
    
    while (strcmp(instruction, "exitPLS") != 0) {
        index = indexOf(instruction, ' ', 0, strlen(instruction));  // find space in the instruction string
        substring(instruction, command, 0, index);  // extract command from instruction string
        substring(instruction, instruction, index + 1, strlen(instruction));    // adjust instruction string
        
        if (strcmp(command, "addPERIOD") == 0) {
            char date1[SUB_LENGTH], date2[SUB_LENGTH];
            
            index = indexOf(instruction, ' ', 0, strlen(instruction));
            substring(instruction, date1, 0, index);                        // extract first date
            substring(instruction, date2, index + 1, strlen(instruction));  // extract second date
            
            duration = addPERIOD(date1, date2);
            //printf("duration from %s to %s = %d days\n", date1, date2, duration);
        }
        else if (strcmp(command, "addORDER") == 0) {
            addORDER(instruction, arrival_time);
            arrival_time++;
        }
        else if (strcmp(command, "addBATCH") == 0) {
            addBATCH(instruction, arrival_time);
            arrival_time++;
        }
        else if (strcmp(command, "runPLS") == 0) {
            char algorithm[MIN_LENGTH];
            
            index = indexOf(instruction, ' ', 0, strlen(instruction));
            substring(instruction, algorithm, 0, index);    // extract algorithm name
            
            printf("runPLS runs algorithm %s\n", algorithm);
            arrival_time++;
        }
        else if (strcmp(command, "printREPORT") == 0) {
            char filename[SUB_LENGTH];
            
            index = indexOf(instruction, ' ', 0, strlen(instruction));
            substring(instruction, filename, 0, index);     // extract report file name
            
            printf("printREPORT print report to %s\n", filename);
            arrival_time++;
        }
        
        index = indexOf(instruction, '|', 0, strlen(instruction)); // find index of pipe
        if (index >= 0) {   // if pipe symbol exist, don't ask for new instruction
            substring(instruction, instruction, index + 2, strlen(instruction));    // adjust instruction
            continue;
        }
        
        pleaseEnter(instruction);   // get next instruction
    }
    
    printf("~~ BYE-BYE ~~\n\n");
    return 0;
}
