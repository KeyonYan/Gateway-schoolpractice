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
#include "cmdparser.h"
#include "logger.h"
#include "sqlite_service.h"

// Sample pthread and mutex
pthread_t id_led;
pthread_mutex_t mutex_led;
pthread_cond_t cond_led;
int led_value = 0;
void* pthread_led(void* arg) {
    printf("pthread_ledis ok\n");
    while(1) {
        pthread_mutex_lock(&mutex_led); // 加锁，保护cond条件
        pthread_cond_wait(&cond_led, &mutex_led); // 阻塞当前线程
        pthread_mutex_unlock(&mutex_led); // 解锁
        printf("pthread led\n");
        sleep(1);
    }
}


/* THREAD MODULES */
pthread_t id_network;   // [线程] 网络模块
pthread_t id_cmdparser; // [线程] 命令解析模块
pthread_t id_sqlite;    // [线程] 数据存储模块
pthread_t id_file;      // [线程] 文件模块
pthread_t id_camera;    // [线程] 视频模块
pthread_t id_alarm;     // [线程] 警报模块
pthread_t id_sesor;     // [线程] 传感器模块

void pthread_create1(pthread_t id, void *(*__start_routine) (void *)) {
    int ret = 0;
    ret = pthread_create(&id, 0, __start_routine, NULL);
    if (ret != 0) {
        perror("thread create");
        exit(-1);
    }
}

/* INIT THREADS */
void initThread() {
    pthread_mutex_init(&mutex_led, NULL);
    pthread_cond_init(&cond_led, NULL);
    // 启动LED线程
    pthread_create1(id_led, pthread_led);
    // 启动网络线程
    pthread_create1(id_network, pthread_network);
    // 启动命令解析线程
    pthread_create1(id_cmdparser, pthread_cmdparser);
    // 启动数据存储线程
    pthread_create1(id_sqlite, pthread_sqlite);
    
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
}

/* MAIN FUNCTION */
int main(int argc, char** argv) {
    logger("INFO", ">> GATEWAY System \033[0;32m[Start]");
    
    initThread();

    return 0;
}