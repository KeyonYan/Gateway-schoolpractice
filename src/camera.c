/* Module: 视频模块
 * Author: 闫科宇
 * Date:   2020-06-06 17:15:59
 * Desc:   练习共享内存的使用
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include "logger.h"

#define BUFSZ 2048

// 用于映射共享内存的结构体
typedef struct {
	int num;
	char buf[1024];
}Data;

char logBuf[1024];

void *pthread_camera() {
	int shmid;
	int ret;
	key_t key;
	Data *shm;
    logger("INFO", "[Camera Module] >> \033[0;32m[...]");

	key = ftok(".", 2019);
	if (key == -1) {
		perror("ftok");
        pthread_exit(NULL);
	}
	// 创建共享内存
	shmid = shmget(key, BUFSZ, SHM_R|SHM_W|IPC_CREAT);
	if (shmid < 0) {
		perror("shmget");
		pthread_exit(NULL);
	}
	// 映射共享内存
	shm = shmat(shmid, NULL, 0);
	
	// 清空共享内存空间
	bzero(shm, BUFSZ);

	// 向共享内存中写入数据
    Data data;
	data.num = 2020;
	strcpy(data.buf, "hello yky");
	memcpy(shm, &data, sizeof(data));
    logger("INFO", "[Camera Module] Write to Share-Mem [Ok]");
    
    // 从共享内存中读取数据
    logger("INFO", "[Camera Module] Read from Share-Mem: ");
    sprintf(logBuf, "[Camera Module] shm->num: %d\tshm->buf: %s", shm->num, shm->buf);
    logger("INFO", logBuf);

    // 解除映射
    ret = shmdt(shm);
	if (ret < 0) {
		logger("ERROR", "[Camera Module] Share-Mem unmap fail.");
        pthread_exit(NULL);
	}
    // 删除共享内存
    ret = shmctl(shmid, IPC_RMID, NULL);
    if (ret < 0) {
        logger("ERROR", "[Camera Module] Share-Mem delete fail.");
        pthread_exit(NULL);
    }

    logger("INFO", "[Camera Module] >> [Close]");
	return NULL;
}