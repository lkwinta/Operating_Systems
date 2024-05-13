#define _XOPEN_SOURCE 700

#include <signal.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


void sighandler(int signo, siginfo_t *info, void *extra) {
    printf("Wartosc: %d\n", info->si_int);
}


int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    sigaction(SIGUSR1, &action, NULL);
    
    int child = fork();
    if(child == 0) {
        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
        

        sigset_t sigset;
        sigfillset(&sigset);
        sigdelset(&sigset, SIGUSR1);

        sigprocmask(SIG_SETMASK, &sigset, NULL);
    }
    else {
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
        long signal = strtol(argv[2], NULL, 10);
        long arg = strtol(argv[1], NULL, 10);

        union sigval val = {arg};

        sigqueue(child, signal, val);
        
        int status;
        wait(&status);
        printf("exit: %d\n", WEXITSTATUS(status));
    }

    return 0;
}
