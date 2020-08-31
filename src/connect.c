#include "../hdr/connect.h"
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