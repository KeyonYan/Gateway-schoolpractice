/* Module: 警报模块（用串口通讯代替）
 * Author: 闫科宇
 * Date:   2020-05-31 10:32:15
 * Desc:   短信警报模块，用串口通讯代替，com.py作为串口接收端，serialport.c作为串口发送端
 */
#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <string.h>
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>  
#include <sys/stat.h>   
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <pthread.h>
#include "logger.h"
#include "alarm.h"

#define SERIALPORT_DEV "/dev/pts/20"

typedef struct {
    char size[4]; // 消息长度
    char which;   // 消息模板
    char size_tal[16];   // 指令
    char info[32];      // 信息长度
}AlarmMessage;

AlarmMessage alarmMsg[] = {
    {"0c", 0, "AT+CMGS=27\r", "11111111"}, // 仓库着火了
    {"0e", 1, "AT+CMGS=28\r", "22222222"}, // 仓库有贼闯入
    {"0a", 2, "AT+CMGS=29\r", "33333333"}, // 光照超标
    {"0a", 3, "AT+CMGS=30\r", "44444444"}  // 湿度超标
};

char buf[1024];
char logBuf[1024];
int fd;

/**
 * 初始化串口
 * @return 0 成功 | -1 失败
 */
int initSerialPort() {
    /* 以读写方式打开串口 */
    fd = open( SERIALPORT_DEV, O_RDWR | O_NOCTTY | O_NDELAY);
    if (-1 == fd){ 
        /* 不能打开串口一 */ 
        return -1;
    }

    struct termios opt;   // termios函数族提供一个常规的终端接口，用于控制非同步通信端口
    tcgetattr(fd, &opt);  // 取得当前串口的属性，并赋给fd这个设备
    /* 控制模式标志 */
    opt.c_cflag |= (CLOCAL | CREAD);    // clocal表示忽略modem控制线，cread表示打开接收者
    opt.c_cflag &= ~CSIZE;              // 清空原有字符长度（csize表示原有字符长度掩码）
    opt.c_cflag &= ~CRTSCTS;            // 启用RTS/CTS（硬件）流控制
    opt.c_cflag |= ~CS8;                // 设置字符长度掩码
    opt.c_cflag &= ~CSTOPB;             // 设置停止位为1个（cstopb表示设置两个停止位）
    /* 输入模式标志 */
    opt.c_iflag |= IGNPAR;              // 忽略帧错误和奇偶校验错误
    /* 输出模式标志 */
    opt.c_oflag = 0;
    /* 本地模式标志 */
    opt.c_lflag = 0;

    cfsetispeed(&opt, B9600);       // 设置输入波特率
    cfsetospeed(&opt, B9600);       // 设置输出波特率
    tcsetattr(fd, TCSANOW, &opt);   // 将opt属性赋给fd设备，tcsanow表示马上生效
    
    return 0;
}

// 线程入口函数
void *pthread_alarm() {
    logger("INFO", "[Alarm Module] >> \033[0;32m[...]");

    int ret = initSerialPort();
    if (ret >= 0) {
        logger("INFO", "[Alarm Module] SerialPort init \033[0;32m[Ok]");
    } else {
        logger("ERROR", "[Alarm Module] SerialPort init [Fail]");
        pthread_exit(NULL);
    }

    bzero(buf, sizeof(buf));
    sprintf(buf, "Hello World");
    write(fd, buf, strlen(buf));

    sprintf(logBuf, "[Alarm Module] SerialPort Write: %s", buf);
    logger("INFO", logBuf);

    close(fd);

    logger("INFO", "[Alarm Module] >> \033[0;32m[Ok]");
    return NULL;
}