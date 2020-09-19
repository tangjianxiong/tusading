#include <stdio.h>
#include <string.h>
#include "../../hdr/encapsulation.h"
#include "../../hdr/protocol.h"
int main(int argc, char *argv[])
{

    char out_str[MAX_ENCODE_SIZE];
    char send;
    char msgtype;
    int num;
    int len = strlen(argv[1]);
    num = unpack(argv[1], len, &send, &msgtype, out_str);
    if (num < 0)
    {
        printf("[errno]%d\n", num);
    }
    else
    {
        printf("[unpack]%s\n", out_str);
        printf("[send]%c\n", send);
        printf("[type]%c\n", msgtype);
    }
    return 0;
}
