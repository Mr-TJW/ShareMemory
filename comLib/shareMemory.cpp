/*
*********************************************************************
* Copyright(C) 2020 南瑞用电-终端部
* All rights reserved. 
* @filename: shareMemory.cpp 
* @brief:	 共享内存
* @name：	 Mr-TJW
* @date：	 2020-09-01
* @history：
*********************************************************************
*/

#include "shareMemory.h"


/***************************************** shmget 函数 *******************************************************
* int shmget(key_t key, size_t size, int shmflg);
* key:  它有效地为共享内存段命名，不同进程指定相同的键值，可以获取同一块内存区域
*       0(IPC_PRIVATE)：会建立新共享内存对象
        大于0的32位整数：视参数shmflg来确定操作，几个进程采用相同键值来获取同一段内存
* size: 共享存储段的字节数
* flag: 读写的权限还有IPC_CREAT或IPC_EXCL对应文件的O_CREAT或O_EXCL
*       IPC_CREAT，当只有此选项打开时,不管是否已存在该块共享内存，都返回该共享内存的ID，若不存在则创建共享内存 
*       IPC_EXCL，当只有此选项打开时，不管有没有该快共享内存，shmget()都返回-1 
*       IPC_CREAT|IPC_EXCL 如果没有该块共享内存，则创建，并返回共享内存ID。若已有该块共享内存，则返回-1 
*       0666 : 6,表示可读可写，第一个0是 UID,第一个6表示拥有者的权限，第二个6表示同组权限，第3个6表示他人的权限。 
* 返回值：  成功返回共享存储的id，失败返回-1
**************************************************************************************************************/

/***************************************** shmat 函数 *******************************************************
* 作用：    shmget创建完共享内存时，它还不能被任何进程访问，shmat()函数的作用就是用来启动对该共享内存的访问，
*           并把共享内存连接到当前进程的地址空间。
* 原型：    void *shmat(int shm_id, const void *shm_addr, int shmflg);
* 参数：    shm_id:   由shmget()函数返回的共享内存标识。
*           shm_addr: 指定共享内存连接到当前进程中的地址位置，通常为空，表示让系统来选择共享内存的地址。
*           shm_flg:  是一组标志位，通常为0
* 返回值：  成功时返回一个指向共享内存第一个字节的指针，如果调用失败返回-1.
**************************************************************************************************************/

/***************************************** shmdt 函数 *******************************************************
* 作用：    将共享内存从当前进程中分离。注意，将共享内存分离并不是删除它，只是使该共享内存对当前进程不再可用
* 原型：    int shmctl(int shm_id, int command, struct shmid_ds *buf);
* 参数：    shmaddr:shmat()函数返回的地址指针
* 返回值：  调用成功时返回0，失败时返回-1.
**************************************************************************************************************/

/***************************************** shmctl 函数 *******************************************************
* 作用：    将共享内存从当前进程中分离。注意，将共享内存分离并不是删除它，只是使该共享内存对当前进程不再可用
* 原型：    int shmctl(int shm_id, int command, struct shmid_ds *buf);
* 参数：    shm_id 共享内存标识
*           command 所要采取的操作，可以取下面三个值
*                   IPC_STAT：把shmid_ds结构中的数据设置为共享内存的当前关联值，即用共享内存的当前关联值覆盖shmid_ds的值。
*                   IPC_SET：如果进程有足够的权限，就把共享内存的当前关联值设置为shmid_ds结构中给出的值
*                   IPC_RMID：删除共享内存段
*           buf 是一个结构指针，它指向共享内存模式和访问权限的结构。
*                   struct shmid_ds
*                    {
*                        uid_t shm_perm.uid;
*                        uid_t shm_perm.gid;
*                        mode_t shm_perm.mode;
*                    };
* 返回值：  调用成功时返回0，失败时返回-1.
**************************************************************************************************************/

/************************ 信号量工作原理 ***********************************************************************
* 由于信号量只能进行两种操作等待和发送信号，即P(sv)和V(sv),他们的行为是这样的：
* （1）P(sv)：如果sv的值大于零，就给它减1；如果它的值为零，就挂起该进程的执行
* （2）V(sv)：如果有其他进程因等待sv而被挂起，就让它恢复运行，如果没有进程因等待sv而挂起，就给它加1.
* 在信号量进行PV操作时都为原子操作（因为它需要保护临界资源）
* 注：原子操作：单指令的操作称为原子的，单条指令的执行是不会被打断的
***************************************************************************************************************/

/************************ semget 函数 ****************************************************************************
* 作用：创建一个新的信号量或获取一个已经存在的信号量的键值
* 原型：int  semget(key_t key ,int  nsems,int semflg);
* 参数：key     为整型值，用户可以自己设定。有两种情况
*               IPC_PRIVATE，该值通常为0，意思就是创建一个仅能被进程进程给我的信号量。
*               非 IPC_PRIVATE，我们可以指定键值，例如1234；也可以一个ftok()函数来取得一个唯一的键值。
*       nsems   进行操作信号量的个数，即sops结构变量的个数，需大于或等于1。最常见设置此值等于1，只完成对一个信号量的操作
*       semflg  信号量的创建方式或权限。有IPC_CREAT，IPC_EXCL。
**************************************************************************************************************/


/************************ semop 函数 ****************************************************************************
* int semop(int semid, struct sembuf *sops, unsigned nsops)
*  struct sembuf 
*  {
*      short sem_num;   //信号量集合中的信号量编号，0代表第1个信号量
*      short sem_op;   //sem_op>0,进行V操作信号量值加 val，表示进程释放控制的资源
*                      //sem_op<0,进行P操作信号量值减val，若(semval-val)<0（semval为该信号量值），则调用进程阻塞，直到资源可用；若设置IPC_NOWAIT不会睡眠，进程直接返回EAGAIN错误
*                      //sem_op==0,时阻塞等待信号量为0，调用进程进入睡眠状态，直到信号值为0；若设置IPC_NOWAIT，进程不会睡眠，直接返回EAGAIN错误
*       short sem_flg; //设置信号量的默认操作
                       //0，阻塞进程，直到信号量的值为0
*                      //IPC_NOWAIT 不阻塞进程，直接返回-1，错误码为 EAGAIN
*                      //SEM_UNDO 选项会让内核记录一个与调用进程相关的UNDO记录，如果该进程崩溃，则根据这个进程的UNDO记录自动恢复相应信号量的计数值
*  }
* nsops：进行操作信号量的个数，即sops结构变量的个数，需大于或等于1。最常见设置此值等于1，只完成对一个信号量的操作
**************************************************************************************************************/

/************************ semctl 函数 ****************************************************************************
* int semctl(int semid,int semnum,int cmd, union semun arg);
* semnum : 信号量集合中的信号量编号，0代表第1个信号量
* cmd : SETVAL：用来把信号量初始化为一个已知的值。p 这个值通过union semun中的val成员设置，其作用是在信号量第一次使用前对它进行设置。
*       IPC_RMID：用于删除一个已经无需继续使用的信号量标识符。
*  union semun
*  {
*         int              val;      // SETVAL使用的值   
*         struct semid_ds  *buf;    // IPC_STAT、IPC_SET 使用缓存区
*         unsigned short   *array;  // GETALL,、SETALL 使用的数组 
*         struct seminfo   *__buf;  // IPC_INFO(Linux特有) 使用缓存区 
*   };   
*    struct semid_ds 
*   { 
*       struct ipc_perm sem_perm;   // 所有者和权限
*       time_t          sem_otime;  // 上次执行 semop 的时间  
*       time_t          sem_ctime;  // 上次更新时间 
*       unsigned short  sem_nsems;  // 在信号量集合里的索引
*   };
* 返回值：失败时 semctl() 返回 -1 并设置 errno 指明错误。成功返回cmd相应返回值
**************************************************************************************************************/

paraShmOper::paraShmOper(int key,int size)
{
    shmKey = (key_t) key;
    shmSize = size;     //sizeof(PARA_SHM_DATA);
    if(-1 == (shmId = shmInit(shmKey, shmSize,&shmAddr)))
    {
        printf("shmId get -1\r\n");
    }

    if(-1 == (semId = semInit(shmKey)))
    {
        printf("semId get -1\r\n");
    }
    printf("shmId = %d,shmAddr = %p\r\n",shmId,shmAddr);
}

paraShmOper::~paraShmOper()
{
    shmdt(shmAddr);
}


/*******************************************************************
* funcname:	shmInit
* para:	    key 共享内存键值
*           shmsize 共享内存大小
*           addr获取到的共享内存地址
* function:	共享内存初始化，获取共享内存
* return:	成功：共享内存标识符，失败：-1 (-1)
********************************************************************/
int paraShmOper::shmInit(key_t key,int shmsize,char **addr)
{   
    int id;

    if(addr == NULL) return -1;

    /* 创建共享内存 */
    if(-1 == (id = shmget(key,shmsize,0666|IPC_CREAT|IPC_EXCL)))     //判断创建的共享内存是否已存在
    {
        if(errno == EEXIST) //此共享内存已存在
        {
            if(-1 == (id = shmget(key,shmsize,0666|IPC_CREAT)))
            {
                perror("shmInit shmget 1:");
            }
            else
            {
                if((char *)-1 == (*addr = (char *)shmat(id,0,0)))   //把共享内存区对象映射到调用进程的地址空间（启用此块共享内存）
                {
                    perror("shmInit shmat 1:");
                    id = -1;
                }
            }
        }
        else
        {
            perror("shmInit shmget 2:");
        }
    }
    else    //此共享内存是新创建的
    {
        if((char *)-1 == (*addr = (char *)shmat(id,0,0)))
        {
            perror("shmInit shmat 2:");
            id = -1;
        }
        else
        {
            memset(*addr,0,shmsize);    //首次创建的共享内存需要初始化
        }
    }
    return id;
}

/*******************************************************************
* funcname:	shmDel
* para:	
* function:	删除指定共享内存
* return:	成功1,失败-1
********************************************************************/
int paraShmOper::shmDel(int id)
{
    if(-1 == id) return -1;

    if(-1 == shmctl(id,IPC_RMID,NULL))  //IPC_RMID 删除共享内存
    {
        perror("shmInit shmctl:");
        return -1;
    }
    return 1;
}

/*******************************************************************
* funcname:	semInit
* para:	    key 
* function:	信号量初始化，一开始给其V造作，将信号量值置为 1
* return:	成功：信号量标识符，失败：-1 (-1)
********************************************************************/
int paraShmOper::semInit(key_t key)
{
    int id = -1;
    struct sembuf   sbuf;

    /* int semget(key_t key, int nsems, int semflg) */
    /* nsems 创建信号量集中信号量的个数 */
    /* semget 与 shmget 类似 */
    if(-1 == (id = semget(key,1,0666|IPC_CREAT|IPC_EXCL)))
    {
        if(errno == EEXIST)
        {
            if(-1 == (id = semget(key,1,0666|IPC_CREAT)))
            {
                perror("semInit semget 1:");
            }
        }
        else
        {
            perror("semInit semget 2:");
        }
    }
    else
    {
        if(-1 == semSetValue(id,1))
        {
            perror("semInit semop:");
            id = -1;
        }
    }
    return id;
}

/*******************************************************************
* funcname:	semSetValue
* para:	    val 所设置的信号量的值
* function:	初始化信号量初始值
* return:	成功1 失败-1
********************************************************************/
int paraShmOper::semSetValue(int id,int val)
{   
    union semun sem_union;

    if(-1 == id) return -1;

    sem_union.val = val;
    if(-1 == semctl(id,0,SETVAL,sem_union)) return -1;

    return 1;
}

/*******************************************************************
* funcname:	semP
* para:	    id 信号量标识符
* function:	占用信号量，如果信号量的值大于零，就给它减 1；如果它的值为零，就挂起该进程的执行，等待信号量值为1
* return:	成功返回 SUCCSEE，失败返回 -1
********************************************************************/
int paraShmOper::semP(int id)
{   
    struct sembuf sbuf;

    if(id < 0) return -1;

    sbuf.sem_num = 0;   //信号量编号，0代表第一个
    sbuf.sem_op  = -1;   //P操作，占用信号量
    sbuf.sem_flg = SEM_UNDO;  //0，阻塞进程，直到信号量的值为0

    if(-1 == semop(id,&sbuf,1))
    {
        perror("semP :");
        return -1;
    }
    return 1;
}

/*******************************************************************
* funcname:	semV
* para:	    id 信号量标识符
* function:	释放信号量
* return:	成功返回 SUCCSEE，失败返回 -1
********************************************************************/
int paraShmOper::semV(int id)
{   
    struct sembuf sbuf;

    if(id < 0) return -1;

    sbuf.sem_num = 0;   //信号量编号，0代表第一个
    sbuf.sem_op  = 1;   //V操作，释放信号量
    sbuf.sem_flg = SEM_UNDO;  //0，阻塞进程，直到信号量的值为0

    if(-1 == semop(id,&sbuf,1))
    {
        perror("semV :");
        return -1;
    }
    return 1;
}

/*******************************************************************
* funcname:	semDel
* para:	    id 信号量标识符
* function:	删除指定信号量
* return:	成功返回 SUCCSEE，失败返回 -1
********************************************************************/
int paraShmOper::semDel(int id)
{
    if(id < 0) return -1;

    if(-1 == semctl(id,0,IPC_RMID,NULL)) return -1;

    return 1;
}

/*******************************************************************
* funcname:	getShmPara
* para:	    paraData 数据缓冲区
* function:	获取共享内存中的数据
* return:	成功1 失败-1
********************************************************************/
int paraShmOper::getShmPara(char *paraData,USHORT paraId)
{
    if((-1 == shmId)) 
    {
        printf("getShmPara shmId init error\n");
        return -1;
    }

    if((paraId >= MAX_PARA_ID_NUM)) 
    {
        printf("putShmPara paraId exceed MAX_PARA_ID_NUM\n");
        return -1;
    }

    if(NULL == paraData)
    {
        printf("getShmPara paraData is NULL\n");
        return -1;
    }

    if(-1 == semP(semId))
    {
        printf("getShmPara semP error,semId = %d\n",semId);
        return -1;
    }

    memcpy(paraData,shmAddr+paraInfo[paraId].offSetof,paraInfo[paraId].size);

    if(-1 == semV(semId))
    {
        printf("getShmPara semV error,semId = %d\n",semId);
        return -1;
    }

    return 1;
}


/*******************************************************************
* funcname:	putShmPara
* para:	    paraData 数据缓冲区
* function:	获取共享内存中的数据
* return:	成功1 失败-1
********************************************************************/
int paraShmOper::putShmPara(char *paraData,USHORT paraId)
{
    if((-1 == shmId)) 
    {
        printf("putShmPara shmId init error\n");
        return -1;
    }

    if((paraId >= MAX_PARA_ID_NUM)) 
    {
        printf("putShmPara paraId exceed MAX_PARA_ID_NUM\n");
        return -1;
    }

    if(NULL == paraData)
    {
        printf("putShmPara paraData is NULL\n");
        return -1;
    }

    if(-1 == semP(semId))
    {
        printf("putShmPara semP error,semId = %d\n",semId);
        return -1;
    }

    memcpy(shmAddr+paraInfo[paraId].offSetof,paraData,paraInfo[paraId].size);

    if(-1 == semV(semId))
    {
        printf("putShmPara semV error,semId = %d\n",semId);
        return -1;
    }

    if(-1 == putShmUpdateFlag(paraId))
    {
        printf("putShmUpdateFlag error,paraId = %d\n",paraId);
        return -1;
    }
    
    return 1;
}

/*******************************************************************
* funcname:	writeCmd
* para:	
* function:	读取Cmd内容
* return:	
********************************************************************/
int paraShmOper::writeCmd(UCHAR *cmdBuf)
{   
    int i;

    if((-1 == shmId)) 
    {
        printf("putShmPara shmId init error\n");
        return -1;
    }

    if(NULL == cmdBuf)
    {
        printf("putShmPara paraData is NULL\n");
        return -1;
    }

    if(-1 == semP(semId))
    {
        printf("putShmPara semP error,semId = %d\n",semId);
        return -1;
    }
    
    PGS_CTL_CMD *ctlCmdBuf = (PGS_CTL_CMD *)shmAddr;
    memcpy(&ctlCmdBuf->ctlCmdBuf[ctlCmdBuf->tail],cmdBuf,sizeof(CTL_CMD_BUF));

    ctlCmdBuf->tail++;
    if(ctlCmdBuf->tail >= CMD_BUF_MAX_NUM)
    {
        ctlCmdBuf->tail = 0;
    }

    for(i=0;i<MAX_P_ID_NUM;i++)
    {
        ctlCmdBuf->count[i]++;
        if(ctlCmdBuf->count[i] >= CMD_BUF_MAX_NUM)
        {
            ctlCmdBuf->count[i] = CMD_BUF_MAX_NUM;
            ctlCmdBuf->head[i]++;
            if(ctlCmdBuf->head[i] >= CMD_BUF_MAX_NUM)
            {
                ctlCmdBuf->head[i] = 0;
            }
        }
    }

    if(-1 == semV(semId))
    {
        printf("writeCmd semP error,semId = %d\n",semId);
        return -1;
    }
    return 1;
}

/*******************************************************************
* funcname:	readCmd
* para:	
* function:	读取Cmd内容
* return:	
********************************************************************/
int paraShmOper::readCmd(UCHAR userNo,UCHAR *cmdBuf)
{   
    int result = -1;
    if((-1 == shmId)) 
    {
        printf("readCmd shmId init error\n");
        return -1;
    }

    if(NULL == cmdBuf)
    {
        printf("readCmd paraData is NULL\n");
        return -1;
    }

    if(-1 == semP(semId))
    {
        printf("readCmd semP error,semId = %d\n",semId);
        return -1;
    }

    PGS_CTL_CMD *ctlCmdBuf = (PGS_CTL_CMD *)shmAddr;

    if(ctlCmdBuf->count[userNo] > 0)
    {
        memcpy(cmdBuf,&ctlCmdBuf->ctlCmdBuf[ctlCmdBuf->head[userNo]],sizeof(CTL_CMD_BUF));
        ctlCmdBuf->head[userNo]++;
        if(ctlCmdBuf->head[userNo] >= CMD_BUF_MAX_NUM)
        {
            ctlCmdBuf->head[userNo] = 0;
        }

        ctlCmdBuf->count[userNo]--;
        result = 1;
        printf("head = %d\n",ctlCmdBuf->head[userNo]);
        printf("count = %d\n",ctlCmdBuf->count[userNo]);
        printf("userNo = %d\r\n",userNo);
    }

    if(-1 == semV(semId))
    {
        printf("readCmd semP error,semId = %d\n",semId);
        return -1;
    }
    
    return result;
}

/*******************************************************************
* funcname:	putShmUpdateFlag
* para:	    paraId c参数索引
* function:	
* return:	
********************************************************************/
int paraShmOper::putShmUpdateFlag(USHORT paraId)
{   
    int i;
    TMN_UPDARE_PARA     *tmnUpdataPara;

    if((-1 == shmId)) 
    {
        printf("putShmUpdateFlag shmId init error\n");
        return -1;
    }

    if((paraId >= MAX_PARA_ID_NUM)) 
    {
        printf("putShmUpdateFlag paraId exceed MAX_PARA_ID_NUM\n");
        return -1;
    }

    if(-1 == semP(semId))
    {
        printf("putShmUpdateFlag semP error,semId = %d\n",semId);
        return -1;
    }

    for(i=0;i<MAX_P_ID_NUM;i++)
    {
        tmnUpdataPara = (TMN_UPDARE_PARA *)(shmAddr + paraUpdateFlag[i].offSetof);
        tmnUpdataPara->paraIsUpdataFlag[paraId] = 1;
    }
   
    if(-1 == semV(semId))
    {
        printf("putShmUpdateFlag semV error,semId = %d\n",semId);
        return -1;
    }

    return 1;
}

/*******************************************************************
* funcname:	checkShmUpdateFlag
* para:	    progress_id 进程 id,paraId 参数索引
* function:	
* return:	
********************************************************************/
int paraShmOper::checkShmUpdateFlag(UCHAR progress_id,USHORT paraId)
{   
    int i,result = -1;
    TMN_UPDARE_PARA     *tmnUpdataPara;

    if((-1 == shmId)) 
    {
        printf("checkShmUpdateFlag shmId init error\n");
        return -1;
    }

    if((paraId >= MAX_PARA_ID_NUM)) 
    {
        printf("checkShmUpdateFlag paraId exceed MAX_PARA_ID_NUM\n");
        return -1;
    }

    if(-1 == semP(semId))
    {
        printf("checkShmUpdateFlag semP error,semId = %d\n",semId);
        return -1;
    }

    tmnUpdataPara = (TMN_UPDARE_PARA *)(shmAddr + paraUpdateFlag[progress_id].offSetof);

    if(1 == tmnUpdataPara->paraIsUpdataFlag[paraId])
    {
        tmnUpdataPara->paraIsUpdataFlag[paraId] = 0;
        result = 1;
    }

    if(-1 == semV(semId))
    {
        printf("checkShmUpdateFlag semV error,semId = %d\n",semId);
        return -1;
    }

    return result;
}