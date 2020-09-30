/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-30
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/hdr/encapsulation.h
 * Description  : pack header file
 */

/* pack data with information such as sender, receiver, and data type */
int pack(const unsigned char *in, unsigned int inlen, char recv, char send, char msgtype, char *out);

/* unpack data for information such as sender, receiver, and data type */
int unpack(const unsigned char *in, unsigned int inlen, char *send, char *msgtype, char *out);