#ifndef	_CONFIG_H
#define	_CONFIG_H

#include "data_type.h"
#include "shareMemory.h"
#include "data_queue.h"
#include "public.h"

#define VENDOR_CODE_LEN							64			/* 厂商代码最大长度 */
#define SOFTWARE_VERSION_LEN					64			/* 软件版本最大长度 */
#define ASSET_QUANTITY_MAX						256			/* 最大资产数量 */
#define ASSET_NUMBER_LEN						22			/* 资产编号最大长度 */
#define ID_TOKEN_LEN							64			/* 身份认证 token 长度 */


/* 版本信息 91000200*/
typedef struct _VERSION_INFO_
{	
	UCHAR				vendor_code_len;				/* 厂商代码 */
	char				vendor_code[VENDOR_CODE_LEN];	
	UCHAR				software_version_len;			/* 软件版本 */
	char				software_version[SOFTWARE_VERSION_LEN];
	DATE_TIME_S			version_date;					/* 版本日期 */
}VERSION_INFO;

typedef struct _HOST_INFO_
{	
	UCHAR				host_addr[6];			/* 主机通信地址 */
	UCHAR				net_link_state;			/* 网络连接状态 */
    USHORT  			heart_interval;     	/* 心跳间隔 单位s*/
	USHORT				last_heart_send_time;	/* 心跳超时时间 */
	VERSION_INFO		version_info;			/* 版本信息 */
	USHORT				master_port;			/* 主站端口号 */
	UCHAR				master_ip[4];			/* 主站ip */
	S_DATA_QUEUE		rec_from_com_queue,send_to_com_queue;	/* 收发队列 */
	paraShmOper	        *paraShm;               /* 共享内存 */
	paraShmOper	        *cmdShm;               /* 共享内存 */
}HOST_INFO;


#endif