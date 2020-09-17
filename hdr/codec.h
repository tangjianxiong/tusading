#include <stdio.h>
#include <string.h>
#ifndef BASE64_H
#define BASE64_H

#define BASE64_msg_encode_OUT_SIZE(s) ((unsigned int)((((s) + 2) / 3) * 4 + 1))
#define BASE64_msg_decode_OUT_SIZE(s) ((unsigned int)(((s) / 4) * 3))

/*
 * out is null-terminated msg_encode string.
 * return values is out length, exclusive terminating `\0'
 */
int msg_encode(const unsigned char *in, unsigned int inlen, char *out);

/*
 * return values is out length
 */
int msg_decode(const char *in, unsigned int inlen, unsigned char *out);

#endif /* BASE64_H */
