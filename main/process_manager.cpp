/*
*********************************************************************
* Copyright(C) 2020 南瑞用电-终端部
* All rights reserved. 
* @filename: process_manger.cpp 
* @brief:	 进程管理
* @name：	 Mr-TJW
* @date：	 2020-08-06
* @history：
*********************************************************************
*/

#include "process_manager.h"

/*******************************************************************
* funcname:	mutex_init
* para:		g_mutex_lock，互斥锁变量
* function:	初始化互斥锁
* return:	0-SUCCESS,其他-error
********************************************************************/
int mutex_init(pthread_mutex_t *g_mutex_lock)
{	
	int ret;
	ret = pthread_mutex_init(g_mutex_lock, NULL);
    if (ret != 0) {
        printf("mutex init failed\n");
        return -1;
    }
}

/*******************************************************************
* funcname:	delete_mutex
* para:	
* function:	删除互斥锁
* return:	
********************************************************************/
void delete_mutex(pthread_mutex_t *g_mutex_lock)
{
	pthread_mutex_destroy(g_mutex_lock);
}

/*******************************************************************
* funcname:	mutex_lock
* para:	
* function:	互斥锁上锁
* return:	
********************************************************************/
void mutex_lock(pthread_mutex_t *g_mutex_lock)
{
	pthread_mutex_lock(g_mutex_lock);
}

/*******************************************************************
* funcname:	mutex_unlock
* para:	
* function:	互斥锁解锁
* return:	
********************************************************************/
void mutex_unlock(pthread_mutex_t *g_mutex_lock)
{
	pthread_mutex_unlock(g_mutex_lock);
}

/*******************************************************************
* funcname:	lock_file
* para:	    fd 文件索引
* function:	设置文件锁
* return:	
********************************************************************/
int lock_file(int fd)
{
	struct flock f1;
	f1.l_type = F_WRLCK;
	f1.l_start = 0;
	f1.l_whence = SEEK_SET;
	f1.l_len = 0;
	/* F_SETLK: 设置记录锁 */
	return (fcntl(fd, F_SETLK, &f1));
}

/*******************************************************************
* funcname:	check_process_running
* para:	
* function:	检测当前程序是否被多次启动
* return:	true  此程序目前被多次启动
*           false 此程序目前没有被多次启动
********************************************************************/
bool check_process_running(void)
{
	int fd;
	char buf[16];
 
	/* ORDWR:  以可读写方式打开文件
	 * OCREAT: 如文件不存在,则创建该文件 */
	fd = open(LOCKFIFE, O_RDWR|O_CREAT, LOCKMODE);
	if(fd < 0){
		exit(1);
	}
 
	if(lock_file(fd)){
		/* 错误码 errno:
		 * EAGAIN: Try again
		 * EACCES: Permission denied */
		if(errno == EACCES || errno == EAGAIN){
			close(fd);
			return true;
		}
		exit(1);
	}
 
	/* 改变文件大小,清空文件 */
	ftruncate(fd, 0);
	/* 写入进程号,可供管理员等查看 */
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf)+1);
	return false;
}


