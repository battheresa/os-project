#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduleUtility.c"    // move to scheduleModule.c later
  
void bubbleSort(Order arr[], int n) { 
    int i, j;
    struct Order temp;
    for (i = 0; i < n; ++i) {
        for (j = i + 1; j < n; ++j) {
            if (arr[i].quantity > arr[j].quantity) {
                temp =  arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
} 

// creates the queue
int sjf_queue(Order orders_read[], Order queue[], int queue_length, int day_now) {
    
    for (int i = 0; i < total_order; i++) { //put the newly arrivals to the waiting queue
        if (orders_read[i].arrival_date == day_now) { // only consider order with arrival_date == day_now
            queue[queue_length] = orders_read[i];
            queue_length++;
        }
    }
    for (int i = 0; i < queue_length; i++) { //sort queue order according to quantity
        bubbleSort(queue,queue_length);
    }
    return queue_length;
}

// count plants filled
int count_filled(int plant_filled[]) {
    int count = 0;
    
    for (int i = 0; i < 3; i++)
        if (plant_filled[i] == -1)
            count++;
    
    return count;
}

int main() {
    total_order = readOrders(orders_read);  // read orders from file
    
    //printSchedule('A', total_order, orders_read);   // test printing out all orders
    
    Order order_now;
    Order queue[ORDER_SIZE];
    
    int queue_length = 0;
    int order_amount = 0;
    int orders_unfinished = 0;
    
    int last_arrival_date = orders_read[total_order - 1].arrival_date;
    int day_now = 0;    // current day
    
    // reset queue everyday
    while (day_now <= last_arrival_date) {
        queue_length = sjf_queue(orders_read, queue, queue_length, day_now);
        queue_length = overdue(queue, unfinished, queue_length, day_now);   // remove overdue orders from queue
        
        order_now = queue[0]; //order struct
        orders_unfinished = order_now.quantity; //order struct quantity
        queue_length = removeOrder(0, queue, queue_length);
        int plant_filled[3] = {0, 0, 0};

        for (int assigned = 0; assigned < 3; assigned++) {
            // printf("Day:%d :", day_now);
            // for (int i = 0; i < queue_length; i++) { //print waiting queue
            //     printf("%d ", queue[i].quantity);
            // }
            // printf("\n");
            if (orders_unfinished == 0) {
                order_now = queue[0];
                if (order_now.quantity == -1)
                    break;
                orders_unfinished = order_now.quantity;
                queue_length = removeOrder(0, queue, queue_length);                
            }
            if ((count_filled(plant_filled) >= 2 || orders_unfinished <= plants_limit[0]) && (plant_filled[0] == 0)){
                plantX[day_now] = order_now; //pass in the day_now struct to the plantX struct array
                if (orders_unfinished <= plants_limit[0]) {
                    plantX[day_now].quantity = orders_unfinished;
                    orders_unfinished = 0;
                }
                else{
                    plantX[day_now].quantity = plants_limit[0];
                    orders_unfinished -= plants_limit[0];
                }
                plant_filled[0] = -1;
            }
            else if ((count_filled(plant_filled) >= 1 || orders_unfinished <= plants_limit[1]) && (plant_filled[1] == 0)) {
                plantY[day_now] = order_now; //pass in the day_now struct to the plantY struct array
                if (orders_unfinished <= plants_limit[1]) {
                    plantY[day_now].quantity = orders_unfinished;
                    orders_unfinished = 0;
                }
                else {
                    plantY[day_now].quantity = plants_limit[1];
                    orders_unfinished -= plants_limit[1];
                }
                plant_filled[1] = -1;
            }
            else{
                plantZ[day_now] = order_now; //pass in the day_now struct to the plantZ struct array
                if (orders_unfinished <= plants_limit[2]) {
                    plantZ[day_now].quantity = orders_unfinished;
                    orders_unfinished = 0;
                }
                else{
                    plantZ[day_now].quantity = plants_limit[2];
                    orders_unfinished -= plants_limit[2];
                }
                plant_filled[2] = -1;
            }
            // printSchedule(plants_code[0], day_now + 1, plantX);
            // printSchedule(plants_code[1], day_now + 1, plantY);
            // printSchedule(plants_code[2], day_now + 1, plantZ);
        }
        // assign to plantX, plantY, or plantZ
        // use plants_limit array in scheuldUtility.c to check the limit of each plant
        day_now++;
    }
    printSchedule(plants_code[0], day_now + 1, plantX);
    printSchedule(plants_code[1], day_now + 1, plantY);
    printSchedule(plants_code[2], day_now + 1, plantZ);
    
    
    return 0;
}