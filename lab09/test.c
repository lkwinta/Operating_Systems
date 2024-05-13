#define _XOPEN_SOURCE 700
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void a (int signo) {

}

void* thread_function(void* arg) {
    

    printf("alive!!!\n");
    while(1) {
        pause();
        printf("runnin!!!\n");
        
    }
}

int main() {
    sigaction(SIGINT, &(struct sigaction){.sa_handler = a}, NULL);
    //signal(SIGINT, a);  
    
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    

    sleep(2);

    pthread_kill(thread, SIGINT);

    sleep(2);
    pthread_kill(thread, SIGINT);
    sleep(2);
    fflush(stdout);
    
}