#include "../hdr/netlink.h"
#include "../hdr/protocol.h"
extern int errno;
int netlink_create_socket(void)
{
    //create a socket
    return socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
}

int netlink_bind(int sock_fd, int p_id)
{
    struct sockaddr_nl addr;

    memset(&addr, 0, sizeof(struct sockaddr_nl));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = p_id;
    addr.nl_groups = 0;

    return bind(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_nl));
}

int netlink_send_message(int sock_fd, const unsigned char *message, int len, unsigned int send_pid, unsigned int recv_pid, unsigned int group)
{
    struct nlmsghdr *nlh = NULL;
    struct sockaddr_nl dest_addr;
    struct iovec iov;
    struct msghdr msg;

    if (!message)
    {
        return -1;
    }

    //create message
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(len));
    if (!nlh)
    {
        perror("malloc");
        return -2;
    }
    nlh->nlmsg_len = NLMSG_SPACE(len);
    nlh->nlmsg_pid = send_pid;
    nlh->nlmsg_flags = 0;
    memcpy(NLMSG_DATA(nlh), message, len);

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    memset(&dest_addr, 0, sizeof(struct sockaddr_nl));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = recv_pid;
    dest_addr.nl_groups = group;

    memset(&msg, 0, sizeof(struct msghdr));
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(struct sockaddr_nl);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    //send message
    if (sendmsg(sock_fd, &msg, 0) < 0)
    {
        printf("send error!\n");
        free(nlh);
        return -3;
    }

    free(nlh);
    return 0;
}

int netlink_recv_message(int sock_fd, unsigned char *message, int *len)
{
    struct nlmsghdr *nlh = NULL;
    struct sockaddr_nl source_addr;
    struct iovec iov;
    struct msghdr msg;

    if (!message || !len)
    {
        return -1;
    }

    //create message
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    if (!nlh)
    {
        perror("malloc");
        return -2;
    }
    iov.iov_base = (void *)nlh;
    iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD);
    memset(&source_addr, 0, sizeof(struct sockaddr_nl));
    memset(&msg, 0, sizeof(struct msghdr));
    msg.msg_name = (void *)&source_addr;
    msg.msg_namelen = sizeof(struct sockaddr_nl);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if (recvmsg(sock_fd, &msg, 0) < 0)
    {
        printf("[recvmsg error!]\n");
        printf("[errno]%d\n", errno);
        free(nlh);
        return -3;
    }
    *len = strlen(NLMSG_DATA(nlh));
    memcpy(message, (unsigned char *)NLMSG_DATA(nlh), *len);

    free(nlh);
    return 0;
}
int netlink_init(int id)
{

    int sock_fd = netlink_create_socket();
    if (sock_fd == -1)
    {
        printf("create socket error!\n");
        printf("[errno]%d\n", errno);
        exit(EXIT_FAILURE);
    }
    //bind the socket to the local address.
    if (netlink_bind(sock_fd, id) < 0)
    {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    //Create socket and initialize successfully
    printf("Initialization succeeded.");
    return sock_fd;
}
