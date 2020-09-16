#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>
#include <pthread.h>
#include "hdr/md5sum.h"
#define FILE_NAME_MAX_SIZE (512)
#define NETLINK_TEST (25)
#define MAX_PAYLOAD (1024)
#define BUFFER_SIZE (1024)
#define TEST_PID (101)
extern int errno;
int netlink_create_socket(void)
{
    //create a socket
    return socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
}

int netlink_bind(int sock_fd)
{
    struct sockaddr_nl addr;

    memset(&addr, 0, sizeof(struct sockaddr_nl));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = TEST_PID;
    addr.nl_groups = 0;

    return bind(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_nl));
}

int netlink_send_message(int sock_fd, const unsigned char *message, int len,
                         unsigned int pid, unsigned int group)
{
    struct nlmsghdr *nlh = NULL;
    struct sockaddr_nl src_addr;
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
    nlh->nlmsg_pid = TEST_PID;
    nlh->nlmsg_flags = 0;
    memcpy(NLMSG_DATA(nlh), message, len);

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    memset(&src_addr, 0, sizeof(struct sockaddr_nl));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = pid;
    src_addr.nl_groups = group;

    memset(&msg, 0, sizeof(struct msghdr));
    msg.msg_name = (void *)&src_addr;
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

int netlink_recv_message(int sock_fd, char *message, int *len)
{
    struct nlmsghdr *nlh = NULL;
    struct sockaddr_nl source_addr;
    struct iovec iov;
    struct msghdr msg;
    extern int errno;
    int err;

    if (!message || !len)
    {
        printf("the message is NULL\n");
        return -1;
    }

    //create message
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    if (!nlh)
    {
        perror("malloc error!\n");
        return -2;
    }
    iov.iov_base = (void *)nlh;
    iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD);
    memset(&source_addr, 0, sizeof(struct sockaddr_nl));
    source_addr.nl_family = AF_NETLINK;
    source_addr.nl_pid = 0;
    source_addr.nl_groups = 0;
    memset(&msg, 0, sizeof(struct msghdr));
    msg.msg_name = (void *)&source_addr;
    msg.msg_namelen = sizeof(struct sockaddr_nl);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = 0;
    msg.msg_controllen = 0;
    msg.msg_flags = 0;
    err = recvmsg(sock_fd, &msg, 0);
    if (err < 0)
    {
        printf("recvmsg error![err1]%d\n", err);
        printf("[errno]%d\n", errno);
        free(nlh);
        return -3;
    }
    *len = nlh->nlmsg_len - NLMSG_SPACE(0);
    memcpy(message, NLMSG_DATA(nlh), *len);
    free(nlh);
    return 0;
}
//�̺߳���������Ϣ
void *thread_recv_message(void *arg)
{

    int len;
    int sock_fd = 3;
    unsigned char buf[2048];
    while (1)
    {
        if (netlink_recv_message(sock_fd, buf, &len) == 0)
            printf("recv:%s len:%d\n", buf, strlen(buf));
        else
        {
            break;
        }
    }
}
int main()
{
    int sock_fd;
    int len;
    unsigned char sendbuf[1024];
    unsigned char buf[2048];
    char hashstr[64] = {0};
    pthread_t tid;
    char *find;
    //�����׽���
    sock_fd = netlink_create_socket();
    if (sock_fd == -1)
    {
        printf("create socket error!\n");
        return -1;
    }
    //bind(),bind the socket to the local address.
    if (netlink_bind(sock_fd) < 0)
    {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    //Create socket and initialize successfully
    printf("Initialization succeeded.");
    pthread_create(&tid, NULL, thread_recv_message, NULL);
    while (1)
    {
        char file_name[FILE_NAME_MAX_SIZE + 1];
        bzero(file_name, FILE_NAME_MAX_SIZE + 1);
        printf("1please enter the filename:\n");

        fgets(file_name, sizeof(file_name), stdin);
        find = strchr(file_name, '\n');
        if (find)
            *find = '\0';
        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);
        netlink_send_message(sock_fd, file_name, strlen(file_name) + 1, 0, 0);
        //打开文件准备写入
        // FILE *fp = fopen(file_name, "w");
        // if (NULL == fp)
        // {
        //     printf("File:\t%s Can Not Open To Write\n", file_name);
        //     exit(1);
        // }
        // printf("file %s open success!\n", file_name);
        // bzero(buffer, BUFFER_SIZE);
        // int length = 0;
        // while (1)
        // {
        //     bzero(buffer, BUFFER_SIZE);
        //     if (netlink_recv_message(sock_fd, buffer, &len) == 0)
        //     {
        //         if (strcmp(buffer, "end") == 0)
        //             break;
        //         printf("recv:%s len:%d\n", buffer, len);
        //         int num = fwrite(buffer, sizeof(char), strlen(buffer), fp);
        //         printf("success to write %d bytes\n", num);
        //     }
        // }
        // fclose(fp);
        // // 接收成功后，关闭文件，关闭socket
        // int ret = 0;
        // ret = md5_checksum(file_name, hashstr);
        // printf("%s  %s\n", hashstr, file_name);
        // //netlink_send_message(sock_fd, hashstr, strlen(hashstr) + 1, 0, 0);
        // printf("Receive File:\t%s From Server IP Successful!\n", file_name);
        // bzero(buffer, sizeof(buffer));
        // bzero(buf, sizeof(buf));
        // bzero(hashstr, sizeof(hashstr));
    }

    close(sock_fd);
    return 0;
}
