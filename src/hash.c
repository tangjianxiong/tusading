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
	/* 本demo的hash值计算方式为md5，如需二次开发请在此重新实现该函数即可 */
	return MDFile(filename, output);
}
/* Calculate the string hash value */
int hash_str(char *str, int len, char *output)
{
	/* 本demo的hash值计算方式为md5，如需二次开发请在此重新实现该函数即可 */
	return MDString(str, output);
}