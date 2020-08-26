#include <stdio.h>
#include <string.h>
#include "../../hdr/encapsulation.h"
#define MAX_SIZE 256
int main()
{
    // char buf[MAX_SIZE]="tusading";
    // char send='a';
    // char recv='b';
    // char msgtype='m';
    // char out[MAX_SIZE]="\0";
    // int len=0;
    // len=strlen(buf);
    // printf("the original msg:%s\n",buf);
    // printf("the packed msg:");
    // Pack(buf,len,recv,send,msgtype,out);
    // printf("%s\n",out);

    char buf[MAX_SIZE] = "amtusading";
    char send;
    char msgtype;
    char out[MAX_SIZE] = "\0";
    int len = 0;
    len = strlen(buf);
    printf("the packed msg:%s\n", buf);
    UnPack(buf, len, &send, &msgtype, out);
    printf("the msg sender is:%c\n", send);
    printf("the msg type is:%c\n", msgtype);
    printf("the  msg is:");
    printf("%s\n", out);
}
