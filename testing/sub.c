#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    
    printf("sub.c says %s\n", argv[1]);
    return 0;
}


