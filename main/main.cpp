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

#include "main.h"

C_TcpClient *com;   	/* 声明tcp客户端类 */
C_TEST      *test;
HOST_INFO   host_info;
paraShmOper shmOper(PARA_SHM_KEY,sizeof(PARA_SHM_DATA));
paraShmOper shmCmd(CMD_SHM_KEY,sizeof(PGS_CTL_CMD));

/*******************************************************************
* funcname:	host_info_config
* para:	
* function:	主用信息配置
* return:	
********************************************************************/
void host_info_config(HOST_INFO *host_info)
{   
    memset(host_info,0,sizeof(HOST_INFO));

    host_info->heart_interval = 10;                     /* 心跳间隔60s */
    host_info->last_heart_send_time = 5;                /* 心跳超时时间 5s */
    host_info->net_link_state = !LINK_OK;               /* 网络连接状态，这里为了测试，暂且置为连接状态 */

    host_info->host_addr[0] = 0x11;                     /* 主机通信地址 */
    host_info->host_addr[1] = 0x11;
    host_info->host_addr[2] = 0x11;
    host_info->host_addr[3] = 0x11;
    host_info->host_addr[4] = 0x11;
    host_info->host_addr[5] = 0x22;

    host_info->version_info.version_date.year  = 2020;  /* 软件版本 */
    host_info->version_info.version_date.mon   = 7;
    host_info->version_info.version_date.day   = 22;
    host_info->version_info.vendor_code_len = strlen("VVVVVVVV1234");
    strncpy(host_info->version_info.vendor_code,"VVVVVVVV1234",strlen("VVVVVVVV1234"));
    host_info->version_info.software_version_len = strlen("V1.4.5");
    strncpy(host_info->version_info.software_version,"V1.4.5",strlen("V1.4.5"));

    host_info->master_ip[0] = 192;
    host_info->master_ip[1] = 168;
    host_info->master_ip[2] = 20;
    host_info->master_ip[3] = 122;
    host_info->master_port  = 9002;
    
    host_info->paraShm = &shmOper;
    host_info->cmdShm = &shmCmd;
    /* 需要其他参数，自行添加 */
} 

/*******************************************************************
* funcname:	main
* para:	
* function:	主函数
* return:	
********************************************************************/
int main(void)
{   
    time_t      curtime = time(NULL);
    time_t      last_send_time = time(NULL);
    
    if(check_process_running() == true)
    {
        printf("process already running, quit!!!\r\n");
        exit(1);
    }

    //shmOper = new paraShmOper(PARA_SHM_KEY,sizeof(PARA_SHM_DATA));
    //shmCmd  = new paraShmOper(CMD_SHM_KEY,sizeof(PGS_CTL_CMD));

    host_info_config(&host_info);

    com     = new C_TcpClient(&host_info);   
    test    = new C_TEST(&host_info);
    

    /* 仅作示例 demo 示例展示功能，实际处理请按业务需求 */
    while(1)
	{  
        com->deal(); 
        test->deal();
        usleep(1000);
    } 

    delete test;
    delete com;
}