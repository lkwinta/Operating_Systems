#define _XOPEN_SOURCE 700 // hacky approach, might be necessary sometimes to enable sig* functions

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

/**
 * @brief Handler that updates current status based on argument get from signal
 * 
 * @param argument argument coming from signal
*/
void SIGUSR1_handler(int signal_number){
    printf("Received SIGUSR1 signal with number: %d\n", signal_number);
}

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - disposition, what to do with received signal
*/
int main(int argc, char** argv) {
     // if we didn't get 3 arguments we cannot proceed
    if(argc < 2) {
        printf("Usage: %s <disposition>\n", argv[0]);
        return -1;
    }

    /* 
        I have decided to put `raise` in all if statements to be consistant with 
        with raise in mask mode - we have to put raise in between instructions
        to demonstrate pending signal when mask is selected
    */

    if(!strcmp("ignore", argv[1])) {
        /* Ignore signal */
        signal(SIGUSR1, SIG_IGN);
        raise(SIGUSR1); // send signal to ourself
    }
    else if(!strcmp("handler", argv[1])) {
        /* Use defined handler as reaction to signal */
        signal(SIGUSR1, SIGUSR1_handler);
        raise(SIGUSR1); // send signal to ourself
    }
    else if(!strcmp("mask", argv[1])) {
        /* setup mask as signal set */
        sigset_t sigset;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGUSR1);

        /* mask signal */
        sigprocmask(SIG_SETMASK, &sigset, NULL);

        raise(SIGUSR1); // send signal to ourself

        /* Fetch all pending signals */
        sigset_t pending_signals;
        sigpending(&pending_signals);

        /* Check if our signal is in pending set */
        printf("Is signal pending? : %i\n", sigismember(&pending_signals, SIGUSR1));
    } else {
        /*Use default handler for the signal*/
        signal(SIGUSR1, SIG_DFL); 
        raise(SIGUSR1); // send signal to ourself
    } 
    return 0;
}