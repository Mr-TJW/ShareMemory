#ifndef	_TCP_CLIENT_H
#define	_TCP_CLIENT_H

#include "public.h"
#include "config.h"

class C_TcpClient : public C_DataQueue
{
private:
    int     socket_fd;
    UCHAR   link_state;
    struct sockaddr_in Host_addr;
public:
    C_TcpClient(HOST_INFO *hostinfo);
    C_TcpClient();
public:
    HOST_INFO *host_info;
private:
    void initial(HOST_INFO *hostinfo);
    void send_data();
    void recv_data();
    void create_connect();
    void close_connect();
public:
    void deal();
};

#endif