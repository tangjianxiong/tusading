#include <stdint.h>
#include <string.h>
//message为明文，len为明文长度，
//output为明文的MD5值（固定128位）
void print_hexData(char *src, long len);
int hash_calculate(unsigned char *message, long len, unsigned char *output);
int hash_verify(unsigned char *hashstr1, unsigned char *hashstr2);
int passwd_vertify(char *str);
