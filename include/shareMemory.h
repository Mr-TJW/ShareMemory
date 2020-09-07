#ifndef	_SHAREMEMORY_H
#define	_SHAREMEMORY_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "public.h"
#include "parameter.h"

#define	PARA_SHM_KEY				0x20000001			/*参数共享内存Key值, 四字节*/
#define	CMD_SHM_KEY				    0x20000002

union   semun 
{ 
    int                  val; 
    struct   semid_ds   *buf; 
    unsigned short int  *array; 
    struct   seminfo    *__buf; 
};

class paraShmOper
{
public:
    paraShmOper(int key,int size);
    ~paraShmOper();
private:
    int     shmKey;
    int     shmSize;
    int     shmId;
    int     semId;
    char	*shmAddr;
private:
    int shmInit(key_t key, int shmsize, char **pAddr);
    int shmDel(int id);

    int semInit(key_t key);
    int semSetValue(int id,int val);
    int semP(int id);
    int semV(int id);
    int semDel(int id);
public:
    int getShmPara(char *paraData,USHORT paraId);
    int putShmPara(char *paraData,USHORT paraId);
    int putShmUpdateFlag(USHORT paraId);
    int checkShmUpdateFlag(UCHAR progress_id,USHORT paraId);
    int readCmd(UCHAR userNo,UCHAR *cmdBuf);
    int writeCmd(UCHAR *cmdBuf);
};



#endif