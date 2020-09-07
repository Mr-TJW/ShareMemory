#include "data_queue.h"

C_DataQueue::C_DataQueue(/* args */)
{

}

C_DataQueue::~C_DataQueue()
{

}

/*******************************************************************
* funcname:	data_queue_initial
* para:	    data_queue-数据队列
* function:	初始化数据队列
* return:	
********************************************************************/
void C_DataQueue::data_queue_initial(S_DATA_QUEUE *data_queue)
{
    (*data_queue).count = 0;
    (*data_queue).next  = 0;
    (*data_queue).start = 0;
    (*data_queue).count = 0;
    (*data_queue).size  = DATA_QUEUE_BUF_SIZE;
}

/*******************************************************************
* funcname:	put_data
* para:	    data_queue-数据队列，data-准备进入队列的数据
* function:	
* return:	
********************************************************************/
void C_DataQueue::put_data(S_DATA_QUEUE *data_queue,unsigned char data)
{   
    (*data_queue).buf[(*data_queue).next++] = data;

    if((*data_queue).next >= (*data_queue).size)
    {
        (*data_queue).next = 0;
    }

    (*data_queue).count++;
    if((*data_queue).count > (*data_queue).size)
    {
        (*data_queue).count = (*data_queue).size;
        (*data_queue).start++;
        if((*data_queue).start >= (*data_queue).size)
        {
            (*data_queue).start = 0;
        }
    }
}

/*******************************************************************
* funcname:	drop_data
* para:	    data_queue-数据队列
* function:	处理完的数据退出队列
* return:	
********************************************************************/
void C_DataQueue::drop_data(S_DATA_QUEUE *data_queue)
{
    if(((*data_queue).next == (*data_queue).start)&&((*data_queue).count ==0)) 
    {   
        (*data_queue).count = 0;
        return;
    }

    (*data_queue).start++;
    if((*data_queue).start >= (*data_queue).size)
    {
        (*data_queue).start = 0;
    }

    if((*data_queue).count > 0)
    {
        (*data_queue).count--;
    }
    
}

/*******************************************************************
* funcname:	mod
* para:     par1-参入比较的参数，par2-边界
* function:	如果par1越界，即超过par2将par1置0
* return:	
********************************************************************/
unsigned int C_DataQueue::mod(unsigned int par1,unsigned int par2)
{
	if ( par1 >= par2 )
	{
		return(par1 - par1 / par2 * par2);
	}
	else
	{
		return(par1);
	}
}