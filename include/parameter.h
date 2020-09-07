#ifndef	_PARAMETER_H
#define	_PARAMETER_H

#include "public.h"

#define MAX_BUF_SIZE                1024
#define MAX_PARA_NAME_LEN           32

typedef struct tag_Terminal_Update_Para
{
    UCHAR    paraIsUpdataFlag[MAX_PARA_NAME_LEN];
}TMN_UPDARE_PARA;

typedef struct tag_Run_Para_F1
{
    UCHAR   buf[MAX_BUF_SIZE];
    USHORT  len;
}RUN_PARA_F1;

typedef struct tag_Run_Para_F2
{
    UCHAR   buf[MAX_BUF_SIZE];
    USHORT  len;
}RUN_PARA_F2;

typedef struct tag_Run_Para_F3
{
    UCHAR   buf[MAX_BUF_SIZE];
    USHORT  len;
}RUN_PARA_F3;

typedef struct tag_Run_Para_F4
{
    UCHAR   buf[MAX_BUF_SIZE];
    USHORT  len;
}RUN_PARA_F4;

typedef struct tag_Run_Para_F5
{
    UCHAR   buf[MAX_BUF_SIZE];
    USHORT  len;
}RUN_PARA_F5;

typedef struct tag_Sys_Para_F1
{
    UCHAR   buf[MAX_BUF_SIZE];
    USHORT  len;
}SYS_PARA_F1;

typedef struct tag_Sys_Para_F2
{
    UCHAR   buf[MAX_BUF_SIZE];
    USHORT  len;
}SYS_PARA_F2;

typedef struct tag_Terminal_Run_Para
{
    RUN_PARA_F1 runF1;
    RUN_PARA_F2 runF2;
    RUN_PARA_F3 runF3;
    RUN_PARA_F4 runF4;
    RUN_PARA_F5 runF5;
}TMN_RUN_PARA;

typedef struct tag_Terminal_Sys_Para
{
    SYS_PARA_F1 sysF1;
    SYS_PARA_F2 sysF2;
}TMN_SYS_PARA;


typedef struct tag_Progress_Control_Command
{
    USHORT              tail;
    USHORT              count[MAX_P_ID_NUM];
    USHORT              head[MAX_P_ID_NUM];             /* 以进程为单位的参数更新标志 */;
    CTL_CMD_BUF         ctlCmdBuf[CMD_BUF_MAX_NUM];
}PGS_CTL_CMD;

typedef struct tag_Para_Share_Memory_Data
{
    TMN_UPDARE_PARA     tmnUpdataPara[MAX_P_ID_NUM];    /* 以进程为单位的参数更新标志 */
    TMN_SYS_PARA        tmnSysPara;
    TMN_RUN_PARA        tmnRunPara;
}PARA_SHM_DATA;

typedef struct tag_Para_Info
{
    int     offSetof;
    int     size;
    char    name[MAX_PARA_NAME_LEN]; 
}PARA_INFO;

#define MAX_PARA_ID_NUM             7

typedef enum tag_Para_Id_Index
{
    PARA_ID_RUN_F1 = 0,
    PARA_ID_RUN_F2 = 1,
    PARA_ID_RUN_F3 = 2,
    PARA_ID_RUN_F4 = 3,
    PARA_ID_RUN_F5 = 4,
    PARA_ID_SYS_F1 = 5,
    PARA_ID_SYS_F2 = 6,
}PARA_ID_INDEX;

static const PARA_INFO paraInfo[MAX_PARA_ID_NUM] = {
    offsetof(PARA_SHM_DATA,tmnRunPara.runF1),sizeof(RUN_PARA_F1),"runF1",
    offsetof(PARA_SHM_DATA,tmnRunPara.runF2),sizeof(RUN_PARA_F2),"runF2",
    offsetof(PARA_SHM_DATA,tmnRunPara.runF3),sizeof(RUN_PARA_F3),"runF3",
    offsetof(PARA_SHM_DATA,tmnRunPara.runF4),sizeof(RUN_PARA_F4),"runF4",
    offsetof(PARA_SHM_DATA,tmnRunPara.runF5),sizeof(RUN_PARA_F5),"runF5",
    offsetof(PARA_SHM_DATA,tmnSysPara.sysF1),sizeof(SYS_PARA_F1),"sysF1",
    offsetof(PARA_SHM_DATA,tmnSysPara.sysF2),sizeof(SYS_PARA_F2),"sysF2",
};

typedef struct tag_Para_Update_Info
{
    int     offSetof;
    int     size;
}PARA_UPDATE_INFO;

static const PARA_UPDATE_INFO paraUpdateFlag[MAX_P_ID_NUM] = {
    offsetof(PARA_SHM_DATA,tmnUpdataPara[0]),sizeof(TMN_UPDARE_PARA),
    offsetof(PARA_SHM_DATA,tmnUpdataPara[1]),sizeof(TMN_UPDARE_PARA),
    offsetof(PARA_SHM_DATA,tmnUpdataPara[2]),sizeof(TMN_UPDARE_PARA),
    offsetof(PARA_SHM_DATA,tmnUpdataPara[3]),sizeof(TMN_UPDARE_PARA),
    offsetof(PARA_SHM_DATA,tmnUpdataPara[4]),sizeof(TMN_UPDARE_PARA),
};

#endif