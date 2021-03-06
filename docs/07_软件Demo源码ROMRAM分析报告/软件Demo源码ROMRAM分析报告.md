---
title: 软件Demo
subtitle: ROMRAM分析报告
author: 汤健雄
changelog:
- ver: 0.6
  date: 2020.9.28
  author: 汤健雄 
  desc: |
		 修改代码围栏样式
- ver: 0.5
  date: 2020.9.28
  author: 汤健雄 
  desc: |
		 修改文档名称
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

# 测试工具
## /proc
/proc是一种伪文件系统（也即虚拟文件系统），存储的是当前内核运行状态的一系列特殊文件，用户可以通过这些文件查看有关系统硬件及当前正在运行进程的信息。
## objdump
objdump工具用来显示二进制文件的信息，就是以一种可阅读的格式让你更多地了解二进制文件可能带有的附加信息,可查看编译产物的相关信息。

# 测试内容
对系统各种运行状态下的RAMROM占用情况进行测试，主要方式通过查看进程的状态。  
测试方法：运行服务端查看相关进程的status。

## 动态ROM分析

### 服务端A
以下是服务端A在运行状态下的ROM占用情况：

```{#idcpp .cpp .numberLines startFrom="1" caption="Example"}
VmData:    25232 kB   //进程数据段大小
VmStk:       132 kB   //进程堆栈段大小
VmExe:        20 kB   //进程代码大小
VmLib:      3720 kB   //进程LIB库大小
VmPTE:        44 kB   //所占页表大小

```

### 客户端B
以下是客户端B在各个功能模式运行状态下的ROM占用情况：

```{#idcpp .cpp .numberLines startFrom="1" caption="Example"}
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

```{#idcpp .cpp .numberLines startFrom="1" caption="Example"}
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

## 动态RAM分析
### 服务端A
以下是服务端A在运行状态下的ROM占用情况：  
无数据传输

```{#idcpp .cpp .numberLines startFrom="1" caption="Example"}
VmPeak:   291472 kB   //进程内存占用峰值
VmSize:   225936 kB   //进程现在占用内存
VmHWM:      1328 kB   //系统分配物理内存峰值
VmRSS:      1328 kB   //进程当前占用物理内存
```

传输数据：

```{#idcpp .cpp .numberLines startFrom="1" caption="Example"}
VmPeak:   291472 kB   //进程内存占用峰值
VmSize:   225936 kB   //进程现在占用内存
VmHWM:     12148 kB   //系统分配物理内存峰值
VmRSS:      2464 kB   //进程当前占用物理内存
```
### 客户端B
以下是客户端B在运行状态下的ROM占用情况：

未连接
```{#idcpp .cpp .numberLines startFrom="1" caption="Example"}
VmPeak:     4740 kB   //进程内存占用峰值
VmSize:     4740 kB   //进程现在占用内存
VmHWM:      1268 kB   //系统分配物理内存峰值
VmRSS:      1268 kB   //进程当前占用物理内存

```

数据传输

```{#idcpp .cpp .numberLines startFrom="1" caption="Example"}
VmPeak:   291472 kB   //进程内存占用峰值
VmSize:   225936 kB   //进程现在占用内存
VmHWM:      1268 kB   //系统分配物理内存峰值
VmRSS:      1268 kB   //进程当前占用物理内存
```
### 客户端C
以下是服务端A在运行状态下的ROM占用情况：

未连接

```{#idcpp .cpp .numberLines startFrom="1" caption="Example"}
VmPeak:     4740 kB   //进程内存占用峰值
VmSize:     4740 kB   //进程现在占用内存
VmHWM:      1264 kB   //系统分配物理内存峰值
VmRSS:      1264 kB   //进程当前占用物理内存
```

数据传输：
```{#idcpp .cpp .numberLines startFrom="1" caption="Example"}
VmPeak:   225936 kB   //进程内存占用峰值
VmSize:   225936 kB   //进程现在占用内存
VmHWM:      1264 kB   //系统分配物理内存峰值
VmRSS:      1264 kB   //进程当前占用物理内存
```
## 静态分析
采用objdump工具对编译产物进行静态分析。
### 内核模块
| 文件名称    | 字段名称 | 含义           | 大小(b) |
| ----------- | -------- | -------------- | ------- |
|             | text     | 代码段         | 2638    |
| 内核模块K   | data     | 已初始化数据段 | 772     |
| kernel_k.ko | bss      | 未初始化数据   | 2856    |
|             | dec      | 总和           | 6266    |
Table: 内核模块静态ROMRAM分析

### 服务端A
| 文件名称 | 字段名称 | 含义           | 大小(b) |
| -------- | -------- | -------------- | ------- |
|          | text     | 代码段         | 16778   |
| 服务端A  | data     | 已初始化数据段 | 872     |
| server_a | bss      | 未初始化数据   | 1632    |
|          | dec      | 总和           | 19282   |
Table: 服务端A静态ROMRAM分析

### 客户端B
| 文件名称 | 字段名称 | 含义           | 大小(b) |
| -------- | -------- | -------------- | ------- |
|          | text     | 代码段         | 17761   |
| 客户端B  | data     | 已初始化数据段 | 872     |
| client_b | bss      | 未初始化数据   | 1040    |
|          | dec      | 总和           | 19673   |
Table: 客户端B静态ROMRAM分析

### 客户端C
| 文件名称 | 字段名称 | 含义           | 大小(b) |
| -------- | -------- | -------------- | ------- |
|          | text     | 代码段         | 17817   |
| 客户端C  | data     | 已初始化数据段 | 872     |
| client_c | bss      | 未初始化数据   | 1040    |
|          | dec      | 总和           | 19729   |
Table: 客户端C静态ROMRAM分析
# 总结
本文档对各个模块在运行过程中的ramrom占用情况进行了测试，暂时只用了/proc工具和objdump，还缺乏专业全面RAMROM的分析。