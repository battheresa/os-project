#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//read orders from file
char *orders[];

int main(){

    int no_plants; //plants are also named into plant 0, plant 1, plant 2 etc
    int total_order; //total orders
    char buf_in[100];
    char buf_out[100];

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
            int total_time=0;

            //time needed depends on how much time each process needs =0 not correct yet
            int time_needed=0;

            close(ptoc[a][1]); /* close child out */
            close(ctop[a][0]);


            // while loop should start here...
                //wait for parent to start
                read(ptoc[a][0],buf_in, 10);

                //write to parent when will the task be completed
                write(ctop[a][1],buf_out,10);

                //receive from parent to see if it should process the thing or not
                read(ptoc[a][0],buf_in, 10);

                if (strcpy(buf_in, "y")){
                    total_time+=time_needed;
                }
        }

        close(ptoc[a][0]); /* close child out */
        close(ctop[a][1]);
        exit(0);
    }

    char order_now;
    int processed_order[no_plants][total_order];
    int endtime[no_plants];
    int plant_now;
    order_now = next_process();

    // while loop should start here....

        for (int i=0; i<no_plants; i++){
            //tell children to start
            write(ptoc[i][1], buf_out, 10); 

            //the time the order would be finished
            read (ctop[i][0],buf_in,10);
            
            //save the order endtime to the plant (pretty sure this is wrong cuz char and int)
            endtime[plant_now]=buf_in;
        }

        //figure out the which plant finishes first
        int min_plant=0;
        for (int i=1; i<no_plants; i++){
            if (endtime[i]<endtime[min_plant]){
                min_plant=i;
            }
        }

        //saves on the processed_order
        int index_now=free_index();
        char processed_order[min_plant][index_now]=next;

        //write the feedback to the plant of who gets to process it
        for (int i=0; i<no_plants; i++){
            if (i==min_plant){
                strcpy(buf_out, "y");
                write(ptoc[plant_now][1], buf_out, 10); 
            }
            else{
                strcpy(buf_out, "n");
                write(ptoc[plant_now][1], buf_out, 10); 
            }
        }
        
}

char next_process(){
    //edd algorithm
}

int free_index(){
    //finding out when is the next free space
}