---
title: 软件Demo
subtitle: 源码检查报告
author: 汤健雄
changelog:
- ver: 0.5
  date: 2020.9.28
  author: 汤健雄 
  desc: |
		 修改文档名称
- ver: 0.1
  date: 2020.9.19
  author: 汤健雄 
  desc: |
		 建立文档   



---


# 前言

## 文档简介
此文档为p166软件项目的静态代码审核报告。

## 目标读者
P166项目软件测试人员和项目验收人员。

## 文档目的
使用相关的静态代码检查工具对软件源代码进行检查，检测代码的一些隐藏错误和缺陷，提高软件的可靠性并节省软件开发和测试的成本。


# 工具说明
静态代码检测工具种类很多，本文档用到的主要有以下工具。
## CPPcheck

Cppcheck是一种C/C++代码缺陷静态检查工具，不同于C/C++编译器及其它分析工具，Cppcheck只检查编译器检查不出来的bug，不检查语法错误，作为编译器的一种补充检查，cppcheck对产品的源代码执行严格的逻辑检查。 执行的检查包括：   

+ 自动变量检查 
+ 数组的边界检查 
+ class类检查 
+ 过期的函数，废弃函数调用检查 
+ 异常内存使用，释放检查 
+ 内存泄漏检查，主要是通过内存引用指针 
+ 操作系统资源释放检查，中断，文件描述符等 
+ 异常STL 函数使用检查 
+ 代码格式错误，以及性能因素检查

## gcov
gcov是测试代码覆盖率的工具，可通过命令行方式进行操作，可以帮助开发者优化代码。主要执行的检查包括

+ 每一行代码的执行频率
+ 实际上哪些代码确实被执行了  
+ 每一段代码的执行时间

## checkpatch.pl
checkpatch是公司在仓库集成的patch检查工具进行格式检查，每次发生变更之后需要达到提交的规范才能进行代码的合并。
# check静态代码检查
静态代码检查环节采用cppcheck工具分别对源码的公共模块、内核模块和应用模块进行了相应的检查。
主要发现的问题如下：
## 公共模块

```
[src/codec.c:129]: (style) The scope of the variable 'c' can be reduced.
[src/codec.c:184]: (style) The scope of the variable 'c' can be reduced.
[src/codec.c:133]: (style) Checking if unsigned variable 'inlen' is less than zero.
[src/codec.c:187]: (style) Checking if unsigned variable 'inlen' is less than zero.
[src/hash.c:190]: (style) Variable 'str' is assigned a value that is never used.
[src/connect.c:6]: (style) The function 'get_id' is never used.
[src/hash.c:305]: (style) The function 'hash_str' is never used.
```
## 服务端A
```
[src/server_a.c:253]: (style) The scope of the variable 'find' can be reduced.
[src/server_a.c:260]: (style) The scope of the variable 'res' can be reduced.
[src/server_a.c:16]: (style) Variable 'thrd_num' is assigned a value that is never used.
[src/server_a.c:32]: (style) Variable 'thrd_num' is assigned a value that is never used.
[src/server_a.c:111]: (style) Variable 'ret' is assigned a value that is never used.
[src/server_a.c:194]: (style) Variable 'num1' is assigned a value that is never used.
[src/server_a.c:205]: (style) Variable 'num' is assigned a value that is never used.
[src/server_a.c:252]: (style) Unused variable: len
[src/server_a.c:257]: (style) Unused variable: buf
[src/server_a.c:258]: (style) Unused variable: buf_hash
[src/server_a.c:262]: (style) Unused variable: msgtype
[src/server_a.c:14]: (style) The function 'thread_recv_message1' is never used.
```

## 客户端B

```
[src/client_b.c:11]: (style) Variable 'thrd_num' is assigned a value that is never used.
[src/client_b.c:121]: (error) fflush() called on input stream 'stdin' results in undefined behaviour.
[src/client_b.c:119]: (portability) scanf without field width limits can crash with huge input data on some versions of libc.
```
## 客户端C
```
[src/client_c.c:11]: (style) Variable 'thrd_num' is assigned a value that is never used.
[src/client_c.c:116]: (error) fflush() called on input stream 'stdin' results in undefined behaviour.
[src/client_c.c:114]: (portability) scanf without field width limits can crash with huge input data on some versions of libc.
```
## 检查结果
各个源码文件均存在或多或少的style类型缺陷，主要有以下几个方面

+ 变量声明未使用
+ 变量值与类型不符
+ 变量范围过大  

客户端bc源文件中出现了error类型的缺陷：主要是因为fflush函数的使用方法不当。  
客户端bc源文件中出现了portability类型的缺陷：主要原因是scanf输入没有字段宽度限制。
# 缺陷统计
| 缺陷类型    | 所属文件   | 数量 | 解决数量 | 遗留缺陷 |
| ----------- | ---------- | ---- | -------- | -------- |
| style       | server_a.c | 11   | 11       | 0        |
|             | client_b.c | 1    | 1        | 0        |
|             | client_c.c | 1    | 1        | 0        |
|             | codec.c    | 4    | 4        | 0        |
|             | hash.c     | 2    | 2        | 0        |
|             | connect.c  | 1    | 1        | 0        |
| error       | client_b.c | 1    | 1        | 0        |
|             | client_c.c | 1    | 1        | 0        |
| portability | client_b.c | 1    | 1        | 0        |
|             | client_c.c | 1    | 1        | 0        |
Table: cppcheck静态代码检查缺陷统计
# 总结
本文档对软件源码的静态审核结果进行了记录，主要是采用cppchek工具对源码进行了自动变量检查、数组的边界检查、class类检查、废弃函数调用检查、异常内存使用，释放检查内存泄漏检查等等，根据检测结果对相应的缺陷进行了修改。产生缺陷的主要原因有几点：

+ 函数定义之后未使用
+ 用户输入未限定边界
+ 变量范围过大
+ 公共接口函数使用不当  

由于检查工具的限制，可能还存在一些隐藏的缺陷未被检测出来，后续可使用其他工具对代码的其他方面进行静态检查，并对此文档进行迭代。