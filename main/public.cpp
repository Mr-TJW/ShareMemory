/*
*********************************************************************
* Copyright(C) 2020 南瑞用电-终端部
* All rights reserved. 
* @filename: public.cpp 
* @brief:	 基础功能：设定及获取终端时间、日志功能
* @name：	 Mr-TJW
* @date：	 2020-08-10
* @history：
*********************************************************************
*/

#include "public.h"

static UCHAR print_switch[PRINT_SWTICH_NUM] = {DISABLE};
void (*PRINT_ENABLE[PRINT_SWTICH_NUM])(void) = {print_debug_enable,print_common_enable,print_warning_enable,print_all_enable};
void (*PRINT_DISABLE[PRINT_SWTICH_NUM])(void) = {print_debug_disable,print_common_disable,print_warning_disable,print_all_disable};
void (*PRINTF[PRINT_SWTICH_NUM])(const char *format, ...) = {print_debug,print_common,print_warning};


/*******************************************************************
* funcname:	set_terminal_time
* para:	    time_value-所设定的时间值
*           time_format-设定值的类型（time_t,struct tm,DATE_TIME_S,DATE_TIME_MS）
* function:	设定终端时间
* return:	成功-SUCCESS，失败-FALIURE
********************************************************************/
int set_terminal_time(const void *time_value,ENUM_TIME_TYPE time_type)
{   
    printf("\r\nset terminal time!\r\n");
    int ret;
    time_t          curtime = time(NULL);
    struct timeval  tv_set;
    struct tm       local_time;
    DATE_TIME_S     *date_time_s;
    DATE_TIME_MS    *date_time_ms;

    switch(time_type)
    {
        case(TIME_TYPE_TIME):
            tv_set.tv_sec  = *((time_t *)time_value);
            tv_set.tv_usec = 0;
            break;
        case(TIME_TYPE_TM):
            tv_set.tv_sec  = mktime((struct tm*)time_value);
            tv_set.tv_usec = 0;
            break;
        case(TIME_TYPE_DATE_S):
            date_time_s = (DATE_TIME_S *)time_value;
            local_time.tm_year = date_time_s->year - 1900; 
            local_time.tm_mon  = date_time_s->mon -1;
            local_time.tm_mday = date_time_s->day;
            local_time.tm_hour = date_time_s->hour;
            local_time.tm_min  = date_time_s->min;
            local_time.tm_sec  = date_time_s->sec;
            tv_set.tv_sec  = mktime(&local_time);
            tv_set.tv_usec = 0;
            break;
        case(TIME_TYPE_DATE_MS):
            date_time_ms = (DATE_TIME_MS *)time_value;
            local_time.tm_year = date_time_ms->year - 1900; 
            local_time.tm_mon  = date_time_ms->mon -1;
            local_time.tm_mday = date_time_ms->day;
            local_time.tm_hour = date_time_ms->hour;
            local_time.tm_min  = date_time_ms->min;
            local_time.tm_sec  = date_time_ms->sec;
            tv_set.tv_sec  = mktime(&local_time);
            tv_set.tv_usec = date_time_ms->msec;
            break;
        default:
            printf("unknown time type, set time failure,quit!!!");
            return FAILURE;
    }

    printf("before set time: \t%s",asctime(localtime(&curtime)));

    ret = settimeofday(&tv_set,NULL);
    if(ret < 0)
    {
        perror("set time failure:");
        return FAILURE;
    }
    else
    {
        curtime = time(NULL);
        printf("after set time: \t%s",asctime(localtime(&curtime)));
        printf("set time successful!!!\r\n\n");
    }
    return SUCCESS;
}

/*******************************************************************
* funcname:	get_terminal_time
* para:	    time_value - 返回的时间值
*           time_type  - 设定值的类型（time_t,struct tm,DATE_TIME_S,DATE_TIME_MS）
* function:	获取当前时间
* return:	void
********************************************************************/
int get_terminal_time(void *time_value,ENUM_TIME_TYPE time_type)
{   
    int ret;
    time_t          curtime = time(NULL);
    struct tm       *local_time;
    DATE_TIME_S     date_time_s;
    DATE_TIME_MS    date_time_ms;
    struct timeval  time_us;

    local_time = localtime(&curtime);

    switch(time_type)
    {
        case(TIME_TYPE_TIME):
            time((time_t *)time_value);
            printf("current time: %s",asctime(localtime(&curtime)));
            break;
        case(TIME_TYPE_TM):
            memcpy((DATE_TIME_MS *)time_value,local_time,sizeof(tm));
            break;
        case(TIME_TYPE_DATE_S):
            (*((DATE_TIME_S *)time_value)).year = local_time->tm_year+1900;
            (*((DATE_TIME_S *)time_value)).mon  = local_time->tm_mon+1;
            (*((DATE_TIME_S *)time_value)).day  = local_time->tm_mday;
            (*((DATE_TIME_S *)time_value)).hour = local_time->tm_hour;
            (*((DATE_TIME_S *)time_value)).min  = local_time->tm_min;
            (*((DATE_TIME_S *)time_value)).sec  = local_time->tm_sec;
            break;
        case(TIME_TYPE_DATE_MS):
            gettimeofday(&time_us,NULL);
            (*((DATE_TIME_MS *)time_value)).year = local_time->tm_year+1900;
            (*((DATE_TIME_MS *)time_value)).mon  = local_time->tm_mon+1;
            (*((DATE_TIME_MS *)time_value)).day  = local_time->tm_mday;
            (*((DATE_TIME_MS *)time_value)).week = local_time->tm_wday;
            (*((DATE_TIME_MS *)time_value)).hour = local_time->tm_hour;
            (*((DATE_TIME_MS *)time_value)).min  = local_time->tm_min;
            (*((DATE_TIME_MS *)time_value)).sec  = local_time->tm_sec;
            (*((DATE_TIME_MS *)time_value)).msec = time_us.tv_usec/1000;
            break;
        default:
            printf("unknown time type, get time failure,quit!!!");
            return FAILURE;
    }

    //printf("current time: %s",asctime(localtime(&curtime)));

    return SUCCESS;
}

/*******************************************************************
* funcname:	check_dir_exist
* para:	    dir_name        目录名
*           create_flag     创建标志，true-不存在则创建，false-不存在不创建
* function:	检测所查目录是否存在
* return:	true-存在，false-不存在
********************************************************************/
bool check_dir_exist(const char * dir_name,bool create_flag)
{
    struct stat stat_buf;
    
    stat(dir_name,&stat_buf);
    if(S_ISDIR(stat_buf.st_mode) == true)   //是否为目录
    {
        return true;
    }

    /* 检测到不是目录，检测是否为文件，若是则删除文件 */
    if(access(dir_name,R_OK) == SUCCESS) 
    {   
        printf("%s is file ,not dir\r\n",dir_name);
        remove(dir_name);
    }

    /* 没有检测到目录，是否创建 */
    if(create_flag == false)
    {
        return false;
    }

    /* 创建目录，文件用户、文件用户组、其他用户都具有对此目录读写执行权限 */
    if(mkdir(dir_name,S_IRWXU | S_IRWXG | S_IRWXO) == SUCCESS)
    {   
        /* 再次判断是否为目录 */
        stat(dir_name,&stat_buf);
        if(S_ISDIR(stat_buf.st_mode) == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

/*******************************************************************
* funcname:	write_log
* para:	    file_name 文件名
*           log_test  添加的日志内容
*           time_log  时间标签，true-带时标，false-不带时标
* function:	写日志文件
* return:	
********************************************************************/
void write_log(const char *file_name,const char *log_text,unsigned char time_flag)
{
    FILE *fp;
    int ret;
    char *time_ptr;
    time_t curtime = time(NULL);
    char log_name[LOG_NAME_MAX_LEN] = {0};

    printf("%s",log_text);
    
    if(check_dir_exist(LOG_DIR_PATH,true) == true)
    {
        sprintf(log_name,"%s/%s",LOG_DIR_PATH,file_name);
    }
    else
    {
        sprintf(log_name,"%s",file_name);
    }
    
    strcat(log_name,".log");

    if((fp = fopen(log_name,"a")) == NULL)      //以追加方式打开文件
    {
        printf("%s open error : %s \r\n",file_name,strerror(errno));
        return;
    }

    /* 是否带时标 */
    if(time_flag == true && log_text[0] != '\r' && log_text[0] != '\n') //单纯的换行不带时标
    {   
        time_ptr = asctime(localtime(&curtime));
        for(int i=0;i<strlen(time_ptr);i++)
        {
            if(time_ptr[i] == '\r' || time_ptr[i] == '\n')
            {
                time_ptr[i] = '\0';
                break;
            }
        }
        fprintf(fp,"%s:",time_ptr);
    }

    fprintf(fp,"%s\r\n",log_text);

    if(fclose(fp) != 0)
    {
        perror("fclose error:");
    }

    if(over_size_bak_file(log_name) == true)
    {
        printf("file size over set value,bak file !!!\r\n");
    }
}

/*******************************************************************
* funcname:	over_size_bak_file
* para:	    filename 文件名(带路径)
* function:	检测文件大小是否超过设定值，则将此文件设定为备份文件，覆盖原先的备份文件
* return:	true：大小超过设定值，false：没有超过设定值
********************************************************************/
bool over_size_bak_file(const char* filename)
{   
    int             fd;
    struct stat     stat_buf;
    unsigned int    file_size;
    char bak_name[LOG_NAME_MAX_LEN] = {0};

    strncpy(bak_name,filename,LOG_NAME_MAX_LEN);
    strcat(bak_name,".bak");
    if(stat(filename,&stat_buf) == SUCCESS)
    {
        file_size = stat_buf.st_size;
        if(file_size > LOG_FILE_SIZE)
        {
            if((fd = open(filename,O_RDONLY,0)) != -1)
            {
                close(fd);
                remove(filename);
            }
            rename(filename,bak_name);
            return true;
        }
    }
    return false;
}

/*******************************************************************
* funcname:	PRINT_ENABLE
* para:	
* function:	使能打印开关
* return:	
********************************************************************/
void print_debug_enable()   {print_switch[DEBUG] = ENABLE;}
void print_common_enable()  {print_switch[COMMON] = ENABLE;}
void print_warning_enable() {print_switch[WARNING] = ENABLE;}
void print_all_enable()
{
    print_switch[DEBUG] = ENABLE;
    print_switch[COMMON] = ENABLE;
    print_switch[WARNING] = ENABLE;
}

/*******************************************************************
* funcname:	PRINT_DISABLE
* para:	
* function:	禁用打印开关
* return:	
********************************************************************/
void print_debug_disable()  {print_switch[DEBUG] = DISABLE;}
void print_common_disable() {print_switch[COMMON] = DISABLE;}
void print_warning_disable(){print_switch[WARNING] = DISABLE;}
void print_all_disable()
{   
    print_switch[DEBUG] = DISABLE;
    print_switch[COMMON] = DISABLE;
    print_switch[WARNING] = DISABLE;
}

/*******************************************************************
* funcname:	PRINTF_LOG
* para:	    可变参数，类似于 printf
* function:	将变参字符串打印为日志
* return:	返回转换的字符串
********************************************************************/
void print_debug(const char *format, ...)
{	
    if(print_switch[DEBUG] == ENABLE)
    {
        va_list arglist;
        char alarm_string[PRINTF_BUF_LEN] = {0};

        va_start(arglist, format);
        vsnprintf(alarm_string,PRINTF_BUF_LEN, format, arglist);		
        va_end(arglist);
        
        printf("%s",alarm_string);
        //write_log("xxx",alarm_string,0);    //将打印信息保存在名为 xxx 的日志中
    }
}

void print_common(const char *format, ...)
{	
    if(print_switch[COMMON] == ENABLE)
    {
        va_list arglist;
        char alarm_string[PRINTF_BUF_LEN] = {0};

        va_start(arglist, format);
        vsnprintf(alarm_string,PRINTF_BUF_LEN, format, arglist);		
        va_end(arglist);
        
        printf("%s",alarm_string);
        //write_log("xxx",alarm_string,0);    //将打印信息保存在名为 xxx 的日志中
    }   
}

void print_warning(const char *format, ...)
{	
    if(print_switch[WARNING] == ENABLE)
    {
        va_list arglist;
        char alarm_string[PRINTF_BUF_LEN] = {0};

        va_start(arglist, format);
        vsnprintf(alarm_string,PRINTF_BUF_LEN, format, arglist);		
        va_end(arglist);
        
        printf("%s",alarm_string);
        //write_log("xxx",alarm_string,0);    //将打印信息保存在名为 xxx 的日志中
    }
}

