#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../hdr/codec.h"

int main()
{
    unsigned char in_str[] = "tusading";
    unsigned char out_str[] = "\0";
    unsigned char out_str1[] = "\0";
    int len = strlen(in_str);
    printf("Before encoding:\n");
    puts(in_str);
    Encode(in_str, len, out_str);
    printf("\nAfter encoding:\n");
    puts(out_str);
    len = strlen(out_str);
    printf("After decoding:\n");
    Decode(out_str, len, out_str1);
    puts(out_str1);
    return 0;
}
