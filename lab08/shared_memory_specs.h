#ifndef SHARED_MEMORY_SPEC_H
#define SHARED_MEMORY_SPEC_H

#define SHARED_MEMORY_DESCRIPTOR_NAME "printer_system_shared_memory"
#include <semaphore.h>

#define MAX_PRINTERS 256
#define MAX_PRINTER_BUFFER_SIZE 256
#define MAX_SEMAPHORE_NAME 40

typedef enum {
    WAITING = 0,
    PRINTING = 1
} printer_state_t;

typedef struct {
    sem_t printer_semaphore;
    char printer_buffer[MAX_PRINTER_BUFFER_SIZE];
    size_t printer_buffer_size;
    printer_state_t printer_state;
} printer_t;

typedef struct {
    printer_t printers[MAX_PRINTERS];
    int number_of_printers;
} memory_map_t;

#endif //SHARED_MEMORY_SPEC_H