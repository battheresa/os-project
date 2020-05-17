#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char input[100];
    
    fgets(input, 100, stdin);
    printf("sub received %s", input);
    
    return 0;
}


