/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-28
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/src/codec.c
 * Description  : Codec module implementation, the user can be customized in this second time
 */
#include "../hdr/codec.h"
#include "../hdr/base64.h"

int msg_encode(const unsigned char *in, unsigned int inlen, char *out)
{
    return base64_encode(in, inlen, out);
}

int msg_decode(const char *in, unsigned int inlen, unsigned char *out)
{
    return base64_decode(in, inlen, out);
}