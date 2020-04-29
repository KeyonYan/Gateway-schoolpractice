#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include "server.h"

pthread_t id_led;
pthread_mutex_t mutex_led;
pthread_cond_t cond_led;
int led_value = 0;

pthread_t id_network;

void *pthread_led(void *arg) {
    printf("pthread_ledis ok\n");
    while(1) {
        pthread_mutex_lock(&mutex_led); // 加锁，保护cond条件
        pthread_cond_wait(&cond_led, &mutex_led); // 阻塞当前线程
        pthread_mutex_unlock(&mutex_led); // 解锁
        printf("pthread led\n");
        sleep(1);
    }
}

int main(int argc, char** argv) {
    int ret = 0;
    printf("gateway start!\n");
    pthread_mutex_init(&mutex_led, NULL);
    pthread_cond_init(&cond_led, NULL);

    ret = pthread_create(&id_led, 0, pthread_led, NULL);
    if (ret != 0) {
        perror("thread create");
        exit(-1);
    }
    ret = pthread_create(&id_network, 0, pthread_network, NULL);
    if (ret != 0) {
        perror("thread create");
        exit(-1);
    }
    while(1) {
        pthread_mutex_lock(&mutex_led);
        led_value++;
        printf("main thread: [%d]\n", led_value);
        if (led_value % 5 == 0) {
            pthread_cond_signal(&cond_led); // 满足条件 线程唤醒
        }
        pthread_mutex_unlock(&mutex_led);
        sleep(1);
    }
    pthread_join(id_led, NULL);
    pthread_mutex_destroy(&mutex_led);
    pthread_cond_destroy(&cond_led);
    return 0;
}