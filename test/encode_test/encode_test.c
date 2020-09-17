#include <stdio.h>
#include <string.h>
#include "../../hdr/codec.h"
#include "../../hdr/protocol.h"
int main(int argc, char *argv[])
{
    char in_str[MAX_MSG_SIZE];
    char out_str[MAX_ENCODE_SIZE];
    int num;
    int len = strlen(argv[1]);
    num = msg_encode(argv[1], len, out_str);
    if (num < 0)
    {
        printf("[errno]%d\n", num);
    }
    else
    {
        printf("[After encoding]%s\n", out_str);
    }
    return 0;
}
