#define _XOPEN_SOURCE 700

#include <signal.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int signo) {
    printf("Confitmation received\n");
}

int main(int argc, char** argv) {
    if(argc < 3) {
        printf("Usage: %s <signal_pid> <signal_argument>\n", argv[0]);
        return -1;
    }
    printf("Sender PID: %d\n", getpid());
    signal(SIGUSR1, handler);

    long signal_pid = strtol(argv[1], NULL, 10);
    long signal_argument = strtol(argv[2], NULL, 10);

    kill(signal_pid, SIGUSR1);

    union sigval value;
    value.sival_int = signal_argument;

    sigqueue(signal_pid, SIGUSR1, value);
    printf("Signal sent: %ld\n", signal_argument);

    sigset_t mask;
    sigfillset(&mask);

    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGINT);

    sigsuspend(&mask);
    return 0;
}