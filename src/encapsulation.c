/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-22
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/src/encapsulation.c
 * Description  : encapsulate messages, adding information such as sender receiver and message type
 */
#include <stdio.h>
#include <string.h>
#include "../hdr/encapsulation.h"

int pack(const unsigned char *in, unsigned int inlen, char recv, char send, char msgtype, char *out)
{
    out[0] = recv;
    out[1] = send;
    out[2] = msgtype;
    memcpy(out + 3, in, inlen);
    return 1;
}

int unpack(const unsigned char *in, unsigned int inlen, char *send, char *msgtype, char *out)
{
    *send = in[0];
    *msgtype = in[1];
    memcpy(out, in + 2, inlen - 2);
    return 1;
}
