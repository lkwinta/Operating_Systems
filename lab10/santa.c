#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t renifer_mutex[9] = {
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

int renifers_back_count = 0;
pthread_mutex_t renifers_back_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t santa, renifers[9];

void* renifer_thread(void* arg){
    int id = *(int*)arg;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    pthread_mutex_lock(&renifer_mutex[id]);
    while(1) {
        sleep(rand() % 6 + 5);

        pthread_mutex_lock(&renifers_back_mutex);
        printf("Renifer: czeka %d reniferów, %d\n", renifers_back_count, id);
        renifers_back_count++;
        if (renifers_back_count == 9) {
            pthread_cond_signal(&santa_cond);
            printf("Renifer: budzę Mikołaja, %d\n", id);
            renifers_back_count = 0;
        }
        pthread_mutex_unlock(&renifers_back_mutex);

        pthread_mutex_lock(&renifer_mutex[id]);
        printf("Renifer: lecę na wakacje, %d\n", id);
    }  

    return NULL;
}

void* santa_thread(void* arg) {
    for (int i = 0; i < 4; i++) {
        pthread_cond_wait(&santa_cond, &santa_mutex);
        printf("Mikołaj: budzę się\n");

        printf("Mikołaj: dostarczam zabawki\n");
        sleep(rand() % 3 + 2);
        for (int j = 0; j < 9; j++) {
            pthread_mutex_unlock(&renifer_mutex[j]);
        }

        printf("Mikołaj: zasypiam\n");
    }

    for (int j = 0; j < 9; j++) {
        pthread_cancel(renifers[j]);
    }

    return NULL;
}

int main() {
    int ids[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    pthread_create(&santa, NULL, santa_thread, NULL);
    for (int i = 0; i < 9; i++) {
        pthread_create(&renifers[i], NULL, renifer_thread, &ids[i]);
    }

    pthread_join(santa, NULL);
    for (int i = 0; i < 9; i++) {
        pthread_join(renifers[i], NULL);
    }

    printf("Koniec\n");

    return 0;
}