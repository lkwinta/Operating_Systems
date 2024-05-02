#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "shared_memory_specs.h"

volatile bool should_close = false;

/**
 * @brief Signal handler for closing client
 * 
 * @param signum - signal number
*/
void SIGNAL_handler(int signum) {
    should_close = true;
}

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - number of printers to spawn
*/
int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <number_of_printers>\n", argv[0]);
        return -1;
    } 
    // convert argument defining number of printers to spawn
    long number_of_printers = strtol(argv[1], NULL, 10);

    // check if number of printers is not too big
    if (number_of_printers > MAX_PRINTERS) {
        printf("Number of printers is too big, maximum number of printers is %d\n", MAX_PRINTERS);
        return -1;
    }

    /* Try to create and open file descriptor for shared memory region */
    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR);
    if(memory_fd < 0)
        perror("shm_open");

    /* Specify shared memory size */
    if(ftruncate(memory_fd, sizeof(memory_map_t)) < 0)
        perror("ftruncate");

    /* try to map shared memory region to program address space */
    memory_map_t* memory_map = mmap(NULL, sizeof(memory_map_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (memory_map == MAP_FAILED)
        perror("mmap");

    /* Clean shared memory region */
    memset(memory_map, 0, sizeof(memory_map_t));

    /* Set number of accessible printers */
    memory_map->number_of_printers = number_of_printers;

    // register signal handler for closing client to all signals
    for (int sig = 1; sig < SIGRTMAX; sig++) {
        signal(sig, SIGNAL_handler);
    }

    /* spawn all the printers */
    for (int i = 0; i < number_of_printers; i++){
        /* initialize unnamed semaphores */
        sem_init(&memory_map->printers[i].printer_semaphore, 1, 1);

        pid_t printer_pid = fork();
        if(printer_pid < 0) {
            perror("fork");
            return -1;
        }
        else if(printer_pid == 0) {
            /* Printer process - wait for printing request and print data */
            while(!should_close) {
                /* if user has signalized that printer should print, start printing data from the buffer */
                if (memory_map->printers[i].printer_state == PRINTING) {

                    /* print data from the buffer with 1s delay after each character*/
                    for (int j = 0; j < memory_map->printers[i].printer_buffer_size; j++) {
                        printf("%c", memory_map->printers[i].printer_buffer[j]);
                        sleep(1);
                    }

                    /* add newline and flush data to be displayed on the screen */
                    printf("\n");
                    fflush(stdout);
                    
                    /* reset printer state */
                    memory_map->printers[i].printer_state = WAITING;

                    /* increment semaphore to signalize that printing has been done */
                    sem_post(&memory_map->printers[i].printer_semaphore);
                }
            }
            exit(0);
        }
    }

    /* wait for all printers to finish */
    while(wait(NULL) > 0) {}

    /* destroy semaphores */
    for (int i = 0; i < number_of_printers; i++)
        sem_destroy(&memory_map->printers[i].printer_semaphore);


    /* clean up */
    munmap(memory_map, sizeof(memory_map_t));

    /* close shared memory file descriptor */
    shm_unlink(SHARED_MEMORY_DESCRIPTOR_NAME);
}