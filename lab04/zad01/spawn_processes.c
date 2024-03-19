#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
     // if we didn't get 2 arguments we cannot procceed
    if(argc < 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return -1;
    }

    long number_of_processes = strtol(argv[1], NULL, 10);
    if(number_of_processes < 0){
        printf("Number of processes must be a positive number\n");
        return -1;
    }

    for(int i = 0; i < number_of_processes; i++) {
        pid_t pid = fork();
        if(pid == 0) {
            printf("Child process pid %d\n", getpid());
            printf("Parent process pid %d\n", getppid());
            exit(0);
        }
    }
    
    while(wait(NULL) > 0);
    printf("Number of processes: %ld\n", number_of_processes);
    
    return 0;
}