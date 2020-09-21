# 项目介绍
## 项目背景
M 公司是一家采用全志 SOC 集成方案的品牌大客户，7 月份跟全志合作立项, 远程联合开发一个代号 P166 的重要项目，该项目基于全志 A100 平台，为了加快 项目并行进度，P166 项目客户端项目经理 L，向全志 Aserver 平台提交了一 项软件开发需求，要求全志方提供一套易用、稳定、可复用的软件 Demo， 降低客户端前期开发工作量，加快二次开发整体进度。
## 项目功能介绍
软件Demo包含后台服务应用A、客户端应用 B、客户端应用C和内核模块K四个独立组件。K作为A和B、A和C之间的通信中转站，B和C之间不能通信。软件Demo主要有以下功能。  
Demo功能1：A和B发生一次通信，A将数据包编码后发送给K，K受到数据包转发给B，B对数据包完成逆向解码还原，并将原始数据的HASH值字符串通过K返还给A。A受到HASH值字符串进行正确性校验，校验成功完成通信，校验失败后Log日志抛出异常码ERN110。  
Demo功能2：同理A和C发生通信过程如上，校验失败后Log日志抛出异常码ERN120。  
## 限制条件
- 规格 
  软件开发：保证解耦设计，可被二次定制，具有一定的鲁棒性   
  代码规范：代码风格符合SWC和SW4的代码规范要求，使用git进行统一的管理  
  测试：各个模块支持多种方便、单独的调试手段，支持临时数据的调试，支持命令调试  
  文档：符合软件设计文档规范，并需在内部评审通过  
- 交付说明 
  代码：提交至git仓库——SWC-Bootcamp  
  文档：上传至edoc，具体文档包括：虚拟项目任务计划书，软件概要设计文档，各个组件 的测试列表、测试报告，各个模块代码的静态代码检查报告，组件之间的联调报告，代码的 ROM/RAM分析报告，开发、调试过程的记录文档，总结文档。 
# 源码结构

```
├── docs                        项目相关文档目录
├── hdr                         头文件目录
│   ├── codec.h                 编解码头文件
│   ├── connect.h               连接控制模块头文件    
│   ├── encapsulation.h         封装模块头文件
│   ├── hash.h                  hash模块头文件
│   ├── netlink.h               netlink模块头文件
│   └── protocol.h              协议相关头文件
├── kernelspace                 内核源码目录
│   ├── kernel_k.c              内核模块k源码
│   ├── Kconfig                 编译相关配置文件
│   └── Makefile                编译相关配置文件
├── lib                         依赖的外部库
│   ├── include
│   └── lib
├── out                         编译产物目录
│   ├── client_b
│   ├── client_c
│   ├── kernel_k.ko
│   └── server_a
├── README.md                   
├── script                      脚本文件目录
│   └── build.sh
├── src                         源文件目录
│   ├── client_b.c              客户端b源码
│   ├── client_c.c              客户端c源码
│   ├── codec.c                 编解码模块源码
│   ├── connect.c               连接控制模块源码
│   ├── encapsulation.c         封装模块源码
│   ├── hash.c                  hash模块源码
│   ├── Makefile                编译Makefile
│   ├── netlink.c               netlink模块源码    
│   └── server_a.c              服务端a源码
├── tools                       相关工具
└── test                        测试文件目录
    ├── codec_test
    ├── file_test
    ├── hash_test
    ├── netlink_file
    ├── pack_test
    └── socket_file
```
