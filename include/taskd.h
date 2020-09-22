#ifndef	_TASKD_H
#define	_TASKD_H

#include "public.h"

#define TASKD_LOG_NAME          "taskd"     //守护进程日志名
#define TASK_NAME_LEN           128         

#define TASK_CONFIG_FILE        "task.ini"  //任务配置文件
#define TASKD_CHDIR_DIR         "/"


#define MSG_QUEUE_KEY       0x11FF      //消息队列键值
#define TASKD_MSG_FILE      "task.msg"  //消息队列标识存储文件

#define TIMER_TIMING_VAL    1           //定时器定时1s
#define WAIT_TASK_STOP_TIME

/* 定时器定时值 */
#define TIMER_UNIT_NUM      5           //定时器数量
typedef enum tag_Timer_Timing_Value {
    TIMING_VAL_ONE_SEC = 1,
    TIMING_VAL_TEN_SEC = 10,
    TIMING_VAL_ONE_MIN = 60,
    TIMING_VAL_FIVE_MIN = 300,
    TIMING_VAL_HOUR = 3600
}TIME_TIMING_VAL;

/* ps 中 stat 状态 */
typedef enum tag_Task_Stat_State {
    TASK_STAT_NULL = 0,         //任务未运行
    TASK_STAT_NORMAL = 1,       //任务正常运行
    TASK_STAT_ZOMBIE = 2,       //任务为僵尸进程
    TASK_STAT_REPEAT = 3,       //任务存在多例，重复
}TASK_STAT_STATE;

/* s级定时器单元 */
typedef struct tag_Timer_Unit{
    UCHAR       isTimeOut;              //超时标志
    UINT        curVal;                 //当前值  
}TIMER_UNIT;

/* 任务信息 */
typedef struct tag_Task_Info {
    pid_t       pid;                    //进程号
    int         rstFailCnt;             //重启失败次数
    int         dayRstCnt;              //日重启次数
    char        state;                  //进程状态
    int         leftHeartBeatVal;       //剩余心跳值,
    char        *binPath;               //执行文件路径
    char        *name;                  //任务名
    TASK_INFO   *next;                  //任务链表
}TASK_INFO;

class Daemon
{
public:
    Daemon();
    ~Daemon();
    void initial();
private:
    time_t      last_record_time;   //上一次的记录时间
    TASK_INFO   *taskInfo;
    UINT        timerSetVal[TIMER_UNIT_NUM] = {TIMING_VAL_ONE_SEC,TIMING_VAL_TEN_SEC,TIMING_VAL_ONE_MIN,TIMING_VAL_FIVE_MIN,TIMING_VAL_HOUR};
    TIMER_UNIT  myTimer[TIMER_UNIT_NUM];
    UINT        heartBeatMaxVal;
    char	    logBuf[512];
    int         msgQueId;   //消息队列标识
private:
    void    initTimer();
    void    MyWriteLog();
    void    InitDaemon();
    void    InitTaskChain();
    bool    isFileCanRun(const char *pathName);
    int     registerTask(const char *pathName,const char *taskName);
    int     createMsgQue();
    void    createTimerInitSignal();
    int     getTaskRunState(const char *taskName);
    void    bootUpTask();
    int     saveMsgQueId(int msgId);
    void    exitDaemon(int signal);
    void    timerInterrupt(int signal);
    int     startOneTask(TASK_INFO *taskInf);
    int     stopOneTask(TASK_INFO *taskInf);
    int     restartOneTask(TASK_INFO *taskInf);
    int     waitTaskStop(const char *taskName);
};

#endif