/* Module: LED模块
 * Author: 闫科宇
 * Date:   2020-06-03 15:21:22
 * Desc:   LED变量的开关控制
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "led.h"
#include "logger.h"

pthread_t led_pid;
pthread_mutex_t led_mutex;
pthread_cond_t led_cond;
int led_status;

void *pthread_led();

void init_led() {
    int ret = 0;
    pthread_mutex_init(&led_mutex, NULL);
    pthread_cond_init(&led_cond, NULL);
    ret = pthread_create(&led_pid, 0, pthread_led,NULL);
    if (ret != 0) {
        logger("ERROR", "[LED Module] Thread Create Fail.");
        exit(-1);
    }
}

void destroy_led() {
    pthread_mutex_destroy(&led_mutex);
    pthread_cond_destroy(&led_cond);
}

void led_on() {
    pthread_mutex_lock(&led_mutex);
    led_status = 1;
    pthread_mutex_unlock(&led_mutex);
    logger("INFO", "[LED Module] LED is Open");
    
    pthread_cond_wait(&led_cond, &led_mutex);
}

void led_off() {
    pthread_mutex_lock(&led_mutex);
    led_status = 0;
    pthread_mutex_unlock(&led_mutex);
    logger("INFO", "[LED Module] LED is Close");

    pthread_cond_wait(&led_cond, &led_mutex);
}

void *pthread_led() {
    logger("INFO", "[LED Module] >> \033[0;32m[Ok]");
    
    while(1) {
        pthread_mutex_lock(&led_mutex);
        pthread_cond_wait(&led_cond, &led_mutex);
        
        if (led_status == 0) {
            logger("INFO", "[LED Module] LED [OFF]");
        } else {
            logger("INFO", "[LED Module] LED [O N]");
        }

        pthread_mutex_unlock(&led_mutex);
    }
}