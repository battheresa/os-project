#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utility.c"

#define ORDER_SIZE 1000

int plants_num = 3;
int plants_limit[] = {300, 400, 500};
char plants_code[] = {'X', 'Y', 'Z'};

int total_order = 0;    // total orders
int orders_unfinished;

// --------------------------------------------------------------------------------

typedef struct Order{
    int quantity;
    int arrival_date;
    char due_date[SUB_LENGTH];
    int finish_date;
    char order_number[SUB_LENGTH];
    char product_name[SUB_LENGTH];
} Order;

Order plantX[ORDER_SIZE];
Order plantY[ORDER_SIZE];
Order plantZ[ORDER_SIZE];
Order unfinished[ORDER_SIZE];

Order null_order = {-1, 0, "null", 0, "null", "null"};

// --------------------------------------------------------------------------------

// add order to beginning of the queue
int addOrder(Order order_now, Order queue[], int queue_length) {
    for (int i = queue_length - 1; i >= 0; i--)
        queue[i + 1] = queue[i];
    
    queue[0] = order_now;
    return queue_length++;
}

// remove from the beginning of the queue
int removeOrder(int idx, Order queue[], int queue_length) {
    for (int i = idx; i < queue_length - 1; i++)
        queue[i] = queue[i + 1];
    
    queue[queue_length - 1] = null_order;
    return queue_length--;
}

// --------------------------------------------------------------------------------

int readOrders(Order orders_read[]) {
    int total_order = 0, index = 0;
    char buf_read[CMD_LENGTH];
    char *token;

    FILE *in_file = fopen(order_path, "r");   // open read file
    
    if (in_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    while (fgets(buf_read, CMD_LENGTH, in_file)) {  // while not EOF
        buf_read[strlen(buf_read) - 1] = 0;     // remove new line from temp
        
        token = strtok(buf_read, " ");
        orders_read[total_order].arrival_date = atoi(token);
    
        token = strtok(NULL, " ");
        memcpy(orders_read[total_order].order_number, token, MIN_LENGTH);
        
        token = strtok(NULL, " ");
        memcpy(orders_read[total_order].due_date, token, MIN_LENGTH);
        
        token = strtok(NULL, " ");
        orders_read[total_order].quantity = atoi(token);
        
        token = strtok(NULL, " ");
        memcpy(orders_read[total_order].product_name, token, MIN_LENGTH);
        
        total_order++;
    }
    
    fclose(in_file);
    return total_order;
}

// --------------------------------------------------------------------------------

void printSchedule(char plant, int num, Order schedule[]) {
    printf("Plant %c queue:\n", plant);
    
    for (int i = 0; i < num; i++) {
        printf("%d ", schedule[i].arrival_date);
        printf("%s ", schedule[i].due_date);
        printf("%s ", schedule[i].order_number);
        printf("%d ", schedule[i].quantity);
        printf("%s ", schedule[i].product_name);
        printf("%d ", schedule[i].finish_date);
        printf("\n");
    }
    
    printf("----------------------------------------\n");
}

// --------------------------------------------------------------------------------

int overdue(Order queue[], Order unfinished[], int queue_length, int day_now) {
    for (int i = 0; i < queue_length; i++) {
        
    }
    
    
    
    /*
    Date now = {4, 1, 2020};  // use "setPeriod(start_date, end_date)" for testing
    int queue_length_saved = queue_length;
    int save1, save2;

    for (int i = 0; i < queue_length; i++) {
        if (i == 0)
            save1 = queue[0].quantity;
        else if (i == 1)
            save2 = queue[1].quantity;

        Date due_now = constructDate(queue[i].due_date);
        int difference = dateToDays(now, due_now);
        
        if (difference < day_now) {
            removeOrder(i, queue, due_now);
        }
        else if (difference == day_now) {
            if (i == 0) {
                if (queue[0].quantity > 1200)
                    removeOrder(i,queue, queue_length);
            }
            else if (i == 1) {
                for (int i = 0; i < ) {
                    if (save1 > 300 && queue[].quantity) {
                        // do something
                    }
                }
            }
            else if (i == 2) {
                if (save1 save2) {
                    // do something
                }
            }
            else {
                removeOrder(i, queue, queue_length);
            }
        }
    }
    */
}
