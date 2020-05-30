/* Module: 传感器模块
 * Author: 闫科宇
 * Date:   2020-05-30 18:53:42
 * Desc:   读取传感器数据，存储并显示
 *         (由于没有硬件设备，这里创建一个设备文件当作传感器，对其进行读写)
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlite_service.h"
#include "sensor.h"
#include "logger.h"

// 线程入口函数
void *pthread_sensor() {
    logger("INFO", "[Sensor Module] >> \033[0;32m[...]");


    logger("INFO", "[Sensor Module] >> \033[0;32m[Ok]");
    return NULL;
}