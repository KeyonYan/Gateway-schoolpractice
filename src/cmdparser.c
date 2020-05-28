/* Module: 命令解析模块
 * Author: 闫科宇
 * Date:   2020-04-29 23:10:38
 * Desc:   从消息队列中获取命令数据进行解析，根据解析结果调度不同模块
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msgqueue.h"
#include "cmdparser.h"
#include "logger.h"
#include "tool.h"

#define SPLIT_DELIM '#' // 命令解析协议分割符
#define MSG_HEAD "CMD"  // 消息头

MSG msg;
char logBuf[1024];
int size;
int cmd_chain_size;
char **cmd_chain;

/**
 * 命令处理与调度
 */
void cmdHandler() {
    if (cmd_chain_size <= 0) return;
    if (strcpy(cmd_chain[0], MSG_HEAD) != 0) return;
    for (int i = 1; i < cmd_chain_size; i++) {
        if (strcpy(cmd_chain[i], "LED")) {
            // do something
            logger("INFO", "control LED");
        } else if (strcpy(cmd_chain[i], "TempSensor")) {
            // do something
            logger("INFO", "control tempSensor");
        } else if (strcpy(cmd_chain[i], "humidSensor")) {
            // do something
            logger("INFO", "control humidSensor");
        }
    }
}

// 命令解析模块-线程主入口
void* pthread_cmdparser() {
    logger("INFO", "[CmdParser Module] >> \033[0;32m[...]");
    

    while(1) {
        size = mqRead(&msg, 0);
        if (size <= 0) continue;
        sprintf(logBuf, "[CmdParser Module] Read a message from MQ: %s", msg.mtext);
        logger("INFO", logBuf);
        
        cmd_chain = split(msg.mtext, SPLIT_DELIM, &cmd_chain_size);
        cmdHandler();

    }

    logger("INFO", "[CmdParser Module] >> \033[0;32m[Start]");
    return NULL;
}


