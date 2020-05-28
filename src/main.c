#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/msg.h>
#include <assert.h>
#include "logger.h"
#include "server.h"
#include "cmdparser.h"
#include "sqlite_service.h"
#include "msgqueue.h"

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
    assert(ret >= 0);
    pthread_detach(id);
}

/* INIT THREADS */
void initThread() {
    // 启动网络线程
    pthread_create1(id_network, pthread_network);
    // 启动命令解析线程
    pthread_create1(id_cmdparser, pthread_cmdparser);
    // 启动数据存储线程
    pthread_create1(id_sqlite, pthread_sqlite);
    

    // pthread_mutex_init(&mutex_led, NULL);
    // pthread_cond_init(&cond_led, NULL);
    // // 启动LED线程
    // pthread_create1(id_led, pthread_led);
    // while(1) {
    //     pthread_mutex_lock(&mutex_led);
    //     led_value++;
    //     printf("main thread: [%d]\n", led_value);
    //     if (led_value % 5 == 0) {
    //         pthread_cond_signal(&cond_led); // 满足条件 线程唤醒
    //     }
    //     pthread_mutex_unlock(&mutex_led);
    //     sleep(1);
    // }
    // pthread_join(id_led, NULL);
    // pthread_mutex_destroy(&mutex_led);
    // pthread_cond_destroy(&cond_led);
}

/**
 * 资源释放处理
 */
void onDestroy() {
    int ret;
    ret = mqRemove();
    if (ret == 0) logger("INFO", "[Main Thread] MQ Remove \033[0;32m[Ok]");
}

/* SIGNAL HANDLER */
void signal_handler(int signo)
{
    char buf[100];
	if(signo==SIGINT) {
        logger("INFO", "[Main Module] >> onDestroy\033[0;32m[...]");
        
        // do something
        onDestroy();
        
        logger("INFO", "[Main Module] >> onDestroy\033[0;32m[Ok]");
        exit(1);
    } else {    
        sprintf(buf, "[Main Module] >> catch other signal: %d", signo);
        logger("WARNNING", buf);
    }
}

/* MAIN FUNCTION */
int main(int argc, char** argv) {
    logger("INFO", "[MainThread] >> GATEWAY System \033[0;32m[...]");
    
    signal(SIGINT, signal_handler);
    initThread();
    int mq_id = mqCreate();
    if (mq_id >= 0)
        logger("INFO", "[MainThread] MQ Create[Ok]");
    else
        logger("ERROR", "[MainThread] MQ Create[Fail]");

    logger("INFO", "[MainThread] >> GATEWAY System \033[0;32m[Start]");

    while(1);

    onDestroy();
    return 0;
}