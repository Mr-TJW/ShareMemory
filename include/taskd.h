#ifndef	_TASKD_H
#define	_TASKD_H

#include "public.h"

#define TASKD_LOG_NAME      "taskd"
#define TASK_NAME_LEN       64

typedef struct tag_Task_Info {
    pid_t   PID;                    //进程号
    int     rstFailCnt;             //重启失败次数
    int     dayRstCnt;              //日重启次数
    char    state;                  //进程状态
    char    name[TASK_NAME_LEN];   //进程名
}TASK_INFO;

class Daemon
{
public:
    Daemon();
    ~Daemon();

private:
    void InitDaemon();
};

#endif