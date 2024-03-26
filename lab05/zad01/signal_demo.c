#define _POSIX_SOURCE

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

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

    if(!strcmp("ignore", argv[1])) {
        signal(SIGUSR1, SIG_IGN);
        raise(SIGUSR1);
    }
    else if(!strcmp("handler", argv[1])) {
        signal(SIGUSR1, SIGUSR1_handler);
        raise(SIGUSR1);
    }
    else if(!strcmp("mask", argv[1])) {
        sigset_t sigset;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGUSR1);

        sigprocmask(SIG_SETMASK, &sigset, NULL);

        raise(SIGUSR1);

        sigset_t pending_signals;
        sigpending(&pending_signals);
        printf("Is signal pending? : %i\n", sigismember(&pending_signals, SIGUSR1));
    } else {
        signal(SIGUSR1, SIG_DFL);
        raise(SIGUSR1);
    } 
    return 0;
}