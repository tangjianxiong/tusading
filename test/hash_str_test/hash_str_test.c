#include <stdio.h>
#include <string.h>
#include "../../hdr/hash.h"
#include "../../hdr/protocol.h"
int main(int argc, char *argv[])
{

    char out_str[32];
    int num;
    int len = strlen(argv[1]);
    num = hash_str(argv[1], len, out_str);
    if (num < 0)
    {
        printf("[errno]%d\n", num);
    }
    else
    {
        printf("[hash]%s\n", out_str);
    }
    return 0;
}
