#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ORDER_SIZE 1000

int plants_num = 3;
int plants_limit[] = {300, 400, 500};
int plants_days_use[] = {0, 0, 0};
int plants_produced[] = {0, 0, 0};
char plants_code[] = {'X', 'Y', 'Z'};

int total_days = 0;
int total_order = 0;
int num_finished = 0;
int num_unfinished = 0;

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
Order finished[ORDER_SIZE];
Order unfinished[ORDER_SIZE];

Order null_order = {-1, 0, "null", 0, "null", "null"};

// --------------------------------------------------------------------------------

// add order at index
int addOrder(int index, Order this_order, Order array[], int length) {
    for (int i = length; i > index; i--)
        array[i] = array[i - 1];
    
    array[index] = this_order;
    return length + 1;
}

// remove order at index
int removeOrder(int index, Order array[], int length) {
    for (int i = index; i < length - 1; i++)
        array[i] = array[i + 1];
    
    array[length - 1] = null_order;
    return length - 1;
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
    
    char *temp[SUB_LENGTH];
    for (int i = 0; i < 6; i++)
        temp[i] = malloc(sizeof(char) * SUB_LENGTH);
    
    while (fgets(buf_read, CMD_LENGTH, in_file)) {  // while not EOF
        buf_read[strlen(buf_read) - 1] = 0;         // remove new line from temp
        
        split(buf_read, temp, " ");
        
        orders_read[total_order].arrival_date = atoi(temp[0]);
        memcpy(orders_read[total_order].order_number, temp[1], MIN_LENGTH);
        memcpy(orders_read[total_order].due_date, temp[2], MIN_LENGTH);
        orders_read[total_order].quantity = atoi(temp[3]);
        memcpy(orders_read[total_order].product_name, temp[4], MIN_LENGTH);
        
        total_order++;
    }
    
    for (int i = 0; i < 6; i++)
        free(temp[i]);
    
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

int overdue(Order queue[], Order unfinished[], int length, int day_now) {
    Date due_now, today = daysToDate(day_now);
    
    for (int i = 0; i < length; i++) {
        due_now = constructDate(queue[i].due_date);
        
        if (!isBefore(today, due_now, false)) {                     // if order due today or overdue
            length = addOrder(0, queue[i], unfinished, length);     // add overdued order to unfinished
            length = removeOrder(i, queue, length);                 // remove overdued queue to unfinished array
            printf("Order at %d removed...", i);
        }
    }
    
    return length;
}

// --------------------------------------------------------------------------------

void generateProduced(int length) {
    for (int i = 0; i < length; i++) {
        if (plantX[i].quantity > 0)
            plants_produced[0] += plantX[i].quantity;
        
        if (plantY[i].quantity > 0)
            plants_produced[1] += plantY[i].quantity;
        
        if (plantZ[i].quantity > 0)
            plants_produced[2] += plantZ[i].quantity;
    }
}

// --------------------------------------------------------------------------------

// compare two Orders
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

// check is the order is in unfinished array
bool isUnfinished(Order this_order, Order unfinished[]) {
    for (int i = 0; i < num_unfinished; i++)
        if (compareOrder(this_order, unfinished[i]))
            return true;
    
    return false;
}

// find the finished date
int findLast(Order this_order, int length) {
    int last = 0;
    
    for (int i = 0; i < length; i++)
        if (compareOrder(this_order, plantX[i]) || compareOrder(this_order, plantY[i]) || compareOrder(this_order, plantZ[i]))
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

// --------------------------------------------------------------------------------

// count plants filled
int count_filled(int plant_filled[]) {
    int count = 0;
    
    for (int i = 0; i < 3; i++)
        if (plant_filled[i] == -1)
            count++;
    
    return count;
}
