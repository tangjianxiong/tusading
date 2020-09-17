
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
/* 将hash值以16进制打印出来 */
void print_hexData(char *src, long len);

int hash_calculate(unsigned char *message, long len, unsigned char *output);
int hash_verify(unsigned char *hashstr1, unsigned char *hashstr2);

/* 计算文件hash值 */
int hash_file(const char *filename, char *output);

int hash_str(const char *str, int len, char *output);