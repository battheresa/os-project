#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.c"

int main(){

    int read_from_file(order ordersread[]);

    int no_plants = 3; //plants are also named into plant 0, plant 1, plant 2 etc
    int plants_orders[3] = {300,400,500}; //how many orders they can do in a day
    int total_order=0; //total orders
    char buf_in[100];
    char buf_out[100];

    //need to think of how to make this 
    order ordersread[200];

    //read orders from file
    total_order = read_from_file(ordersread);

    for (int i=0; i< total_order; i++){
        printf("%d",ordersread[i].arrival_date);
        printf("%s",ordersread[i].order_number);
        printf("%s",ordersread[i].due_date);
        printf("%d",ordersread[i].quantity);
        printf("%s",ordersread[i].product_name);

    }


    // int total_days=0; //how many days have passed
    // char *plant_order[endtime_of_the_plants][total_order];

    // //write all the orders the parent 
    // for (int i=0; i< total_days; i++){
    //     write(ctop[a][1],buf_in, 10); 
    // }

    // int day_now=0; //start the first day with 0
    // struct order order_now; 
    // char processed_order[no_plants][total_order];
    // int plant_now;
    // int count_assigned=0;
    // int orders_unfinished;
    // struct order queue[total_order];
    // char order_name[10];

    // for (int i=0; i<total_order; i++){

    //     //if there is an order that just arrived
    //     if (new_arrival){
    //        edd_queue(struct order queue);
    //     }

    //     order_now= queue[0];
    //     order_name = order_now.order_number;
    //     order_amount= order_now.quantity;
    //     orders_unfinished=order_amount;
    //     remove_from_queue(queue[0]);

    //     //while not all the plants are assigned yet
    //     while (count_assigned <3){ 

    //         if (orders_unfinished==0){
    //             order_name = queue[0].order_number;
    //             order_amount= queue[0].quantity;
    //             orders_unfinished=order_amount;
    //             remove_from_queue(queue[0]);
    //         }
            
    //         //assigning the order to the different plants
            
    //         /* havent figure out the code yet hahahha 
    //             should be related to best fit algorithm though
    //         */
    //     }

    //     //if the orders is still unfinished 
    //     if (orders_unfinished>0){
    //         order_now.quantity=orders_unfinished;
    //         add_to_beginning(order_now);
    //     }
    //     count_assigned=0;

    // }
        
}

/* read from the file*/
int read_from_file(order ordersread[]){

    int total_order=0;
    char temp[40];
    char *quant_temp;

    FILE *in_file = fopen("orders.txt", "r");   // open read file
    
    if (in_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    while (fgets(temp, CMD_LENGTH, in_file)) {  // while not EOF
        temp[strlen(temp) - 1] = 0;     // remove new line from temp
        //split the file

        ordersread[total_order].arrival_date= temp[0]-48;
        substring(temp, ordersread[total_order].order_number,2,5);
        substring(temp, quant_temp, 7,15);
        printf("%s", quant_temp);
        
    
        substring(temp, quant_temp,19,24);
        ordersread[total_order].quantity=atoi(quant_temp);
        substring(temp, ordersread[total_order].product_name,25,35);

        total_order++;
        
    }
    fclose(in_file);
    return total_order;

}

// /* what is the number of products in this order*/
// void amount_of_order(char order_now){

// }

// /*look if there is an arrival at this day*/
// void new_arrival(int day_now){

// }

// /*creates the queue*/
// void edd_queue(struct order order_now){
// }


// int free_index(){
//     //finding out when is the next free space
// }

// void add_to_beginning(){
//     //add to beginning of the queue
// }

// void remove_from_queue(){
//     //remove from the beginning of the queue

// }