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
#include "../../hdr/hash.h"

#define NETLINK_TEST (25)
#define MAX_BUFF_SIZE (1024)
#define MAX_PAYLOAD (1024)
#define TEST_PID (100)
#define FILE_NAME_MAX_SIZE (512)

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
    nlh->nlmsg_pid = TEST_PID;
    nlh->nlmsg_flags = 0;
    memcpy(NLMSG_DATA(nlh), message, len);

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    memset(&dest_addr, 0, sizeof(struct sockaddr_nl));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = pid;
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
        printf("recvmsg error!\n");
        return -3;
    }
    *len = strlen(NLMSG_DATA(nlh));
    memcpy(message, (unsigned char *)NLMSG_DATA(nlh), *len);

    free(nlh);
    return 0;
}
void *thread_recv_message(void *arg)
{
    printf("Start receiving messages...\n");
    int len;
    int sock_fd = 3;
    char buffer[MAX_BUFF_SIZE];
    char endstr[] = "end";
    bzero(buffer, MAX_BUFF_SIZE);
    char hashstr[64] = {0};
    while (1)
    {
        if (netlink_recv_message(sock_fd, buffer, &len) == 0)
        {
            char file_name[FILE_NAME_MAX_SIZE + 1];
            bzero(file_name, FILE_NAME_MAX_SIZE + 1);
            strncpy(file_name, buffer, strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));
            printf("prepare to send file:%s\n", file_name);
            // 打开文件并读取文件数据
            //计算文件file_name的md5值保存到hashstr
            bzero(hashstr, sizeof(hashstr));
            int ret = 0;
            ret = md5_checksum(file_name, hashstr);
            //判断md5值计算正确与否
            printf("%s  %s\n", hashstr, file_name);
            FILE *fp = fopen(file_name, "r");
            if (NULL == fp)
            {
                printf("File:%s Not Found\n", file_name);
            }
            else
            {
                bzero(buffer, MAX_BUFF_SIZE);
                int length = 0;
                // 每读取一段数据，便将其发送给客户端，循环直到文件读完为止
                while ((length = fread(buffer, sizeof(char), MAX_BUFF_SIZE, fp)) > 0)
                {
                    netlink_send_message(sock_fd, buffer, strlen(buffer) + 1, 0, 0);
                    printf("[sendlen]%d\n", length);
                    bzero(buffer, MAX_BUFF_SIZE);
                }
                netlink_send_message(sock_fd, endstr, strlen(endstr) + 1, 0, 0);
                // 关闭文件
                fclose(fp);

                //获取客户端接收到的文件的md5值，进行校验
                //返回校验结果给客户端
                printf("File:%s Transfer Successful!\n", file_name);
                bzero(hashstr, sizeof(hashstr));
                bzero(buffer, sizeof(buffer));
            }
        }
    }
}
int main()
{
    int sock_fd;
    int len;
    unsigned char sendbuf[1024];
    unsigned char buf[2048];
    char *find;
    pthread_t tid;
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
    printf("Initialization succeeded.");
    pthread_create(&tid, NULL, thread_recv_message, NULL);
    while (1)
    {
        printf("please enter the message:(enter the 'exit' to stop)\n");
        fgets(sendbuf, sizeof(sendbuf), stdin);
        find = strchr(sendbuf, '\n');
        if (find)
            *find = '\0';

        netlink_send_message(sock_fd, sendbuf, strlen(sendbuf) + 1, 0, 0);
    }

    close(sock_fd);
    return 0;
}
