#include <stdio.h>
#include <string.h>
#include "../../hdr/netlink.h"
#include "../../hdr/protocol.h"
#include <mcheck.h>
int main(int argc, char *argv[])
{
    mtrace();
    int portid;
    sscanf(argv[1], "%d", &portid);
    printf("[pid]%d\n", portid);
    netlink_init(portid);
    muntrace();
    return 1;
}
