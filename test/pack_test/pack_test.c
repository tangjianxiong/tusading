#include <stdio.h>
#include <string.h>
#include "../../hdr/encapsulation.h"
#include "../../hdr/protocol.h"
int main(int argc, char *argv[])
{

    char out_str[MAX_PACK_SIZE];
    char recv = argv[2][0];
    char send = argv[3][0];
    char msgtype = argv[4][0];
    int num;
    int len = strlen(argv[1]);
    num = pack(argv[1], len, recv, send, msgtype, out_str);
    if (num < 0)
    {
        printf("[errno]%d\n", num);
    }
    else
    {
        printf("[pack]%s\n", out_str);
    }
    return 0;
}
