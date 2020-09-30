/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-30
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/hdr/communication.h
 * Description  : communication header file
 */

#include "netlink.h"

/* send message */
int communication_send_message(int sock_fd, const unsigned char *message, int len, unsigned int send_pid, unsigned int recv_pid, unsigned int group);
/* recv message*/
int communication_recv_message(int sock_fd, unsigned char *message, int *len);
/* communication initialization*/
int communication_init(int id);