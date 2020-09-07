# 共享内存同步机制的实现

> 简介 主进程作为 tcp 客户端，将接收到的消息存储于共享内存中并置位相应标志位，测试进程可以判断当前有无未处理的共享内存信息。

## 共享库 libparaShm.so
将共享内存与信号量搭配起来，实现共享内存的同步进制。

paraShmOper 类中定义了两种写入和读取共享内存信息的方法。

* getShmPara 与 putShmPara
```c
int getShmPara(char *paraData,USHORT paraId);
int putShmPara(char *paraData,USHORT paraId);
```
这种是读取和写入特定数据结构的操作共享内存的方法。

* readCmd 和 writeCmd
```c
int readCmd(UCHAR userNo,UCHAR *cmdBuf);
int writeCmd(UCHAR *cmdBuf);
```
这种是读取和写入消息串的操作共享内存的方法。