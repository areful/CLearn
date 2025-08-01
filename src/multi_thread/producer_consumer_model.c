#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>   // 包含 nanosleep 函数头文件

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
sem_t empty, full;
pthread_mutex_t mutex;

int in = 0;
int out = 0;

void *producer(void *arg) {
    int item;
    struct timespec delay;
    for (int i = 0; i < 20; ++i) {
        item = rand() % 100;  // 产生随机数
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        // 休眠500毫秒
        delay.tv_sec = 0;
        delay.tv_nsec = 500000000;  // 500ms
        nanosleep(&delay, NULL);
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    struct timespec delay;
    for (int i = 0; i < 20; ++i) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        item = buffer[out];
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        // 休眠1秒
        delay.tv_sec = 1;
        delay.tv_nsec = 0;
        nanosleep(&delay, NULL);
    }
    return NULL;
}

int main() {
    pthread_t producerThread, consumerThread;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
