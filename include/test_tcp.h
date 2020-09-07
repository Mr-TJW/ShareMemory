#ifndef	_TEST_TCP_H
#define	_TEST_TCP_H

#include "public.h"
#include "config.h"
#include "tcp_client.h"

#define MSG_BUF_LEN     2048

#define MSG_TYPE_RECV   1
#define MSG_TYPE_SEND   2

class C_TEST : public C_DataQueue
{
public:
    C_TEST(HOST_INFO *host_info);
    ~C_TEST();
public:
    void    deal();
private:
    HOST_INFO   *hostInfo;
    UCHAR  		send_ready_flag;
	USHORT		out_msg_len;
    USHORT		in_msg_len;
	UCHAR		msg_out_buf[MSG_BUF_LEN];
    UCHAR       msg_in_buf[MSG_BUF_LEN];
    time_t      curtime;

    void     send_com();
    void     read_msg();
    void     deal_msg();
    void     release_msg_buf(UCHAR msg_type);
};

#endif