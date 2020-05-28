/* Module: 命令解析模块
 * Author: 闫科宇
 * Date:   2020-04-29 23:10:38
 * Desc:   从消息队列中获取命令数据进行解析，根据解析结果调度不同模块
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "msgqueue.h"
#include "cmdparser.h"
#include "logger.h"

// 命令解析模块-线程主入口
void* pthread_cmdparser() {
    logger("INFO", "[CmdParser Module] >> \033[0;32m[...]");
    MSG msg;
    char logBuf[1024];
    int size;

    while(1) {
        size = mqRead(&msg, 0);
        if (size <= 0) continue;
        sprintf(logBuf, "[CmdParser Module] Read a message from MQ: %s", msg.mtext);
        logger("INFO", logBuf);

        sleep(3);
    }

    logger("INFO", "[CmdParser Module] >> \033[0;32m[Start]");
    return NULL;
}