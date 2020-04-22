#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>

pthread_t id_led;
pthread_mutex_t mutex_led;
pthread_cond_t cond_led;
int led_value = 0;

void *pthread_led(void *arg) {
    printf("pthread_ledis ok\n");
    while(1) {
        printf("pthread led\n");
        sleep(1);
    }
}

int main(int argc, char** argv) {
    int ret = 0;
    printf("gateway start!\n");
    
    ret = pthread_create(&id_led, 0, pthread_led, NULL);
    if (ret != 0) {
        perror("thread create");
        exit(-1);
    }
    pthread_join(id_led, NULL);
    return 0;
}