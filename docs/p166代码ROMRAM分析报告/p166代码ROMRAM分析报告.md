---
title: SWC 虚拟项目
subtitle: ROMRAM分析报告
author: 汤健雄
changelog:
- ver: 0.1
  date: 2020.9.19
  author: 汤健雄 
  desc: |
		1.建立文档   



---


# 前言

## 文档简介
此文档为p166软件项目的ROMRAM分析报告。

## 目标读者
P166项目软件测试人员和项目验收人员。

## 文档目的
对软件运行过程中的ramrom占用情况进行测试和分析。


# 测试内容
对系统各种运行状态下的RAMROM占用情况进行测试，主要方式通过查看进程的状态。  
测试方法：运行服务端查看相关进程的status。

## ROM分析

### 服务端A
以下是服务端A在运行状态下的ROM占用情况：

```
VmData:    25232 kB   //进程数据段大小
VmStk:       132 kB   //进程堆栈段大小
VmExe:        20 kB   //进程代码大小
VmLib:      3720 kB   //进程LIB库大小
VmPTE:        44 kB   //所占页表大小

```

### 客户端B
以下是客户端B在各个功能模式运行状态下的ROM占用情况：

```
未连接状态：
VmData:      260 kB   //进程数据段大小
VmStk:       132 kB   //进程堆栈段大小
VmExe:        20 kB   //进程代码大小
VmLib:      3720 kB   //进程LIB库大小
VmPTE:        28 kB   //所占页表大小

发送消息模式：
VmData:    25232 kB   //进程数据段大小
VmStk:       132 kB   //进程堆栈段大小
VmExe:        20 kB   //进程代码大小
VmLib:      3720 kB   //进程LIB库大小
VmPTE:        48 kB   //所占页表大小

传输文件模式：
VmData:    11280 kB   //进程数据段大小
VmStk:       132 kB   //进程堆栈段大小
VmExe:        20 kB   //进程代码大小
VmLib:      3720 kB   //进程LIB库大小
VmPTE:        44 kB   //所占页表大小
```

### 客户端C 
以下是客户端B在各个功能模式状态下的ROM占用情况：

```
未连接状态：
VmData:      260 kB   //进程数据段大小
VmStk:       132 kB   //进程堆栈段大小
VmExe:        20 kB   //进程代码大小
VmLib:      3720 kB   //进程LIB库大小
VmPTE:        24 kB   //所占页表大小

发送消息模式：
VmData:    25232 kB   //进程数据段大小
VmStk:       132 kB   //进程堆栈段大小
VmExe:        20 kB   //进程代码大小
VmLib:      3720 kB   //进程LIB库大小
VmPTE:        52 kB   //所占页表大小

传输文件模式：
VmData:    11280 kB   //进程数据段大小
VmStk:       132 kB   //进程堆栈段大小
VmExe:        20 kB   //进程代码大小
VmLib:      3720 kB   //进程LIB库大小
VmPTE:        44 kB   //所占页表大小
```

## RAM分析
### 服务端A
以下是服务端A在运行状态下的ROM占用情况：  
无数据传输

```
VmPeak:   291472 kB   //进程内存占用峰值
VmSize:   225936 kB   //进程现在占用内存
VmHWM:      1328 kB   //系统分配物理内存峰值
VmRSS:      1328 kB   //进程当前占用物理内存
```

传输数据：

```
VmPeak:   291472 kB   //进程内存占用峰值
VmSize:   225936 kB   //进程现在占用内存
VmHWM:     12148 kB   //系统分配物理内存峰值
VmRSS:      2464 kB   //进程当前占用物理内存
```
### 客户端B
以下是客户端B在运行状态下的ROM占用情况：

未连接
```
VmPeak:     4740 kB   //进程内存占用峰值
VmSize:     4740 kB   //进程现在占用内存
VmHWM:      1268 kB   //系统分配物理内存峰值
VmRSS:      1268 kB   //进程当前占用物理内存

```

数据传输

```
VmPeak:   291472 kB   //进程内存占用峰值
VmSize:   225936 kB   //进程现在占用内存
VmHWM:      1268 kB   //系统分配物理内存峰值
VmRSS:      1268 kB   //进程当前占用物理内存
```
### 客户端C
以下是服务端A在运行状态下的ROM占用情况：

未连接

```
VmPeak:     4740 kB   //进程内存占用峰值
VmSize:     4740 kB   //进程现在占用内存
VmHWM:      1264 kB   //系统分配物理内存峰值
VmRSS:      1264 kB   //进程当前占用物理内存
```

数据传输：
```
VmPeak:   225936 kB   //进程内存占用峰值
VmSize:   225936 kB   //进程现在占用内存
VmHWM:      1264 kB   //系统分配物理内存峰值
VmRSS:      1264 kB   //进程当前占用物理内存
```

# 总结
本文档对各个模块在运行过程中的ramrom占用情况进行了测试，暂时只用了/proc工具查看了相关进程的ram rom占用信息，还缺乏专业全面RAMROM的分析。