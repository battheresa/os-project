#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int no_plants = 3; //plants are also named into plant 0, plant 1, plant 2 etc
    int plants_orders = [300,400,500] //how many orders they can do in a day
    int total_order; //total orders
    char buf_in[100];
    char buf_out[100];
    
    struct order{
        int arrival_date;
        char order_number[10];
        int due_date;
        int quantity;
        char product_name;
    }

    //read from file to get the total_order
    total_order=;

    struct order ordersread[total_order];

    //read orders from file
    read_from_file(ordersread);

    //for the pipes
    int ptoc[10][2];
    int ctop[10][2];

    //creating pipes
    for (int i=0; i<no_plants; i++){
        pipe(ptoc[i]);
        pipe(ctop[i]);
    }

    for(int a=0;a<no_plants;a++) 
    {
        
        if(fork() == 0) 
        { 
            int total_days=0; //how many days have passed
            char *plant_order[endtime_of_the_plants][total_order];

            close(ptoc[a][1]); /* close child out */
            close(ctop[a][0]);


            while(true){
                read(ptoc[a][0],buf_in, 10); //read from parent for what order they have to process

                if (strcmp(buf_in, "done")==0){
                    break;
                }

                //places it in the order
                plant_order[total_days]=buf_in;

            }

            //write all the orders the parent 
            for (int i=0; i< total_days; i++){
                write(ctop[a][1],buf_in, 10); 
            }

        }

        close(ptoc[a][0]); /* close child out */
        close(ctop[a][1]);
        exit(0);
    }

    int day_now=0; //start the first day with 0
    struct order order_now; 
    char order_name[10];
    char processed_order[no_plants][total_order];
    int plant_now;
    int count_assigned=0;
    int orders_unfinished;
    struct order queue[total_order];
    int order_time;

    for (int i=0; i<total_order; i++){

        //if there is an order that just arrived
        if (new_arrival){
           edd_queue(struct order queue);
        }

        order_now= queue[0];
        order_name = order_now.order_number;
        order_amount= order_now.quantity;
        orders_unfinished=order_amount;
        remove_from_queue(queue[0]);

        //while not all the plants are assigned yet
        while (count_assigned <3){ 

            if (orders_unfinished==0){
                order_name = queue[0].order_number;
                order_amount= queue[0].quantity;
                orders_unfinished=order_amount;
                remove_from_queue(queue[0]);
            }
            
            //assigning the order to the different plants
            
            /* havent figure out the code yet hahahha 
                should be related to best fit algorithm though
            */
        }

        //if the orders is still unfinished 
        if (orders_unfinished>0){
            order_now.quantity=orders_unfinished;
            add_to_beginning(order_now);
        }
        count_assigned=0;

    }
        
}

/* read from the file*/
void read_from_file(struct order ordersread[]){
    

}

/* what is the number of products in this order*/
void amount_of_order(char order_now){

}

/*look if there is an arrival at this day*/
void new_arrival(int day_now){

}

/*creates the queue*/
void edd_queue(){
}


int free_index(){
    //finding out when is the next free space
}

void add_to_beginning(){
    //add to beginning of the queue
}

void remove_from_queue(){
    //remove from the beginning of the queue

}