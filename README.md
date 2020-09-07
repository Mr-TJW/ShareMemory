# 聚合器 Dlt698 协议实现

[TOC]

## 项目：有序充电聚合方案及开发

## Dlt698 消息帧处理

* Dlt698 消息帧的识别与校验
* Dlt698 消息帧的解包
* Dlt698 消息帧的组包发送

## 业务内容

* 登录请求
* 心跳请求
* 读取资产信息响应
* 读取版本信息响应
* 充电执行事件的解包
* 充电计划上报事件、充电计划申请事件、充电计划执行事件、订单结算事件、充电模组在线状态变化事件、充电模组工作状态上送。
* 日志打印

# 消息处理
* 采用数据队列处理方式
* 接收到一帧数据里面包含多个 dlt698 报文，依次处理
* 一组 dlt698 报文被分成多个数据帧，等接收到完整 dlt698 报文后再进行处理

## 通信方式
* TCP 客户端模式
* 收发数据均采用数据队列方式，本次接收的数据不会覆盖上一次还未处理的数据

## 版本
* V1.0.0
  最初版本，主要处理业务逻辑，无实际收发消息功能
* V1.1.0
  添加了数据队列、tcp 客户端通信方式，将基本功能（设置及查询系统时间、防止多次启动、日志处理）等封装成库。
