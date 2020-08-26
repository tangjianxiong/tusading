#include <stdio.h>
#include <string.h>
#include "../hdr/encapsulation.h"
int Pack(const unsigned char *in, unsigned int inlen, char recv, char send, char msgtype, char *out)
{
    out[0] = recv;
    out[1] = send;
    out[2] = msgtype;
    memcpy(out + 3, in, inlen);
    return 1;
}
int UnPack(const unsigned char *in, unsigned int inlen, char *send, char *msgtype, char *out)
{
    *send = in[0];
    *msgtype = in[1];
    memcpy(out, in + 2, inlen - 2);
    return 1;
}
