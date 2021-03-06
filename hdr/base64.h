/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-30
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/hdr/base64.h
 * Description  : base64 codec header file
 */

#include <stdio.h>
#include <string.h>
#ifndef BASE64_H
#define BASE64_H

#define BASE64_msg_encode_OUT_SIZE(s) ((unsigned int)((((s) + 2) / 3) * 4 + 1))
#define BASE64_msg_decode_OUT_SIZE(s) ((unsigned int)(((s) / 4) * 3))

/*
 * out is null-terminated base64_encode string.
 * return values is out length, exclusive terminating `\0'
 */
int base64_encode(const unsigned char *in, unsigned int inlen, char *out);

/*
 * return values is out length
 */
int base64_decode(const char *in, unsigned int inlen, unsigned char *out);

#endif
