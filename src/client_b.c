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
#include "../hdr/encapsulation.h"
#include "../hdr/codec.h"
#include "../hdr/hash.h"
#include "../hdr/connect.h"
#define NETLINK_TEST (25)
#define MAX_PAYLOAD (1024)
#define MAX_MSG_SIZE (1024)
#define THREAD_NUMBER 3
char s_hashstr[50][MAX_MSG_SIZE];
int s_hashstr_i = 0;
int s_hashstr_vertify_i = 0;
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
    addr.nl_pid = PID_B;
    addr.nl_groups = 0;

    return bind(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_nl));
}

int netlink_send_message(int sock_fd, const unsigned char *message, int len, unsigned int pid, unsigned int group)
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
    nlh->nlmsg_pid = PID_B;
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
    *len = nlh->nlmsg_len - NLMSG_SPACE(0);
    memcpy(message, (unsigned char *)NLMSG_DATA(nlh), *len);

    free(nlh);
    return 0;
}
void *thread_recv_message(void *arg)
{
    int thrd_num = *((int *)arg);
    int len;
    int sock_fd = 3;
    char send = '0';
    char msgtype = '0';
    unsigned char buf[MAX_MSG_SIZE];
    unsigned char encode_msg[MAX_MSG_SIZE];
    unsigned char msg[MAX_MSG_SIZE];
    unsigned char hashstr1[16];
    unsigned char hash_send[20];

    printf("recv_thread %d start receiving messages...\n", thrd_num);
    while (1)
    {
        /*
        接收数据流程：
        1. 收到数据buf
        2. 拆包得到send、type、msg
        3. 判断数据类型
            1° 数据msgtype='m':
                a.解码得到原始msg，打印出来
                b.计算msg的hash值，得到s_hashstr
                c.打包recv+send+msgtype+s_hashstr
                d.发送给内核k
            2° hash校验值msgtype='h':
                a.msg和目前的s_hashstr[i]进行校验
                b.成功log抛出发送成功
                c.失败log抛出error，根据send的不同errno不同
            3° 来自内核的反馈消息：
                直接打印
        */

        if (netlink_recv_message(sock_fd, buf, &len) == 0)
        {
            printf("[thread]thread %d has recv the msg.\n", thrd_num);
            unpack(buf, len, &send, &msgtype, encode_msg);
            if (send == 'k')
                printf("[kernel message]:%s\n", encode_msg);
            if (msgtype == 'm')
            {
                msg_decode(encode_msg, strlen(encode_msg), msg);
                printf("[message]recv the msg:%s ", msg);
                printf("the sender is:%c\n", send);
                hash_calculate(msg, strlen(msg), hashstr1);
                printf("[HASH]");
                print_hexData(hashstr1, 16);
                pack(hashstr1, 16, send, 'b', 'h', hash_send);
                netlink_send_message(sock_fd, hash_send, strlen(hash_send) + 1, 0, 0);
            }
            if (msgtype == 'h')
            {
                printf("the %d hash vertify\n", s_hashstr_vertify_i);
                hash_verify(s_hashstr[s_hashstr_vertify_i], encode_msg);
                s_hashstr_vertify_i++;
            }
        }
    }
}
int main()
{
    int sock_fd;
    int len;
    char *find;
    unsigned char sendbuf[MAX_MSG_SIZE]; //缓存输入数据
    unsigned char sendbuf_encode[MAX_MSG_SIZE];
    unsigned char sendbuf_pack[MAX_MSG_SIZE];
    unsigned char buf[MAX_MSG_SIZE]; //接收消息
    unsigned char buf_hash[MAX_MSG_SIZE];
    pthread_t tid[THREAD_NUMBER];
    int no, res;
    char recv;
    char msgtype;
    void *void_pointer;
    //creat socket
    sock_fd = netlink_create_socket();
    if (sock_fd == -1)
    {
        printf("create socket error!\n");
        return -1;
    }
    //bind the socket to the local address.
    if (netlink_bind(sock_fd) < 0)
    {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    //Create socket and initialize successfully
    printf("Initialization succeeded.");
    //pthread_create(&tid, NULL, thread_recv_message, NULL);
    for (no = 0; no < THREAD_NUMBER; no++)
    {
        void_pointer = &no;
        res = pthread_create(&tid[no], NULL, thread_recv_message, void_pointer);
        if (res != 0)
        {
            printf("create msg_recv_thread %d failed\n", no);
            exit(res);
        }
    }

    while (1)
    {
        /*
        发送消息流程
        1. 获取原始消息和消息的去向（recv）
        2. 根据原始消息计算HASH值，依次存入字符串数组s_hashstr[50]中
        3. 将消息进行编码，得到encodesendbuf
        4. 打包，得到recv+send+msgtype+encodesendbuff
        5. 将消息发送给内核k
        */
        printf("\nplease enter the message:(enter the 'exit' to stop)\n");
        fgets(sendbuf, sizeof(sendbuf), stdin);
        find = strchr(sendbuf, '\n');
        if (find)
            *find = '\0';

        hash_calculate(sendbuf, strlen(sendbuf), s_hashstr[s_hashstr_i]);
        printf("[HASH]The original hash value:");
        print_hexData(s_hashstr[s_hashstr_i], 16);
        s_hashstr_i++;
        msg_encode(sendbuf, strlen(sendbuf), sendbuf_encode);
        printf("[CODEC]the encoded message is:%s\n", sendbuf_encode);
        printf("please enter the recv:\n");
        scanf("%c", &recv);
        getchar();

        pack(sendbuf_encode, strlen(sendbuf_encode), recv, 'b', 'm', sendbuf_pack);
        printf("[PACK]the packed message is:%s\n", sendbuf_pack);
        // if (strcmp(sendbuf, "exit\n") == 0)
        //     break;
        //send message
        netlink_send_message(sock_fd, sendbuf_pack, strlen(sendbuf_pack) + 1, 0, 0);
    }

    close(sock_fd);
    return 0;
}
