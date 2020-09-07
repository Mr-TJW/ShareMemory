/*
*********************************************************************
* Copyright(C) 2020 南瑞用电-终端部
* All rights reserved. 
* @filename: main.cpp 
* @brief:	 
* @name：	 Mr-TJW
* @date：	 2020-07-16
* @history：
*********************************************************************
*/

#include "test.h"


/*******************************************************************
* funcname:	main
* para:	
* function:	主函数
* return:	
********************************************************************/
int main(void)
{   
    int i;
    paraShmOper shmOper(PARA_SHM_KEY,sizeof(PARA_SHM_DATA));
    paraShmOper shmCmd(CMD_SHM_KEY,sizeof(PGS_CTL_CMD));
    RUN_PARA_F1 runF1;
    CTL_CMD_BUF cmdBuf;
    time_t          curtime;

    memset(&cmdBuf,0,sizeof(CTL_CMD_BUF));

    while(1)
	{   
      /*  
        if(shmOper.checkShmUpdateFlag(P_ID_TEST1,PARA_ID_RUN_F1) > 0)
        {
            if(shmOper.getShmPara((char *)&runF1,PARA_ID_RUN_F1) > 0)
            {   
                curtime = time(NULL);
                printf("%sshmData len = %d\r\n",asctime(localtime(&curtime)),runF1.len);
                for(i=0;i<runF1.len;i++)
                {
                    printf("%02X ",runF1.buf[i]);
                }
                printf("\n");
            }
        }   
        */
        if(shmCmd.readCmd(1,(UCHAR *)&cmdBuf) > 0)
        {
            curtime = time(NULL);
            printf("%scmd len = %d\r\n",asctime(localtime(&curtime)),cmdBuf.cmdLen);
            for(i=0;i<cmdBuf.cmdLen;i++)
            {
                printf("%02X ",cmdBuf.cmdBuf[i]);
            }
            printf("\n");
        } 
    } 
}