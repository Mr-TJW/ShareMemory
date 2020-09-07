#ifndef	_DATA_QUEUE_H
#define	_DATA_QUEUE_H

#define         DATA_QUEUE_BUF_SIZE     2048

typedef struct _S_DATA_QUEUE_
{
    unsigned short      next;
    unsigned short      start;
    unsigned short      count;
    unsigned short      size; 
    unsigned char       buf[DATA_QUEUE_BUF_SIZE];
}S_DATA_QUEUE;

class C_DataQueue
{
private:
    /* data */
public:
    C_DataQueue(/* args */);
    ~C_DataQueue();
public:
    void    data_queue_initial(S_DATA_QUEUE *data_queue);
    void    put_data(S_DATA_QUEUE *data_queue,unsigned char  data);
    void    drop_data(S_DATA_QUEUE *data_queue);
    unsigned int mod(unsigned int par1,unsigned int par2);
};

#endif