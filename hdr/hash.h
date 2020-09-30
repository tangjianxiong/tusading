/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-30
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/hdr/hash.h
 * Description  : md5 hash header file
 */

#include "md5.h"

/* Print the hash value in hexadecimal */
void print_hexData(char *src, long len);
/* Calculate the string hash value */
int hash_calculate(unsigned char *message, long len, unsigned char *output);
/* verify hash value */
int hash_verify(unsigned char *hashstr1, unsigned char *hashstr2);
/* Calculate the file hash value */
int hash_file(char *filename, char *output);
/* Calculate the string hash value */
int hash_str(char *str, int len, char *output);