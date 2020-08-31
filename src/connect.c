#include "../hdr/connect.h"
#include "../hdr/protocol.h"
#define PASSWD_B "tusading"
#define PASSWD_C "tusading"
int get_id(char a)
{
    int res;
    switch (a)
    {

    case 'a':
        res = PID_A;
        break;
    case 'b':
        res = PID_B;
        break;

    case 'c':
        res = PID_C;
        break;

    default:
        res = -1;
        break;
    }
    return res;
}
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