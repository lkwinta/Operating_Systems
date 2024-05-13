#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Declare global variables for easy access between threads
*/

/* mutex and condition variable to wake up santa */
pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;

/* declare mutexes for all reindeers */
pthread_mutex_t reindeers_mutexes[9] = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};

/* declare mutex and counter to count reindeers that has come back */
int reindeers_back_count = 0;
pthread_mutex_t reindeers_back_mutex = PTHREAD_MUTEX_INITIALIZER;

/* declare thread handles for santa and reindeer threads*/
pthread_t santa_thread;
pthread_t reindeers_threads[9];

/**
 * @brief Function that is executed by reindeer threads
 * 
 * @param arg - id of the reindeer
 * 
 * @return NULL
*/
void* reindeer_thread_handler(void* arg){
    // dereference argument pointer to get reindeer id
    int id = *(int*)arg;
    // set cancel type to asynchronous to be able to cancel thread immediately
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    // lock reindeer mutex to prevent cycling until reindeers do not come back from santa claus - when santa releases their mutexes
    pthread_mutex_lock(&reindeers_mutexes[id]);
    while(1) {
        // reindeer is on vacation for 5-10 seconds
        sleep(rand() % 6 + 5);

        // lock counter mutex to increment counter of reindeers that has come back
        pthread_mutex_lock(&reindeers_back_mutex);
        printf("Renifer: czeka %d reniferów, %d\n", reindeers_back_count, id);

        // increment counter and check if all reindeers are back
        reindeers_back_count++;
        if (reindeers_back_count == 9) {
            printf("Renifer: budzę Mikołaja, %d\n", id);
            // if all reindeers are back, wake up santa and reset counter
            pthread_cond_signal(&santa_cond);
            reindeers_back_count = 0;
        }

        // unlock counter mutex
        pthread_mutex_unlock(&reindeers_back_mutex);

        // wait for santa to release reindeer mutex
        pthread_mutex_lock(&reindeers_mutexes[id]);

        // reindeer is flying on vacation
        printf("Renifer: lecę na wakacje, %d\n", id);
    }  

    return NULL;
}

void* santa_thread_handler(void* arg) {
    // cycle 4 times to deliver toys
    for (int i = 0; i < 4; i++) {
        // wait for reindeers to wake up santa
        pthread_cond_wait(&santa_cond, &santa_mutex);
        printf("Mikołaj: budzę się\n");

        printf("Mikołaj: dostarczam zabawki\n");
        // santa is delivering toys for 2-4 seconds
        sleep(rand() % 3 + 2);

        // release all reindeers
        for (int j = 0; j < 9; j++) {
            pthread_mutex_unlock(&reindeers_mutexes[j]);
        }

        printf("Mikołaj: zasypiam\n");
    }

    // cancel all reindeer threads
    for (int j = 0; j < 9; j++) {
        pthread_cancel(reindeers_threads[j]);
    }

    return NULL;
}

int main() {
    int ids[9];
    // create santa and reindeer threads
    pthread_create(&santa_thread, NULL, santa_thread_handler, NULL);
    for (int i = 0; i < 9; i++) {
        ids[i] = i;
        pthread_create(&reindeers_threads[i], NULL, reindeer_thread_handler, &ids[i]);
    }

    // wait for santa and reindeer threads to finish
    pthread_join(santa_thread, NULL);
    for (int i = 0; i < 9; i++) {
        pthread_join(reindeers_threads[i], NULL);
    }

    printf("Koniec\n");

    return 0;
}