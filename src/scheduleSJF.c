#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheduleUtility.c"    // move to scheduleModule.c later


// creates the queue
int sjf_queue(Order orders_read[], Order queue[], int queue_length, int day_now) {
    // sort order according to quantity
    // only consider order with arrival_date == day_now
    
    return queue_length;
}


int main() {
    Order orders_read[ORDER_SIZE];
    total_order = readOrders(orders_read);  // read orders from file
    
    printSchedule('A', total_order, orders_read);   // test printing out all orders
    
    Order order_now;
    Order queue[ORDER_SIZE];
    
    int queue_length = 0;
    int order_amount = 0;
    
    int last_arrival_date = orders_read[total_order - 1].arrival_date;
    int day_now = 0;    // current day
    
    // reset queue everyday
    while (day_now <= last_arrival_date) {
        queue_length = sjf_queue(orders_read, queue, queue_length, day_now);
        queue_length = overdue(queue, unfinished, queue_length, day_now);   // remove overdue orders from queue
        
        // assign to plantX, plantY, or plantZ
        // use plants_limit array in scheuldUtility.c to check the limit of each plant
    }
    
    printSchedule(plants_code[0], day_now, plantX);
    printSchedule(plants_code[1], day_now, plantY);
    printSchedule(plants_code[2], day_now, plantZ);
    
    return 0;
}

