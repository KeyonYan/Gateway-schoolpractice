#ifndef _MSGQUEUE_H_
#define _MSGQUEUE_H_

#define MSG_TEXT_SIZE 1024

typedef struct {
    long mtype;         // 消息类型
    char mtext[MSG_TEXT_SIZE];    //消息正文
    // 消息的正文可以有多个成员，任意数据类型
}MSG;

#define MSG_TYPE 7 // 消息类型

int mqCreate();
MSG* msgCreate();
int mqWrite(MSG* msg);
int mqRead(MSG* msg, long type);
int mqRemove();

#endif // !_MSGQUEUE_H_