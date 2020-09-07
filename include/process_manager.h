#ifndef	_PROCESS_MANAGER_H
#define	_PROCESS_MANAGER_H

#include "public.h"

#define LOCKFIFE	"daemon.pid"
 
/* 参数mode设定,只有在建立新文件时才会有效
 * S_IRUSR: 代表该文件所有者具有可读的权限
 * S_IWUSR: 代表该文件所有者具有可写的权限
 * S_IRGRP: 代表该文件用户组具有可读的权限
 * S_IROTH: 代表该其他用户具有可读的权限   */
#define LOCKMODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
 
int mutex_init();
void delete_mutex();
void mutex_lock();
void mutex_unlock();
int lock_file(int fd);
bool check_process_running(void);

#endif