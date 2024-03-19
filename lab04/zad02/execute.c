#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// declare global variable
int global = 0;

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - directory path for ls command
*/
int main(int argc, char** argv) {
     // if we didn't get 3 arguments we cannot procceed
    if(argc < 2) {
        printf("Usage: %s <path to directory>\n", argv[0]);
        return -1;
    }

    /* print program name */
    printf("%s \n", strrchr(argv[0], '/') + 1);

    /* declar local variable */
    int local = 0;

    /* spawn child process for ls execution */
    pid_t pid = fork();

    if (pid == 0) {
        /* Execute childs code */
        printf("child process\n");
        global++;
        local++;

        printf("child pid = %d, parent pid = %d \n", getpid(), getppid());
        printf("child's local = %d, child's global = %d \n", local, global);

        /* ecxecute ls command and get error code */
        int status = execl("/bin/ls", "ls", "-l", argv[1], NULL);

        /* exit from child with ls return code */
        exit(status);
    } 

    /* wait for end of child execution and get error code */
    int status = 0;
    wait(&status);
    int child_return_status = WEXITSTATUS(status);

    /* print parent process */
    printf("parent process\n");
    printf("parent pid = %d, child pid = %d \n", getpid(), pid);
    printf("Child exit code: %d \n", child_return_status);
    printf("parent's local = %d, parent's global = %d \n", local, global);

    /* return child error code*/
    return child_return_status;
}