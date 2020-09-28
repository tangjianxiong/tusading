#include "../hdr/codec.h"
#include "../hdr/base64.h"
/*
 * out is null-terminated msg_encode string.
 * return values is out length, exclusive terminating `\0'
 */
int msg_encode(const unsigned char *in, unsigned int inlen, char *out)
{
    return base64_encode(in, inlen, out);
}

/*
 * return values is out length
 */
int msg_decode(const char *in, unsigned int inlen, unsigned char *out)
{
    return base64_decode(in, inlen, out);
}