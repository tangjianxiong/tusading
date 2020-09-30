/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-30
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/hdr/codec.h
 * Description  : codec header file
 */

#include <stdio.h>
#include <string.h>
/*
 * out is null-terminated msg_encode string.
 * return values is out length, exclusive terminating `\0'
 */
int msg_encode(const unsigned char *in, unsigned int inlen, char *out);

/*
 * return values is out length
 */
int msg_decode(const char *in, unsigned int inlen, unsigned char *out);
