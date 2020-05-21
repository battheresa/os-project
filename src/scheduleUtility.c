#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utility.c"

#define ORDER_SIZE 1000

int plants_num = 3;
int plants_limit[] = {300, 400, 500};
int plants_days_use[] = {0, 0, 0};
int plants_produced[] = {0, 0, 0};
char plants_code[] = {'X', 'Y', 'Z'};

int total_order = 0;
int num_finished = 0;   // TODO: count finished orders
int num_unfinished = 0; // TODO: count unfinished order

// --------------------------------------------------------------------------------

typedef struct Order{
    int quantity;
    int arrival_date;
    char due_date[SUB_LENGTH];
    int finish_date;
    char order_number[SUB_LENGTH];
    char product_name[SUB_LENGTH];
} Order;

Order orders_read[ORDER_SIZE];

Order plantX[ORDER_SIZE];
Order plantY[ORDER_SIZE];
Order plantZ[ORDER_SIZE];
Order finished[ORDER_SIZE];     // TODO: keep track of all finished orders
Order unfinished[ORDER_SIZE];

Order null_order = {-1, 0, "null", 0, "null", "null"};

// --------------------------------------------------------------------------------

// add order at index
int addOrder(int index, Order order_now, Order queue[], int queue_length) {
    for (int i = queue_length; i > index; i--)
        queue[i] = queue[i - 1];
    
    queue[index] = order_now;
    return queue_length + 1;
}

// remove order at index
int removeOrder(int index, Order queue[], int queue_length) {
    for (int i = index; i < queue_length - 1; i++)
        queue[i] = queue[i + 1];
    
    queue[queue_length - 1] = null_order;
    return queue_length - 1;
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
        
        char *temp[SUB_LENGTH];
        for (int i = 0; i < 6; i++)
            temp[i] = malloc(sizeof(char) * SUB_LENGTH);
        
        split(buf_read, temp, " ");
        
        orders_read[total_order].arrival_date = atoi(temp[0]);
        memcpy(orders_read[total_order].order_number, temp[1], MIN_LENGTH);
        memcpy(orders_read[total_order].due_date, temp[2], MIN_LENGTH);
        orders_read[total_order].quantity = atoi(temp[3]);
        memcpy(orders_read[total_order].product_name, temp[4], MIN_LENGTH);
        
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
    Date due_now, today = daysToDate(day_now);
    
    for (int i = 0; i < queue_length; i++) {
        due_now = constructDate(queue[i].due_date);
        
        if (!isBefore(today, due_now, false)) {   // if order due today or overdue
            removeOrder(i, queue, queue_length);
            printf("Order at %d removed...", i);
        }
    }
    
    return queue_length;
}

// --------------------------------------------------------------------------------

bool compareOrder(Order a, Order b) {
    if (a.arrival_date != b.arrival_date)
        return false;

    if (strcmp(a.due_date, b.due_date) != 0)
        return false;
    
    if (strcmp(a.order_number, b.order_number) != 0)
        return false;
    
    if (strcmp(a.product_name, b.product_name) != 0)
        return false;
    
    return true;
}


bool isUnfinished(Order check, Order unfinished[]) {
    for (int i = 0; i < num_unfinished; i++)
        if (compareOrder(check, unfinished[i]))
            return true;
    
    return false;
}


// find the finished date
int findLast(Order now, int length) {
    int last = 0;
    
    for (int i = 0; i < length; i++)
        if (compareOrder(now, plantX[i]) || compareOrder(now, plantY[i]) || compareOrder(now, plantZ[i]))
            last = i;
    
    return last;
}


// subtract from total_order from unfinished order and update the finished order
void generateFinished(Order orders_read[], Order unfinished[], int length) {
    for (int i = 0; i < total_order; i++) {
        if (!isUnfinished(orders_read[i], unfinished)) {
            finished[num_finished] = orders_read[i];
            finished[num_finished].finish_date = findLast(orders_read[i], length);
            num_finished++;
        }
    }
}
