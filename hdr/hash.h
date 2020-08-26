#include <stdint.h>
#include <string.h>
//message为明文，len为明文长度，
//output为明文的MD5值（固定128位）
void Print_HexData(char *src, long len);
int Hash_Calculate(unsigned char *message, long len, unsigned char *output);
int Hash_verify(unsigned char *hashstr1, unsigned char *hashstr2);
int Passwd_Vertify(char *str);
