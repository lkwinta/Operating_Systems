#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

/*
 * Funkcja 'create_shared_mem' powinna zaalokować segment pamięci współdzielonej
 * o rozmiarze 'size' z nazwą 'name' i zwrócić jego adres.
 * Segment ma umożliwiać odczyt i zapis danych.
 */
void* create_shared_mem(const char* name, off_t size) {
    int memory_fd = shm_open(name, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR);
    if(memory_fd < 0)
        perror("shm_open");

    if(ftruncate(memory_fd, size) < 0)
        perror("ftruncate");

    void* memory_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);

    if(memory_ptr == NULL)
        perror("mmap");

    return memory_ptr;
}


int main(void) {
    pid_t pid = fork();
    if (pid != 0) {
        char* buf = create_shared_mem("/data", 100);
        float* fbuf = (float*) buf;
        wait(NULL);
        printf("Values: %.3f %e %.3f\n", fbuf[0], *(float*)(buf + 37), fbuf[10]);
        munmap(buf, 100);
    } else {
        void* buf = (float*) create_shared_mem("/data", 40);
        memset(buf, 0xBF, 40); 
        munmap(buf, 40);
    }
    shm_unlink("/data");
}
