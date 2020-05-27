/* Module: 日志模块
 * Author: 闫科宇
 * Date:   2020-04-29 22:00:00
 * Desc:   日志打印模块，支持控制台输出与日志文件输出
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "logger.h"

void log2stdout(const char* level, char* content);
void log2file(const char* level, char* content);
void getCurrentTime(char* curtime);

const char LOG_INFO[] = "INFO";
const char LOG_WARNNING[] = "WARNNING";
const char LOG_ERROR[] = "ERROR";

const char COLOR_GREEN[] = "\033[0;32m";
const char COLOR_RED[] = "\033[0;31m";
const char COLOR_YELLOW[] = "\033[0;33m";
const char COLOR_WHITE[] = "\033[0;37m";
const char COLOR_CYAN[] = "\033[0;36m";


char curtime[50];

void logger(const char* level, char* content) {
    log2stdout(level, content);
    log2file(level, content);
}

void log2stdout(const char* level, char* content) {
    getCurrentTime(curtime);
    if (strcmp(level, LOG_INFO) == 0)
        printf("%s[%s]%s[%s]%s %s\n", COLOR_GREEN, level, COLOR_CYAN, curtime, COLOR_WHITE, content);
    else if (strcmp(level, LOG_WARNNING) == 0)
        printf("%s[%s]%s[%s]%s %s\n", COLOR_YELLOW, level, COLOR_CYAN, curtime, COLOR_WHITE, content);
    else
        printf("%s[%s]%s[%s]%s %s\n", COLOR_RED, level, COLOR_RED, curtime, COLOR_RED, content);
    printf(COLOR_WHITE);
}

void log2file(const char* level, char* content) {
    // TODO
    return;
}

void getCurrentTime(char* curtime) {
    char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
    sprintf(curtime, "%d/%d/%d %s %02d:%02d:%02d", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday, 
        wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
}