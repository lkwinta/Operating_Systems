#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

int status = -1;
int status_chagnes = 0;

void argument_handler(int argument){
    status_chagnes++;
    status = argument;
}

void SIGUSR1_action(int signo, siginfo_t *info, void *extra)
{
    int int_val = info->si_value.sival_int;
    printf("Received value: %d pid: %d\n", int_val, info->si_pid);

    kill(info->si_pid, SIGUSR1);
}

int main() {
    printf("Catcher PID: %d\n", getpid());

    struct sigaction action;
    action.sa_sigaction = SIGUSR1_action;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    sigaction(SIGUSR1, &action, NULL);

    while(1) {
        switch(status){
            case 1:
                for(int i = 1; i <= 100; i++){
                    printf("%i, ", i);
                }
                printf("\n");
                status = -1;
            case 2:
                printf("So far status has changed %d times\n", status_chagnes);
                status = -1;
            case 3:
                printf("Received exit signal! Exiting...\n");
                exit(0);
                break;
            default:
                break; //no work to do
        }
    } 

    return 0;
}