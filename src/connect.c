/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-28
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/src/connect.c
 * Description  : server and client establishes the connection
 */
#include "../hdr/connect.h"
#include "../hdr/protocol.h"
#define PASSWD_B "tusading"
#define PASSWD_C "tusading"

int passwd_vertify(char *passwd, char name)
{
    switch (name)
    {
    case NAME_B:
        return 1;
        break;
    case NAME_C:
        return 1;
        break;
    default:
        return -1;
        break;
    }
}