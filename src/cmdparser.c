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
int cmdHandler() {
    if (cmd_chain_size <= 0) {
        logger("ERROR", "[CmdParser Module]"
                        "Message format does not conform to protocol: No delim");
        return -1;
    };
    if (strcmp(cmd_chain[0], MSG_HEAD) != 0) {
        logger("ERROR", "[CmdParser Module]"
                        "Message format does not conform to protocol: No header");
        return -2;
    }
    sprintf(logBuf, "[CmdParser Module] cmd_chain_size: %d", cmd_chain_size);
    logger("INFO", logBuf);
    for (int i = 1; i < cmd_chain_size; i++) {
        if (strcmp(cmd_chain[i], "LED") == 0) { /* LED管理 */
            // do something
            logger("INFO", "control LED");
        } else if (strcmp(cmd_chain[i], "TempSensor") == 0) { /* 温度传感器管理 */
            // do something
            logger("INFO", "control tempSensor");
        } else if (strcmp(cmd_chain[i], "HumidSensor") == 0) { /* 湿度传感器管理 */
            // do something
            logger("INFO", "control humidSensor");
        }
    }
    return 0;
}

// 命令解析模块-线程主入口
void* pthread_cmdparser() {
    logger("INFO", "[CmdParser Module] >> \033[0;32m[Ok]");

    while(1) {
        size = mqRead(&msg, 0);
        if (size <= 0) continue;
        sprintf(logBuf, "[CmdParser Module] Read a message from MQ: %s", msg.mtext);
        logger("INFO", logBuf);
        
        cmd_chain = split(msg.mtext, SPLIT_DELIM, &cmd_chain_size);
        
        #ifdef __DEBUG__
        printf("cmd_chain_size: %d\n", cmd_chain_size);
        printf("cmd_chain0: %s\n", cmd_chain[0]);
        printf("cmd_chain1: %s\n", cmd_chain[1]);
        #endif // !__DEBUG__
        
        cmdHandler();

    }

    logger("INFO", "[CmdParser Module] >> \033[0;32m[Close]");
    return NULL;
}


