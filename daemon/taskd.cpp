/*
*********************************************************************
* Copyright(C) 2020 南瑞用电-终端部
* All rights reserved. 
* @filename: taskd.cpp 
* @brief:	 守护进程
* @name：	 Mr-TJW
* @date：	 2020-09-08
* @history：
*********************************************************************
*/
#include "taskd.h"

/*******************************************************************
* funcname:	MyWriteLog
* para:	
* function:	记录 task 进程日志
* return:	
********************************************************************/



Daemon::Daemon()
{
	initial();
}

Daemon::~Daemon()
{

}

void Daemon::initial()
{	
	taskInfo = NULL;
	msgQueId = -1;
	heartBeatMaxVal = 60;
	memset(logBuf,0,sizeof(logBuf));
	last_record_time = time(NULL);
}

void Daemon::MyWriteLog()
{
    write_log(TASKD_LOG_NAME,(const char *)logBuf,true);
	memset(logBuf,0,sizeof(logBuf));
}

/*******************************************************************
* funcname:	InitDaemon
* para:	    
* function:	创建守护进程
* return:	
********************************************************************/
void Daemon::InitDaemon()
{   
    int i;

    if (fork() != 0)            //成为会话首进程，失去控制终端
	{   
        MyWriteLog("fork error 1");
		exit(1);
	}
	
	if (setsid() < 0)           //创建新会话，使守护进程完全独立出来
	{   
        MyWriteLog("setsid error ");
		exit(1);
	}
	
	if (fork() != 0)            //使进程不再是会话首进程，禁止进程重新打开控制终端
	{   
        MyWriteLog("fork error 2");
		exit(1);
	}
	
	if (chdir("/") == -1)       //改变当前工作目录为根目录
	{   
        MyWriteLog("chdir error ");
		exit(1);
	}

    for(i = 0; i < 3; ++i)      //关闭文件描述符(常说的输入，输出，报错3个文件)
	{
		close(i);
	}
	
	umask(0);                   //重设文件掩码为0（将权限全部开放）
	signal(SIGCHLD,SIG_IGN);    //忽略 SIGHUP 信号
    MyWriteLog("守护进程初始化完成！");
}

/*******************************************************************
* funcname:	isFileCanRun
* para:	
* function:	检测文件是否可以运行
* return:	成功TRUE，失败FALSE
********************************************************************/
bool Daemon::isFileCanRun(const char *pathName)
{	
	if(pathName == NULL)
	{	
		MyWriteLog("pathName = NULL");
		return FALSE;
	}

	if(access(pathName,F_OK) == -1)
	{
		sprintf(logBuf,"%s文件不存在",pathName);
		MyWriteLog(logBuf);
		return FALSE;
	}

	if(access(pathName,X_OK) == -1)
	{
		sprintf(logBuf,"%s文件无法执行",pathName);
		MyWriteLog(logBuf);
		return FALSE;
	}

	return TRUE;
}

/*******************************************************************
* funcname:	registerTask
* para:		pathName 任务路径名，taskName 任务名
* function:	从任务配置文件中获取任务列表，注册到任务链表中
* return:	
********************************************************************/
int Daemon::registerTask(const char *pathName,const char *taskName)
{	
	char *pName,tName;
	TASK_INFO *tInfo,*curInfo;
	
	if(	NULL == (pName = (char *)malloc(strlen(pathName)+1)) ||
		NULL == (tName = (char *)malloc(strlen(taskName)+1)) ||
		NULL == (tInfo = (TASK_INFO *)malloc(sizeof(TASK_INFO))))
	{
		MyWriteLog("registerTask malloc 申请内存失败");
		return FAILURE;
	}

	strcpy(pName,pathName);
	strcpy(tName,taskName);

	tInfo->binPath = pathName;
	tInfo->name = tName;
	tInfo->PID = -1;
	tInfo->rstFailCnt = 0;
	tInfo->dayRstCnt = 0;
	tInfo->leftHeartBeatVal = heartBeatMaxVal;
	tInfo->next = NULL;
	
	if(taskInfo == NULL)
	{
		taskInfo = tInfo;
	}
	else
	{	
		curInfo = taskInfo;
		while(curInfo->next != NULL)
		{
			curInfo = curInfo->next;
		}
		curInfo->next = tInfo;
	}
	return SUCCESS;
}

/*******************************************************************
* funcname:	InitTaskChain
* para:	
* function:	初始化任务链表
* return:	
********************************************************************/
int Daemon::InitTaskChain()
{
	int		i,taskNum;
	char	taskName[TASK_NAME_LEN];
	char	pathName[TASK_NAME_LEN];
	char 	resultBuf[TASK_NAME_LEN];
	char 	tempBuf[TASK_NAME_LEN];
	char	cmdBuf[TASK_NAME_LEN];

	if(SUCCESS == readIniFile(TASK_CONFIG_FILE,"任务表","任务总数",resultBuf))
	{
		taskNum = atoi(resultBuf);
	}
	else
	{
		MyWriteLog("任务总数获取失败");
	}

	for(i=0;i<taskNum;i++)
	{
		sprintf(cmdBuf,"任务%d命令",i+1);
		if(FAILURE == readIniFile(TASK_CONFIG_FILE,"任务表",cmdBuf,resultBuf))
		{
			sprintf(logBuf,"配置文件%s，任务表段落，%s 获取失败",TASK_CONFIG_FILE,logBuf);
			MyWriteLog(logBuf);
			memset(logBuf,0,sizeof(TASK_NAME_LEN));
			continue;
		}

		strcpy(pathName,resultBuf);
		if(NULL != (tempBuf = strrchr(pathName,'/')))
		{
			tempBuf++;
			strcpy(taskName,tempBuf);

			if(isFileCanRun(pathName))
			{
				if(FAILURE == registerTask(pathName,taskName))
				{
					sprintf(logBuf,"守护进程注册%s任务失败",taskName);
					MyWriteLog(logBuf);
				}
				else
				{
					sprintf(logBuf,"守护进程注册%s任务成功",taskName);
					MyWriteLog(logBuf);
				}	
			}
		}
	}
}

/*******************************************************************
* funcname:	createMsgQue
* para:	
* function:	创建消息队列
* return:	成功SUCCESS，失败FAILURE
********************************************************************/
int Daemon::createMsgQue()
{	
	if(-1 == (msgQueId = msgget(MSG_QUEUE_KEY,IPC_CREAT|0666)))
	{
		MyWriteLog("消息队列创建失败");
		return FAILURE;
	}
	else
	{
		if(FAILURE == saveMsgQueId(msgQueId))
		{
			return FAILURE;
		}
		
	}
	MyWriteLog("消息队列创建并保存成功");
	return SUCCESS;
}

/*******************************************************************
* funcname:	saveMsgQueId
* para:		msgId 消息队列标识
* function:	保存消息队列标识到指定文件中,以便其他进程获取
* return:	成功SUCCESS，失败FAILURE
********************************************************************/
int Daemon::saveMsgQueId(int msgId)
{
	FILE *fp;

	if(NULL == (fp = fopen(TASKD_MSG_FILE,"w+")))
	{
		MyWriteLog("消息队列标识存储文件创建失败");
		return FAILURE;
	}
	else
	{
		fwrite(&msgId,sizeof(int),1,fp);
		fclose(fp);
	}
	return SUCCESS;
}

/*******************************************************************
* funcname:	createTimer
* para:	
* function:	创建定时器，并注册信号处理函数，软中断信号
* return:	成功SUCCESS，失败FAILURE
********************************************************************/
void Daemon::createTimerInitSignal()
{	
	/* 注册退出信号，即接收到此信号后进入退出函数 */
	if(signal(SIGTERM,exitDaemon) == SIGERR)
	{
		MyWriteLog("守护进程注册退出信号失败");
		exit(1);
	}

	/* 注册定时器信号，即接收到此信号后进入定时器中断函数 */
	if(signal(SIGTERM,timerInterrupt) == SIGERR)
	{
		MyWriteLog("守护进程注册定时器信号失败");
		exit(1);
	}

	if(-1 == alarm(TIMER_TIMING_VAL))
	{
		MyWriteLog("守护进程创建定时器失败");
		exit(1);
	}

	MyWriteLog("守护进程注册退出信号成功");
	MyWriteLog("守护进程注册定时器信号成功");
	MyWriteLog("守护进程创建定时器成功");
}

/*******************************************************************
* funcname:	checkTaskRunState
* para:	
* function:	检测程序运行状态
* return:	
********************************************************************/
int Daemon::checkTaskRunState(const char *taskName)
{

}

/*******************************************************************
* funcname:	bootUpTask
* para:	
* function:	启动任务，将任务链表中的任务启动
* return:	
********************************************************************/
void Daemon::bootUpTask()
{
	
}

/*******************************************************************
* funcname:	timerInterrupt
* para:		signal 信号
* function:	定时器软中断函数
* return:	
********************************************************************/
void Daemon::timerInterrupt(int signal)
{	
	int i;
	time_t curTime = time(NULL);

	if(abs(curTime - last_record_time > TIME_VAL_HOUR))
	{
		MyWriteLog("守护进程监控中......");
	}

	for(i=0;i<TIMER_UNIT_NUM;i++)
	{
		if(myTimer[i].isTimeout)
		{
			continue;
		}

		myTimer[i].elapse--;
		if(myTimer[i].elapse < = 0)
		{
			myTimer[i].isTimeout = true;
		}
	}

	/* 重新定时，定时器触发中断后，就会失效，需要再次设定 */
	alarm(TIMER_TIMING_VAL);
}

/*******************************************************************
* funcname:	initTimer
* para:	
* function:	初始化定时器
* return:	
********************************************************************/
void Daemon::initTimer()
{	
	for (int i=0; i<TIMER_UNIT_NUM; i++)
	{
		myTimer[i].isTimeout = false;
		myTimer[i].elapse = TimerInterval[i];
	}
}

/*******************************************************************
* funcname:	exitDaemon
* para:		signal 信号
* function:	响应守护进程退出信号,同时进行资源回收,退出守护进程
* return:	
********************************************************************/
void Daemon::exitDaemon(int signal)
{	
	TASK_INFO *tInfo,tempInfo;

	if(SIGTERM == signal)
	{
		MyWriteLog("守护进程响应系统终止命令");
	}

	/* 释放定时器资源 */
	if(-1 == alarm(0))
	{
		MyWriteLog("守护进程定时器删除失败");
	}
	else
	{
		MyWriteLog("守护进程定时器删除成功");
	}
	
	/* 删除消息队列 */
	if(msgQueId > 0)
	{
		if(-1 == msgctl(msgQueId,IPC_RMID,NULL))
		{
			MyWriteLog("守护进程消息队列删除失败");
		}
		else
		{
			msgQueId = -1;
			if(FAILURE == saveMsgQueId(msgQueId))
			{
				MyWriteLog("守护进程消息队列删除失败");
			}
			else
			{
				MyWriteLog("守护进程消息队列删除成功");
			}
		}
	}
	
	/* 释放任务链表资源 */
	tInfo = taskInfo;
	while(tInfo != NULL)
	{
		free(tInfo->name);
		free(tInfo->binPath);
		tempInfo = tInfo->next;
		free(tInfo);
		tInfo = tempInfo;
	}
	MyWriteLog("守护进程释放任务链表完成");
	MyWriteLog("守护进程退出");

	exit(0);
}