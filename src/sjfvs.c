#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCHAR 200

struct day{
    int task;
    char order[MAXCHAR];
};

struct day plantX[MAXCHAR];
struct day plantY[MAXCHAR];
struct day plantZ[MAXCHAR];

void plant(int task, char order[], int order_quantity, int plant_limit, char order_name[]){
	printf("%d %d\n",order_quantity,plant_limit);
	if(order_quantity >= plant_limit){
		task = task + plant_limit;
		printf("%d\n",task);
	}
	else
		task = task + order_quantity;
        order_quantity = order_quantity - plant_limit;
        strcpy(order,order_name);
}

int sum(int arr[], int n) 
{ 
    int sum = 0; 
  
    for (int i = 0; i < n; i++) 
    	sum += arr[i]; 
  
    return sum; 
} 

int main(){
	FILE *fp;
        char str[MAXCHAR];
        char* filename = "orders.txt";
	char *token;
	int quantity[MAXCHAR];
	char order_name[MAXCHAR][MAXCHAR];
	int q = 0;
	int x_limit = 300;
	int y_limit = 400;
	int z_limit = 500;

	fp = fopen(filename, "r");
	if (fp == NULL){
        	printf("Could not open file %s",filename);
        	return 1;
    	}

	while (fgets(str, MAXCHAR, fp) != NULL){
    		token = strtok(str, " ");
		for(int i = 0;i < 5; i++) {
      			if(i == 3){
				quantity[q] = atoi(token);
				printf("%d ",quantity[q]);
			}
			else if ( i == 4 ){
				strcpy(order_name[q],token);
				printf("%s \n",order_name[q]);
			}
			token = strtok(NULL, " ");
   		}
		q++;
	}

	//struct day plantX[MAXCHAR];
	//struct day plantY[MAXCHAR];
    //struct day plantZ[MAXCHAR];
	
	int order = 0;
	int day = 0;
	// while(sum(quantity, q) != 0 ){
	plantX[day].task = 0;
    plantY[day].task = 0;
    plantZ[day].task = 0;
	strcpy(plantX[day].order,"");
	strcpy(plantY[day].order,"");
	strcpy(plantZ[day].order,"");
	// 	while(quantity[order] != 0){ 
	// 		if(plantZ[day].task != z_limit ){
                plant(plantZ[day].task, plantZ[day].order, quantity[order], z_limit, order_name[order]);
				printf("%d \n",plantZ[day].task);
	// 		}
	// 		else if(plantY[day].task != y_limit ){
    //                             plant(plantY[day].task, plantY[day].order, quantity[order], y_limit, order_name[order]);
    //                     }
	// 		else if(plantX[day].task != x_limit ){
    //                             plant(plantX[day].task, plantX[day].order, quantity[order], x_limit, order_name[order]);
    //                     }
	// 		else
	// 			break;
	// 		order++;
	// 	}
	// 	day++;
	// }
	//printf("%d\n",day);
	//for( int i = 0; i < day ; i++){
	//	printf("Plant X: %d %s \n", plantX[i].production, plantX[i].order);
	//	printf("Plant Y: %d %s \n", plantY[i].production, plantY[i].order);
	//	printf("Plant Z: %d %s \n", plantZ[i].production, plantZ[i].order);
	//}
    	fclose(fp);
    	return 0;

}
