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

#include "shared_memory_specs.h"


void generate_random_string(char* buffer, int length) {
    for(int i = 0; i < length; i++) {
        buffer[i] = 'a' + rand() % 26;
    }
    buffer[length] = '\0';
}

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - number of users
*/
int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <number_of_users>\n", argv[0]);
        return -1;
    }

    long number_of_users = strtol(argv[1], NULL, 10);

    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR,  S_IRUSR | S_IWUSR);
    if(memory_fd < 0)
        perror("shm_open");

    memory_map_t* memory_map = mmap(NULL, sizeof(memory_map_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (memory_map == MAP_FAILED)
        perror("mmap");

    char user_buffer[MAX_PRINTER_BUFFER_SIZE] = {0};

    for (int i = 0; i < number_of_users; i++){
        //pid_t user_pid = fork(); 
        //if(user_pid == 0) {
            while(1) {
                generate_random_string(user_buffer, 10);

                // critical section
                int max_sem_value = INT32_MIN;
                int max_sem_index = -1;

                // for (int j = 0; j < memory_map->number_of_printers; j++) {
                //     int sem_value;
                //     if(sem_getvalue(memory_map->printers[j].printer_semaphore, &sem_value) < 0) {
                //         perror("sem_getvalue");
                //     }
                //     if(sem_value > max_sem_value) {
                //         max_sem_value = sem_value;
                //         max_sem_index = j;
                //     }
                // }
                
                // if(max_sem_index == -1) {
                //     printf("No available printers\n");
                //     max_sem_index = rand() % memory_map->number_of_printers;
                // }

                if(sem_wait(memory_map->printers[0].printer_semaphore) < 0){
                    perror("asdadads");
                }
                memcpy(memory_map->printers[0].printer_buffer, user_buffer, MAX_PRINTER_BUFFER_SIZE);
                memory_map->printers[0].printer_state = PRINTING;
                printf("User %d is printing on printer %d\n", i, 0);

                sleep(rand() % 3 + 1);
            } 
            exit(0);
        //}
    }

    // wait for all children to finish
    while(wait(NULL) > 0) {};

    munmap(memory_map, sizeof(memory_map_t));
}