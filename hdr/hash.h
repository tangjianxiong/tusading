
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
//message为明文，len为明文长度，
//output为明文的MD5值（固定128位）
void print_hexData(char *src, long len);
int hash_calculate(unsigned char *message, long len, unsigned char *output);
int hash_verify(unsigned char *hashstr1, unsigned char *hashstr2);
int hash_file(const char *filename, char *output);
int hash_str(const char *str, int len, char *output);