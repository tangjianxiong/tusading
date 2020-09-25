#include "../hdr/connect.h"
#include "../hdr/protocol.h"
#define PASSWD_B "tusading"
#define PASSWD_C "tusading"

int passwd_vertify(char *passwd, char name)
{
    switch (name)
    {
    case NAME_B:
        return 1;
        break;
    case NAME_C:
        return 1;
        break;
    default:
        return -1;
        break;
    }
}