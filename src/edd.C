#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.c"

int main(){

    int read_from_file(Order ordersread[]);
    void edd_queue(Order ordersread[], Order queue[], int* queue_length, int day_now, int total_order);
    void remove_beginning(Order queue[], int* queue_length);
    void add_to_beginning(Order order_now, Order queue[], int* queue_length);
    int countfilled(int plantfilled[]);


    int no_plants = 3; //plants are also named into plant 0, plant 1, plant 2 etc
    int total_order=0; //total orders
    char buf_in[100];
    char buf_out[100];

    //need to think of how to make this 
    // Order *ordersread = malloc(total_order * sizeof(Order));
    Order ordersread[1000];

    //read orders from file
    total_order = read_from_file(ordersread);

    int day_now=0; //start the first day with 0
    int total_processed=0;
    Order order_now; 
    Order queue[1000];
    Order plantA[1000];
    Order plantB[1000];
    Order plantC[1000];
    int plantA_tot=0, plantB_tot=0, plantC_tot=0;
    int plant_now;
    int queue_length=0;
    int count_assigned=0;
    int orders_unfinished;
    char order_name[15];
    int order_amount;
    
    int quantity;
    int arrival_date;
    char due_date[SUB_LENGTH];
    int finish_date;
    char order_number[SUB_LENGTH];
    char product_name[SUB_LENGTH];
    Order null_order={0,0,"",0,"",""};

    // for (int i=0; i<4; i++){
    //     edd_queue(ordersread, queue, &queue_length, day_now, total_order);
    //     day_now++;
    // }
    
    //every new day, it restarts the queue
    while(true){
        edd_queue(ordersread, queue, &queue_length, day_now, total_order);

        order_now= queue[0];
        total_processed++;
        orders_unfinished=order_now.quantity;
        remove_beginning(queue, &queue_length);

        int plant_filled[3]={0,0,0};
        for (int count_assigned=0; count_assigned<3; count_assigned++){

            if (orders_unfinished==0){
                order_now= queue[0];
                if (order_now.quantity==0){
                    break;
                }
                else{
                    orders_unfinished=order_now.quantity;
                    remove_beginning(queue, &queue_length);
                }
                total_processed++;
            }
            
            //assigning the order to the different plants
            // printf("%d\n",countfilled(plant_filled));
            
            printf("%s %d ",order_now.order_number,orders_unfinished);
            if ((countfilled(plant_filled)>=2 || orders_unfinished<=300)&&(plant_filled[0]==0)){

                plantA[plantA_tot]=order_now;
                plantA[plantA_tot].finish_date=day_now;

                if (orders_unfinished<=300){
                    plantA[plantA_tot].quantity=orders_unfinished;
                    orders_unfinished=0;
                }
                else{
                    plantA[plantA_tot].quantity=300;
                    orders_unfinished-=300;
                }
                
                plantA_tot++;
                plant_filled[0]=-1;
                printf("300\n");
                
            }
            else if ((countfilled(plant_filled)>=1 || orders_unfinished<=400)&&(plant_filled[1]==0 )){
                plantB[plantB_tot]=order_now;
                plantB[plantB_tot].finish_date=day_now;

                if (orders_unfinished<=400){
                    plantB[plantB_tot].quantity=orders_unfinished;
                    orders_unfinished=0;
                }
                else{
                    plantB[plantB_tot].quantity=400;
                    orders_unfinished-=400;
                }
                plantB_tot++;
                plant_filled[1]=-1;
                printf("400\n");

            }
            else{
                plantC[plantC_tot]=order_now;
                plantC[plantC_tot].finish_date=day_now;

                if (orders_unfinished<=500){
                    plantC[plantC_tot].quantity=orders_unfinished;
                    orders_unfinished=0;
                }
                else{
                    plantC[plantC_tot].quantity=500;
                    orders_unfinished-=500;
                }
                plantC_tot++;
                plant_filled[2]=-1;
                printf("500\n");

            }
            


        }

        if (orders_unfinished>0){
            order_now.quantity=orders_unfinished;
            total_processed--;
            add_to_beginning(order_now, queue, &queue_length);
        }

        if (total_processed==total_order){
            break;
        }
        day_now++;

        

    }
    printf("plant A queue:\n");
    for (int i=0;i<plantA_tot;i++){
        printf("%d ", plantA[i].arrival_date);
        printf("%s ", plantA[i].due_date);
        printf("%s ", plantA[i].order_number);
        printf("%d ", plantA[i].quantity);
        printf("%s ", plantA[i].product_name);
        printf("%d ", plantA[i].finish_date);
        printf("\n");
    }
    printf("--------------\n");

    printf("plant B queue:\n");
    for (int i=0;i<plantB_tot;i++){
        printf("%d ", plantB[i].arrival_date);
        printf("%s ", plantB[i].due_date);
        printf("%s ", plantB[i].order_number);
        printf("%d ", plantB[i].quantity);
        printf("%s ", plantB[i].product_name);
        printf("%d ", plantB[i].finish_date);
        printf("\n");
    }
    printf("----------\n");

    printf("plant C queue:\n");
    for (int i=0;i<plantC_tot;i++){
        printf("%d ", plantC[i].arrival_date);
        printf("%s ", plantC[i].due_date);
        printf("%s ", plantC[i].order_number);
        printf("%d ", plantC[i].quantity);
        printf("%s ", plantC[i].product_name);
        printf("%d ", plantC[i].finish_date);
        printf("\n");
    }

    printf("///////////////////////////\n");
    
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
            // printf("queue length: %d\n",*queue_length);

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
                    queue[k+1]=queue[k];
                }
                queue[placed]=ordersread[i];
                (*queue_length)++;

            }

            // for (int i=0;i<*queue_length;i++){
            //     printf("%d ", queue[i].arrival_date);
            //     printf("%s ", queue[i].due_date);
            //     printf("%s ", queue[i].order_number);
            //     printf("%d ", queue[i].quantity);
            //     printf("%s ", queue[i].product_name);
            //     printf("\n");
            // }
            // printf("\n");
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

/*remove from the beginning of the queue*/
void remove_beginning(Order queue[], int* queue_length){
    for (int i=0; i< (*queue_length)-1; i++){
        queue[i]=queue[i+1];
    }
    queue[*(queue_length)-1].quantity=0;
    (*queue_length)--;
}

/*add to beginning of the queue*/
void add_to_beginning(Order order_now, Order queue[], int* queue_length){

    for (int k=(*queue_length)-1;k>=0; k--){
        queue[k+1]=queue[k];
    }
    queue[0] = order_now;
    (*queue_length)++;
}

int countfilled(int plantfilled[]){
    int count=0;
    for (int i=0; i<3; i++){
        if (
            plantfilled[i]==-1){
            count++;
        }
    }
    return count;
}
