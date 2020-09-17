#include <stdio.h>
#include <string.h>
#include "../../hdr/codec.h"
#include "../../hdr/protocol.h"
int main(int argc, char *argv[])
{
    char out_str[MAX_MSG_SIZE];
    int num;
    int len = strlen(argv[1]);
    num = msg_decode(argv[1], len, out_str);
    if (num < 0)
    {
        printf("[errno]%d\n", num);
    }
    else
    {
        printf("[After decoding]%s\n", out_str);
    }
    return 0;
}