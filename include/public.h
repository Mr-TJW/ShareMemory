#ifndef	_PUBLIC_H
#define	_PUBLIC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <sys/types.h> 
#include <netdb.h>      
#include <unistd.h>     
#include <sys/select.h>   
#include <sys/errno.h> 
#include <sys/ioctl.h>   
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include<sys/time.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "data_type.h"

extern  int errno;

#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))

#ifndef offsetof
#define offsetof(type, member) ((size_t) &((type *)0)->member)
#endif

#define MAX_P_ID_NUM							5			/* 最大进程数量 */

typedef enum tag_Progress_Id_Info 
{
	P_ID_MAIN 	= 0,		/* 主进程 */
	P_ID_TEST1	= 1,		/* 测试进程 1 */
	P_ID_TEST2	= 2,		/* 测试进程 2 */
	P_ID_TEST3	= 3,		/* 测试进程 3 */
	P_ID_TEST4	= 4,		/* 测试进程 4 */
}P_ID_INFO;

#define LOG_NAME_MAX_LEN 						64			/* log 文件名最大长度 */
#define LOG_DIR_PATH							"../log"	/* log 文件存放路径 */
#define LOG_FILE_SIZE							1*1024*1024	/* 以字节为单位，1M */
#define LOG_BUF_LEN								4096		/* 一行日志的最大长度 */
#define INI_FILE_ITEM_LEN						128			/* ini 文件一行最长字节数 */

#define SUCCESS									0
#define	FAILURE									-1
#define TRUE									1
#define FALSE									0

#define LINK_OK     							1			/* 连接成功 */
#define ENABLE                  				1           /* 使能 */
#define DISABLE                 				0           /* 禁用 */
#define PRINTF_BUF_LEN          				2048        /* 一条打印信息的最大长度 */
#define PRINT_SWTICH_NUM        				4           /* 打印开关个数 */

#define CMD_BUF_MAX_LEN         				1024		/* 共享内存cmdBuf最大字节 */
#define CMD_BUF_MAX_NUM     					32			/* 共享内存中cmdBuf最大数量 */

typedef enum _ENUM_PRINT_SWTICH_
{
    DEBUG = 0,
    COMMON,
    WARNING,
    ALL
}ENUM_PRINT_SWTICH;

extern  int errno;
extern void (*PRINT_ENABLE[PRINT_SWTICH_NUM])(void);
extern void (*PRINT_DISABLE[PRINT_SWTICH_NUM])(void);
extern void (*PRINTF[PRINT_SWTICH_NUM])(const char *format, ...);

typedef enum _tag_Second_Uint_Time_Val
{
    TIME_VAL_SEC 	= 1,
    TIME_VAL_MIN 	= 60,
	TIME_VAL_HOUR 	= 3600,
	TIME_VAL_DAY	= 86400
}SEC_UINT_TIME_VAL;

/*年月日时分秒*/
typedef struct _DATE_TIME_S_
{
	USHORT		year;
	UCHAR		mon;
	UCHAR		day;
	UCHAR		hour;
	UCHAR		min;
	UCHAR		sec;
}DATE_TIME_S;

/*年月日周时分秒毫秒*/
typedef struct _DATE_TIME_MS_
{
	USHORT		year;
	UCHAR		mon;
	UCHAR		day;
    UCHAR       week;
	UCHAR		hour;
	UCHAR		min;
	UCHAR		sec;
    USHORT      msec;
}DATE_TIME_MS;

typedef enum _TIME_UNIT_
{
    TIME_UNIT_SEC = 0,      
    TIME_UNIT_MIN,           
    TIME_UNIT_HOUR,    
	TIME_UNIT_DAY,   
	TIME_UNIT_MON,   
	TIME_UNIT_YEAR,       
}TIME_UNIT;


typedef enum _ENUM_TIME_TYPE_
{
    TIME_TYPE_TIME = 0,  		/* time函数生成的 s， 	即 time_t 类型 */
	TIME_TYPE_TM,				/* struct tm 类型 */
	TIME_TYPE_DATE_S,    		/* 年月日时分秒 ,		即 DATE_TIME_S */
	TIME_TYPE_DATE_MS,			/* 年月日周时分秒毫秒 ，即 DATE_TIME_MS */
}ENUM_TIME_TYPE;


typedef struct tag_Control_Command_Buf
{
    int         cmdType;        /* 命令类型 */
    int         cmdFunc;        /* 命令功能号 */
    int         cmdPoint;       /* 命令点号 */
    time_t      cmdTime;        /* 命令下发时间 */
    USHORT      cmdLen;         /* 命令长度 */
    UCHAR       cmdBuf[CMD_BUF_MAX_LEN];    /* 命令内容 */
}CTL_CMD_BUF;

int 	set_terminal_time(const void *time_value,ENUM_TIME_TYPE time_type = TIME_TYPE_TIME);
int 	get_terminal_time(void *time_value,ENUM_TIME_TYPE time_type = TIME_TYPE_TIME);
void 	write_log(const char *file_name,const char *log_text,unsigned char time_flag);
bool	over_size_bak_file(const char* filename);
bool	check_dir_exist(const char * dir_name,bool create_flag);
void 	print_debug_enable();
void 	print_common_enable();
void 	print_warning_enable();
void 	print_all_enable();
void 	print_debug_disable();
void 	print_common_disable();
void 	print_warning_disable();
void 	print_all_disable();
void 	print_debug(const char *format, ...);
void 	print_common(const char *format, ...);
void 	print_warning(const char *format, ...);
int 	readIniFile(const char *fileName,const char *segName,const char *itemName,char *result);

#endif
