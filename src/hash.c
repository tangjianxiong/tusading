/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-30
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/src/hash.c
 * Description  : calculates the hash values for files and strings
 */
#include "../hdr/hash.h"

int hash_verify(unsigned char *hashstr1, unsigned char *hashstr2)
{
	if (strcmp(hashstr1, hashstr2) == 0)
	{
		printf("Check success!\n");
		return 1;
	}
	else
	{
		printf("Check failure!\n");
		return -1;
	}
}

void print_hexData(char *src, long len)
{
	int i = 0;
	for (; i < len; i++)
	{
		printf("%X", (unsigned char)src[i]);
	}
	printf("\n");
}

int hash_file(char *filename, char *output)
{
	return MDFile(filename, output);
}
/* Calculate the string hash value */
int hash_str(char *str, int len, char *output)
{
	return MDString(str, output);
}