#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheduleUtility.c"    // move to scheduleModule.c later


// creates the queue
int edd_queue(Order orders_read[], Order queue[], int queue_length, int day_now) {
    int date_earlier(int first, int second, Date date_first, Date date_second);
    
    Date in_queue;
    Date to_queue;

    //printf("queue length: %d\n", queue_length);
    
    for (int i = 0; i < total_order; i++) {
        int placed = queue_length;

        if (orders_read[i].arrival_date == day_now) {
            //printf("queue length: %d\n",queue_length);

            to_queue = constructDate(orders_read[i].due_date);
            
            //printf("%s \n", orders_read[i].order_number);
            
            if (queue_length == 0) {
                queue[queue_length] = orders_read[i];
                queue_length++;
            }
            else {
                for (int j = 0; j < queue_length; j++) {
                    in_queue = constructDate(queue[j].due_date);
                    
                    if (date_earlier(i, j, to_queue, in_queue) == i) {  //if the date is earlier
                        placed = j;
                        break;
                    }
                }

                //move all the queue to the back and give space for the earliest
                for (int k = queue_length - 1; k >= placed; k--)
                    queue[k + 1] = queue[k];
                
                queue[placed] = orders_read[i];
                queue_length++;
            }
        }
    }
    
    return queue_length;
}


// note that this compares with 1 jan 2020
int date_earlier(int first, int second, Date date_first, Date date_second) {
    Date now = {01, 01, 2020}; // use "setPeriod(start_date, end_date)" for testing
    int first_num = dateToDays(now, date_first);
    int second_num = dateToDays(now, date_second);
    
    if (first_num <= second_num)
        return first;
    else
       return second;
}


int count_filled(int plant_filled[]) {
    int count = 0;
    
    for (int i = 0; i < 3; i++)
        if (plant_filled[i] == -1)
            count++;
    
    return count;
}


int main() {
    Order orders_read[ORDER_SIZE];
    total_order = readOrders(orders_read);  // read orders from file

    int last_arrival_date = orders_read[total_order - 1].arrival_date;
    int day_now = 0;    // start the first day with 0
    int count_assigned = 0;
    int total_processed = 0;
    
    Order order_now;
    Order queue[ORDER_SIZE];
    
    int queue_length = 0;
    int order_amount = 0;
    char order_name[15];
    
    int quantity;
    int arrival_date;
    char due_date[SUB_LENGTH];
    int finish_date;
    char order_number[SUB_LENGTH];
    char product_name[SUB_LENGTH];

    /*
    for (int i = 0; i < 4; i++) {
        edd_queue(orders_read, queue, queue_length, day_now);
        day_now++;
    }
    */
    
    // reset queue everyday
    while (day_now <= last_arrival_date) {
        edd_queue(orders_read, queue, queue_length, day_now);
        queue_length = overdue(queue, unfinished, queue_length, day_now);   // remove overdue orders from queue
        
        order_now = queue[0];
        total_processed++;
        orders_unfinished = order_now.quantity;
        queue_length = removeOrder(0, queue, queue_length);

        int plant_filled[3] = {0, 0, 0};
        
        for (int count_assigned = 0; count_assigned < 3; count_assigned++) {
            if (orders_unfinished == 0) {
                order_now = queue[0];
                
                if (order_now.quantity == -1)
                    break;
                
                orders_unfinished = order_now.quantity;
                queue_length = removeOrder(0, queue, queue_length);
                total_processed++;
            }
            
            //printf("%s %d ",order_now.order_number,orders_unfinished);
            
            // assigning the order to the different plants
            if ((count_filled(plant_filled) >= 2 || orders_unfinished <= plants_limit[0]) && (plant_filled[0] == 0)) {
                plantX[day_now] = order_now;
                plantX[day_now].finish_date = day_now;

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
                plantY[day_now] = order_now;
                plantY[day_now].finish_date = day_now;

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
            else {
                plantZ[day_now] = order_now;
                plantZ[day_now].finish_date = day_now;

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
            
        }

        if (orders_unfinished > 0) {
            order_now.quantity = orders_unfinished;
            total_processed--;
            queue_length = addOrder(order_now, queue, queue_length);
        }

        if (count_filled(plant_filled) <= 3) {
            if (plant_filled[0] == 0)
                plantX[day_now] = null_order;
            
            if (plant_filled[1] == 0)
                plantY[day_now] = null_order;
            
            if (plant_filled[2] == 0)
                plantZ[day_now] = null_order;
        }
        
        day_now++;
    }
    
    printSchedule(plants_code[0], day_now, plantX);
    printSchedule(plants_code[1], day_now, plantY);
    printSchedule(plants_code[2], day_now, plantZ);
    
    return 0;
}


