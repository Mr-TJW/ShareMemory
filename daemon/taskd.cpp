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
static void MyWriteLog(const char *log)
{
    write_log(TASKD_LOG_NAME,log,true);
}


Daemon::Daemon()
{

}

Daemon::~Daemon()
{

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

