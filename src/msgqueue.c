/* Module: MQ消息队列函数封装
 * Author: 闫科宇
 * Date:   2020-05-28 12:03:33
 * Desc:   提供消息队列的创建、写入、读取
 */

#include "msgqueue.h"
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

key_t mq_key;   // 消息队列的key值
int mq_id;              // 消息队列标识符

/**
 * 消息队列的创建
 * @Return 消息队列标识符
 */
int mqCreate() {
    mq_key = ftok(".", 2020);
    mq_id = msgget(mq_key, IPC_CREAT|0666);
    return mq_id;
}

/**
 * 创建一条Message
 * @Return 指向创建的消息的指针
 */
MSG* msgCreate() {
    MSG* msg = (MSG*) malloc(sizeof(MSG));
    msg->mtype = MSG_TYPE;
    bzero(msg->mtext, sizeof(msg->mtext));
    return msg;
}

/**
 * 写入消息到消息队列（非阻塞）
 * @Param msg 消息
 * @Return 成功：0|失败：-1
 */
int mqWrite(MSG* msg) {
    return msgsnd(mq_id, msg, MSG_TEXT_SIZE, IPC_NOWAIT); // no-blocking
}

/**
 * 从消息队列读取消息（阻塞）
 * @Param msg 消息
 * @Param type 消息类型[0:返回队列中第一个消息| >0: 返回类型为type的消息]
 * @Return 成功：读取消息的长度|失败：-1
 */
int mqRead(MSG* msg, long type) {
    return msgrcv(mq_id, msg, MSG_TEXT_SIZE, type, 0);
}

/**
 * 从系统中删除消息队列
 */
int mqRemove() {
    return msgctl(mq_id, IPC_RMID, NULL);
}