#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.c"

int main(){

    int read_from_file(Order ordersread[]);
    void edd_queue(Order ordersread[], Order queue[], int* queue_length, int day_now, int total_order);


    int no_plants = 3; //plants are also named into plant 0, plant 1, plant 2 etc
    int plants_orders[3] = {300,400,500}; //how many orders they can do in a day
    int total_order=0; //total orders
    char buf_in[100];
    char buf_out[100];

    //need to think of how to make this 
    // Order *ordersread = malloc(total_order * sizeof(Order));
    Order ordersread[1000];

    //read orders from file
    total_order = read_from_file(ordersread);

    // for (int i=0; i< total_order; i++){
    //     printf("%d ",ordersread[i].arrival_date);
    //     printf("%s ",ordersread[i].order_number);
    //     printf("%s ",ordersread[i].due_date);
    //     printf("%d ",ordersread[i].quantity);
    //     printf("%s ",ordersread[i].product_name);

    //     printf("\n");
    // }

    int day_now=0; //start the first day with 0
    Order order_now; 
    int queue_length=0;
    int plant_now;
    int count_assigned=0;
    int orders_unfinished;
    Order queue[1000];
    char order_name[10];

    for (int i=0; i<4; i++){
        edd_queue(ordersread, queue, &queue_length, day_now, total_order);
        day_now++;
    }

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
int read_from_file(Order ordersread[]){

    int total_order=0;
    char temp[40];
    char quant_temp[15];

    FILE *in_file = fopen("orders.txt", "r");   // open read file
    
    if (in_file == NULL) {
        printf("Error in opening files\n");
        exit(1);
    }
    
    while (fgets(temp, CMD_LENGTH, in_file)) {  // while not EOF
        temp[strlen(temp) - 1] = 0;     // remove new line from temp
        //split the file

        ordersread[total_order].arrival_date= temp[0]-48;
        substring(temp, ordersread[total_order].order_number, 2, 5);
        substring(temp, ordersread[total_order].due_date, 8, 10);    
        substring(temp, quant_temp, 19, 24);
        ordersread[total_order].quantity=atoi(quant_temp);
        substring(temp, ordersread[total_order].product_name,24,9);

        total_order++;
        
    }
    fclose(in_file);
    return total_order;

}


/*creates the queue*/
void edd_queue(Order ordersread[], Order queue[], int* queue_length, int day_now, int total_order){
    
    int date_earlier(int first, int second, Date date_first, Date date_second);

    Date in_queue;
    Date to_queue;

    // printf("queue length: %d\n",*queue_length);
    for (int i= 0; i< total_order; i++){
        int placed=*queue_length;

        if (ordersread[i].arrival_date==day_now){
            printf("queue length: %d\n",*queue_length);

            to_queue=constructDate(ordersread[i].due_date);
            
            // printf("%s \n", ordersread[i].order_number);
            if (queue_length==0){
                queue[*queue_length]=ordersread[i];
                (*queue_length)++;
            }

            else{
                for (int j=0; j<*queue_length; j++){
                    in_queue= constructDate(queue[j].due_date);
                    
                    //if the date is earlier
                    if (date_earlier(i,j, to_queue, in_queue)==i){
                        placed=j;
                        break;
                    }
                }

                //move all the queue to the back and give space for the earliest
                for (int k=(*queue_length)-1;k>=placed; k--){
                    printf("hiiiiii %s \n", queue[k].order_number);
                    queue[k+1]=queue[k];
                }
                queue[placed]=ordersread[i];
                (*queue_length)++;

            }

            for (int i=0;i<*queue_length;i++){
                    printf("%d ", queue[i].arrival_date);
                    printf("%s ", queue[i].due_date);
                    printf("%s ", queue[i].order_number);
                    printf("%d ", queue[i].quantity);
                    printf("%s ", queue[i].product_name);
                    printf("\n");
                }
                printf("\n");
        }
    }
    
}

//note that this compares with 1 jan 2020
int date_earlier(int first, int second, Date date_first, Date date_second){
    //compare it to 1 january 2000? 
    Date now={01,01,2020};
    int firstno=dateToDays(now,date_first);
    int secondno=dateToDays(now,date_second);
    
    if (firstno<=secondno){
        return first;
    }
   
    else{
       return second;
    }
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