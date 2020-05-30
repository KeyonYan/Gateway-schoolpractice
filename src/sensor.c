/* Module: 传感器模块
 * Author: 闫科宇
 * Date:   2020-05-30 18:53:42
 * Desc:   读取传感器数据，存储并显示
 *         (由于没有硬件设备，这里创建一个设备文件当作传感器，对其进行读写)
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "sqlite_service.h"
#include "sensor.h"
#include "logger.h"

// 设备节点（需先在GateWayYKY/kernel里调用make install安装内核驱动，并mknod 创建设备节点）
#define CHAR_DEV_NAME "/dev/sensor_dev"  

int fd;
char buf[32];
char logBuf[512];

void openDevice() {
    // 打开设备文件
	fd = open(CHAR_DEV_NAME, O_RDONLY | O_NDELAY);
	if(fd < 0) {
		logger("ERROR","[Sensor Module] Open sensor device failed!");
        pthread_exit(NULL);
    }
}

void readDevice() {
    // 读取传感器值
    read(fd, buf, 2);
    sprintf(logBuf, "[Sensor Module] Temperature: %d", buf[0]);
    logger("INFO", logBuf);
    sprintf(logBuf, "[Sensor Module] Humidity: %d", buf[1]);
    logger("INFO", logBuf);
}

// 线程入口函数
void *pthread_sensor() {
    logger("INFO", "[Sensor Module] >> \033[0;32m[...]");

    openDevice();
    while(1) { /* 每3s读取一次传感器 */
        readDevice();
        sleep(3);
    }

    logger("INFO", "[Sensor Module] >> \033[0;32m[Ok]");
    return NULL;
}