#include "tcp_client.h"

 C_TcpClient:: C_TcpClient(HOST_INFO *hostinfo)
{
    initial(hostinfo);
}
 C_TcpClient:: C_TcpClient()
{

}

/*******************************************************************
* funcname:	initial
* para:	    hostinfo-主用信息
* function:	tcp客户端初始化
* return:	
********************************************************************/
void C_TcpClient::initial(HOST_INFO *hostinfo)
{   
    char ip[32];

    host_info = hostinfo;
    data_queue_initial(&host_info->rec_from_com_queue); 
    data_queue_initial(&host_info->send_to_com_queue);

    socket_fd = -1;
    link_state = !LINK_OK;
    host_info->net_link_state = !LINK_OK;

    /* 主站信息初始化 */
    sprintf(ip,"%d.%d.%d.%d",host_info->master_ip[0],host_info->master_ip[1],
        host_info->master_ip[2],host_info->master_ip[3]);
    Host_addr.sin_family = AF_INET;
    Host_addr.sin_port = htons(host_info->master_port);
    Host_addr.sin_addr.s_addr = inet_addr(ip);
    bzero(&(Host_addr.sin_zero),8);
}

void C_TcpClient::deal()
{	
	if((link_state != LINK_OK) || (host_info->net_link_state != LINK_OK))
	{
		create_connect();
	}
	else
	{	
		recv_data();
		send_data();		
	}
}	

/*******************************************************************
* funcname:	create_connect
* para:	
* function:	创建 tcp 链接
* return:	
********************************************************************/
void C_TcpClient::create_connect()
{      
    int ret;
    struct timeval timeo = {5, 0};
	int len = sizeof(timeo);

    if(socket_fd == -1)
    {
        if ( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        {	
            printf("%s\r\n",strerror(errno));
            perror("Error connecting to socket\n");
            return ;
        }
        else
        {
            printf("<<<<<<<<<Net Mode TCP>>>>>>>>>\r\n");
        }
        
        if(setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeo, len) != 0)
        {   
            printf("%s\r\n",strerror(errno));
            close_connect();
            return;
        }
    }
    else
    {
        close_connect();
        return;
    }
    

    ret = connect(socket_fd,(struct sockaddr *)&Host_addr,sizeof(struct sockaddr));
    if(ret != -1)
    {
        printf("net connect ok\r\n");
        link_state = LINK_OK;
        host_info->net_link_state = LINK_OK;
    }
    else
    {
        printf("%s\r\n",strerror(errno));
    }
    
}

/*******************************************************************
* funcname:	close_connect
* para:	
* function:	关闭tcp连接
* return:	
********************************************************************/
void C_TcpClient::close_connect()
{   
    close(socket_fd);
    socket_fd = -1;
    link_state = !LINK_OK;
    host_info->net_link_state = !LINK_OK;
}

/*******************************************************************
* funcname:	send_data
* para:	
* function:	tcp 发送
* return:	
********************************************************************/
void C_TcpClient::send_data()
{   
    int     i;
    time_t  tp;
    struct  tm * lt;	
    USHORT  send_num;
    UCHAR   temp_buf[DATA_QUEUE_BUF_SIZE];

    if((send_num = host_info->send_to_com_queue.count) > 0)
    {   
  /*      
        printf("\r\n-------------------打印队列-----------------------\r\n");
        for(int i=0;i<host_info->send_to_com_queue.size;i++)
        {
            printf("%02X ",host_info->send_to_com_queue.buf[i]);
        }
        printf("\r\n-------------------打印队列-----------------------\r\n\r\n");
*/
        for(i=0;i<send_num;i++)
        {   
            temp_buf[i] = host_info->send_to_com_queue.buf[mod(host_info->send_to_com_queue.start+i,
                host_info->send_to_com_queue.size)];
        }
        send_num = i;
    }

    if(send_num > 0)
    {
        if(send(socket_fd,temp_buf,send_num,0) > 0)
        {
			time(&tp);
			lt=localtime(&tp);	

			printf("\r\nnet send time= %d年%d月%d日%d时%d分%d秒\r\n",(lt->tm_year-100+2000),
                (lt->tm_mon+1),lt->tm_mday,lt->tm_hour,lt->tm_min,lt->tm_sec);	
			printf("send byte = %d \r\n",send_num );

            for(i=0;i<send_num;i++)
            {
                printf("%02X",temp_buf[i]);
                drop_data(&host_info->send_to_com_queue);
            }
            printf("\n\n");
        }
        else
        {
            close_connect();
        }
    }
}

/*******************************************************************
* funcname:	recv_data
* para:	
* function:	tcp 接受
* return:	
********************************************************************/
void C_TcpClient::recv_data()
{   
    int     i,ret;
    time_t  tp;
    struct  tm * lt;	
    USHORT  recv_len;
    UCHAR   temp_buf[DATA_QUEUE_BUF_SIZE];
    fd_set  readskt;
	struct  timeval time_out;

	FD_ZERO(&readskt);
	time_out.tv_sec =0;
	time_out.tv_usec = 10*1000;
	FD_SET(socket_fd,&readskt);
			
	ret = select(socket_fd+1,&readskt,NULL,NULL,&time_out);
	if (ret > 0)
    {   
        recv_len = recv(socket_fd, temp_buf,DATA_QUEUE_BUF_SIZE,0);
        if(recv_len > 0)
        {   
            if(recv_len > DATA_QUEUE_BUF_SIZE)
            {
                printf("tcp 接收字节: %d,缓冲区长度：%d, 超出缓冲区，溢出，不处理\r\n",recv_len,DATA_QUEUE_BUF_SIZE);
                return;
            }
            time(&tp);
			lt=localtime(&tp);	

			printf("\r\nnet recv time= %d年%d月%d日%d时%d分%d秒\r\n",(lt->tm_year-100+2000),
                (lt->tm_mon+1),lt->tm_mday,lt->tm_hour,lt->tm_min,lt->tm_sec);	
			printf("recv byte = %d \r\n",recv_len );

            for(i=0;i<recv_len;i++)
            {
                printf("%02X",temp_buf[i]);
                put_data(&host_info->rec_from_com_queue,temp_buf[i]);
            }
            printf("\n\n");
        }
        else if(ret == 0)
        {
            printf("接收过程中网络中断，关闭 tcp 连接 \r\n");
            close_connect();
        }
        else
        {
            printf("%s\r\n",strerror(errno));
            close_connect();
        }
    }
    else if(ret < 0)
    {
        printf("recv err 2 select socket< 0!\r\n" );
        close_connect();
    }
}