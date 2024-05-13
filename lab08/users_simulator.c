#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
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
 * @brief Generate random string of given length, function will append \0 at the end of string
 * 
 * @param buffer - buffer to store generated string
 * @param length - length of generated string
*/
void generate_random_string(char* buffer, int length) {
    for(int i = 0; i < length; i++) {
        buffer[i] = 'a' + rand() % 26;
    }
    buffer[length] = '\0';
}

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - number of users to spawn
*/
int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <number_of_users>\n", argv[0]);
        return -1;
    }

    /* convert argument defining number of users to spawn */
    long number_of_users = strtol(argv[1], NULL, 10);

    /* open descriptor defining shared memory that will be mapped */
    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if(memory_fd < 0)
        perror("shm_open");

    /* try to map shared memory region to program address space with READ and WRITE permission with flag that memory will be shared */
    memory_map_t* memory_map = mmap(NULL, sizeof(memory_map_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (memory_map == MAP_FAILED)
        perror("mmap");

    /* Define buffer for random string */
    char user_buffer[MAX_PRINTER_BUFFER_SIZE] = {0};

    // register signal handler for closing client to all signals
    for (int sig = 1; sig < SIGRTMAX; sig++) {
        signal(sig, SIGNAL_handler);
    }


    int seed = 0;
    // spawn users
    for (int i = 0; i < number_of_users; i++){
        seed += 10;
        pid_t user_pid = fork(); 
        if (user_pid < 0) { /* Failed to spawn all users */
            perror("fork");
            return -1;
        }
        else if(user_pid == 0) {
            srand(seed);
            /* Try to dispatch data to the printers in random manner */
            while(!should_close) {
                /* Generate random message */
                generate_random_string(user_buffer, 10);

                /**
                 * try finding printer that is not busy at the moment
                */
                int printer_index = -1;
                for (int j = 0; j < memory_map->number_of_printers; j++) {
                    int val;
                    sem_getvalue(&memory_map->printers[j].printer_semaphore, &val);
                    if(val > 0) {
                        printer_index = j;
                        break;
                    }
                }

                /**
                 * if all printers are busy, dispatch work to random printer to uniformly distribute work
                */
                if(printer_index == -1)
                    printer_index = rand() % memory_map->number_of_printers;

                /* try to decrement semaphore, blocks program if the printer is currently busy */
                if(sem_wait(&memory_map->printers[printer_index].printer_semaphore) < 0)
                    perror("sem_wait");

                /* copy data to printer buffer */
                memcpy(memory_map->printers[printer_index].printer_buffer, user_buffer, MAX_PRINTER_BUFFER_SIZE);
                memory_map->printers[printer_index].printer_buffer_size = strlen(user_buffer);

                /* set printer state to printing */
                memory_map->printers[printer_index].printer_state = PRINTING;

                printf("User %d is printing on printer %d\n", i, printer_index);
                fflush(stdout);

                /* sleep for random amount of time before trying to dispatch new work */
                sleep(rand() % 3 + 1);
            }
            exit(0);         
        }
    }

    // wait for all children to finish
    while(wait(NULL) > 0) {};

    // unmap memory map
    munmap(memory_map, sizeof(memory_map_t));
}