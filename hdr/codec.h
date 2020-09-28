#include <stdio.h>
#include <string.h>
/*
 * out is null-terminated msg_encode string.
 * return values is out length, exclusive terminating `\0'
 */
int msg_encode(const unsigned char *in, unsigned int inlen, char *out);

/*
 * return values is out length
 */
int msg_decode(const char *in, unsigned int inlen, unsigned char *out);
