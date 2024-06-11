#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_COUNT 10

int counter = 0;
pthread_mutex_t counter_lock;

void *reader_thread(void *arg) {
    printf("Start reader.\n");

//zablokuj mutex counter_lock


    int temp = counter;
    sleep(1);

//odblokuj mutex counter_lock


    printf("Read %d.\n", temp);
    return 0;
}

void *writer_thread(void *arg) {
    printf("Start writer.\n");

//zablokuj mutex


    int temp = counter;
    temp += 1;
    sleep(1);
    counter = temp;

//odblokuj mutex


    printf("Written %d.\n", temp);
    return 0;
}

int main(int argc, char* argv[]) {
    pthread_t threads[THREAD_COUNT];

//zainicjuj mutex counter_lock


    for (int i = 0; i < THREAD_COUNT; i++) {
        int result;
        if ((i % 3) == 0) {
          result = pthread_create(&(threads[i]), NULL, writer_thread, NULL);
        } else {
          result = pthread_create(&(threads[i]), NULL, reader_thread, NULL);
        }
        if (result != 0) {
            perror("Could not create thread.");
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
//usuń mutex counter_lock

    printf("\nCounter value after executing %d threads is %d.\n", THREAD_COUNT, counter);
    return 0;
}
