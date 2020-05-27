/* Module: 命令解析模块
 * Author: 闫科宇
 * Date:   2020-04-29 23:10:38
 * Desc:   从消息队列中获取命令数据进行解析，根据解析结果调度不同模块
 */
#include <stdio.h>
#include "cmdparser.h"
#include "logger.h"

// 命令解析模块-线程主入口
void* pthread_cmdparser() {
    logger("INFO", "[CmdParser Module] >> \033[0;32m[...]");


    logger("INFO", "[CmdParser Module] >> \033[0;32m[Start]");
    return NULL;
}