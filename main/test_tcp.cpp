/*
*********************************************************************
* Copyright(C) 2020 南瑞用电-终端部
* All rights reserved. 
* @filename: test.cpp 
* @brief:	 用于测试
* @name：	 Mr-TJW
* @date：	 2020-09-01
* @history：
*********************************************************************
*/
#include "test_tcp.h"

C_TEST::C_TEST(HOST_INFO *host_info)
{
    hostInfo = host_info;
    out_msg_len = 0;
}

C_TEST::~C_TEST()
{

}

/*******************************************************************
* funcname:	send_com
* para:	    buf-发送数据，send_len-数据长度
* function:	发送数据
* return:	
********************************************************************/
void  C_TEST::send_com()
{   
    if((hostInfo->send_to_com_queue.size - hostInfo->send_to_com_queue.count) > out_msg_len)
    {
        for(int i=0;i<out_msg_len;i++)
        {             
            put_data(&hostInfo->send_to_com_queue,msg_out_buf[i]);
        }  
        release_msg_buf(MSG_TYPE_SEND);
    }
    else
    {
        printf("发送队列满\r\n");
    }
    
}


/*******************************************************************
* funcname:	read_msg
* para:	    TempBuf-报文，FrameLen-报文长度
* function:	读取698报文
* return:	响应字节数
********************************************************************/
void C_TEST::read_msg()
{   
    int             i;
	int             ret=-1;
    time_t          curtime = time(NULL);
    CTL_CMD_BUF     cmdBuf;
    S_DATA_QUEUE    *rec_queue = &hostInfo->rec_from_com_queue;

    memset(&cmdBuf,0,sizeof(CTL_CMD_BUF));

    if(hostInfo->cmdShm->readCmd(0,(UCHAR *)&cmdBuf) > 0)
    {
        printf("%scmd recv byte = %d \n",asctime(localtime(&curtime)),cmdBuf.cmdLen);
        for(i=0;i<cmdBuf.cmdLen;i++)
        {
            printf("%02X ",cmdBuf.cmdBuf[i]);
        }
        printf("\r\n");
    }

    in_msg_len = 0;
    if(rec_queue->count > 0)
    {   
        in_msg_len = rec_queue->count;
        printf("%s tcp recv byte = %d \n",asctime(localtime(&curtime)),rec_queue->count);
        for(i=0; i < in_msg_len; i++)
        {  
            msg_in_buf[i] = rec_queue->buf[rec_queue->start];
            drop_data(rec_queue);
            printf("%02X ",msg_in_buf[i]);
        }
        printf("\n");
    }
    

    /*APDU处理*/
    deal_msg();
    release_msg_buf(MSG_TYPE_RECV);
}

/*******************************************************************
* funcname:	release_recv_buf
* para:	
* function:	清除接收缓冲
* return:	
********************************************************************/
void C_TEST::release_msg_buf(UCHAR msg_type)
{   
    if(msg_type == MSG_TYPE_RECV)
    {
        in_msg_len = 0;
    }
    else if(msg_type == MSG_TYPE_SEND)
    {
        out_msg_len = 0;
    }
    
}

/*******************************************************************
* funcname:	deal_msg
* para:	
* function:	处理消息
* return:	
********************************************************************/
void C_TEST::deal_msg()
{   
    USHORT  i;
    RUN_PARA_F1 runF1;
    CTL_CMD_BUF cmdBuf;

    memset(&runF1,0,sizeof(RUN_PARA_F1));

    if(in_msg_len > 0)
    {   
        runF1.len = in_msg_len;
        cmdBuf.cmdLen = in_msg_len;

        memcpy(runF1.buf,msg_in_buf,in_msg_len);
        memcpy(cmdBuf.cmdBuf,msg_in_buf,in_msg_len);

        printf("paraShmData.len = %d\r\n",runF1.len);
        for(i=0;i<runF1.len;i++)
        {
            printf("%02X ",runF1.buf[i]);
        }
        printf("\n");

        //hostInfo->paraShm->putShmPara((char *)&runF1,PARA_ID_RUN_F1);
        hostInfo->cmdShm->writeCmd((UCHAR *)&cmdBuf);
        
        memset(&runF1,0,sizeof(runF1));
        
        //out_msg_len = in_msg_len;
        //memcpy(msg_out_buf,msg_in_buf,out_msg_len);
    }
}

/*******************************************************************
* funcname:	deal
* para:	
* function:	C_TEST入口
* return:	
********************************************************************/
void C_TEST::deal()
{
    curtime = time(NULL);
    read_msg();
    send_com();
}