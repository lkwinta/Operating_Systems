#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "shared_memory_specs.h"

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - number of printers
*/
int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <number_of_printers>\n", argv[0]);
        return -1;
    } 

    long number_of_printers = strtol(argv[1], NULL, 10);

    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR);
    if(memory_fd < 0)
        perror("shm_open");

    if(ftruncate(memory_fd, sizeof(memory_map_t)) < 0)
        perror("ftruncate");


    memory_map_t* memory_map = mmap(NULL, sizeof(memory_map_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (memory_map == MAP_FAILED)
        perror("mmap");

    memory_map->number_of_printers = number_of_printers;

    for (int i = 0; i < number_of_printers; i++){

        memory_map->printers[i].printer_semaphore = sem_open("printer_semaphore", O_CREAT, S_IRUSR | S_IWUSR, 1);
        if(memory_map->printers[i].printer_semaphore == SEM_FAILED)
            perror("sem_open");

        printf("Printer %d created\n", i);
        printf("Printer semaphore: %p\n", memory_map->printers[i].printer_semaphore);

        //pid_t printer_pid = fork();
        ///if(printer_pid == 0) {
            while(1) {
                if (memory_map->printers[i].printer_state == PRINTING) {
                    for (int j = 0; j < MAX_PRINTER_BUFFER_SIZE; j++) {
                        printf("%c", memory_map->printers[i].printer_buffer[j]);
                        sleep(1);
                    }

                    memory_map->printers[i].printer_state = WAITING;
                    sem_post(memory_map->printers[i].printer_semaphore);
                }
            }
        //}
    }

    while(wait(NULL) > 0) {}

    munmap(memory_map, sizeof(memory_map_t));
    shm_unlink(SHARED_MEMORY_DESCRIPTOR_NAME);
}