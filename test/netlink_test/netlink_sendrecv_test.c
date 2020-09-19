#include <stdio.h>
#include <string.h>
#include "../../hdr/netlink.h"
#include "../../hdr/protocol.h"
int main(int argc, char *argv[])
{

    int portid = 101;
    int sock_fd;
    char buf[MAX_PAYLOAD];
    int len;
    if (argc < 2)
    {
        printf("enter message!\n");
        exit(EXIT_FAILURE);
    }
    sock_fd = netlink_init(portid);

    netlink_send_message(sock_fd, argv[1], strlen(argv[1]) + 1, portid, 0, 0);
    if (netlink_recv_message(sock_fd, buf, &len) == 0)
    {
        printf("recv:%s len:%d\n", buf, len);
    }

    return 1;
}
