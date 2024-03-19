#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - number of processes to spawn
*/
int main(int argc, char** argv) {
     // if we didn't get 2 arguments we cannot procceed
    if(argc < 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return -1;
    }

    /* convert argument into long value */
    long number_of_processes = strtol(argv[1], NULL, 10);

    /* ensure that converted number is greater than 0 */
    if(number_of_processes < 0){
        printf("Number of processes must be a positive number\n");
        return -1;
    }

    /* spawn child processes with fork */
    for(int i = 0; i < number_of_processes; i++) {
        pid_t pid = fork();

        if(pid == 0) {
            /* execute child's code */

            printf("Child process pid %d\n", getpid());
            printf("Parent process pid %d\n", getppid());

            /* exit with successful return code */
            exit(0);
        }
    }
    
    /* wait for all child processes to end*/
    while(wait(NULL) > 0);

    /* print number of processes*/
    printf("Number of processes: %ld\n", number_of_processes);
    
    return 0;
}