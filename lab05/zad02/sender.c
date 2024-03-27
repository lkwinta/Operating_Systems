#define _XOPEN_SOURCE 700 // hacky approach, might be necessary sometimes to enable sig* functions

#include <signal.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief simple handler for SIGUSR1 signal
 * 
 * @param signo number of the signal that was risen
*/
void SIGUSR1_handler(int signo) {
    printf("Confirmation received\n");
}

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - pid of the catcher program
 * argv[2] - mode that will be send to catcher
*/
int main(int argc, char** argv) {
    if(argc < 3) {
        printf("Usage: %s <signal_pid> <signal_argument>\n", argv[0]);
        return -1;
    }
    /* Printf current process PID */
    printf("Sender PID: %d\n", getpid());

    /* Register the feedback handler */
    signal(SIGUSR1, SIGUSR1_handler);

    /* Convert arguments to actual numbers */
    long signal_pid = strtol(argv[1], NULL, 10);
    long signal_argument = strtol(argv[2], NULL, 10);

    /* Store mode argument in proper union */
    union sigval value = {signal_argument};

    /* send signal to catcher process with given argument */
    sigqueue(signal_pid, SIGUSR1, value);
    printf("Signal sent with argument: %ld\n", signal_argument);

    /* mask signals so only SIGUSR1 and SIGINT can interrupt the suspend */
    sigset_t mask;
    sigfillset(&mask);

    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGINT);

    /* Wait for the feedback*/
    sigsuspend(&mask);
    return 0;
}