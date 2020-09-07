#include "../hdr/encapsulation.h"
#include "../hdr/codec.h"
#include "../hdr/hash.h"
#include "../hdr/connect.h"
#include "../hdr/protocol.h"
#include "../hdr/netlink.h"
// char s_hashstr[50][MAX_MSG_SIZE];
// int s_hashstr_i = 0;
// int s_hashstr_vertify_i = 0;

char hashstr[MAX_MSG_SIZE];
void *thread_recv_message(void *arg)
{
    int thrd_num = *((int *)arg);
    int len;
    int sock_fd = 3;
    char send = 0;
    char msgtype = 0;
    unsigned char buf[MAX_MSG_SIZE];
    unsigned char encode_msg[MAX_MSG_SIZE];
    unsigned char msg[MAX_MSG_SIZE];
    unsigned char hashstr1[16];
    unsigned char hash_send[20];
    unsigned char replystr1[4] = "\0";
    unsigned char replystr2[3] = "\0";
    //printf("recv_thread %d start receiving messages...\n", thrd_num);
    while (1)
    {
        /*
        接收数据流程：
        1. 收到数据buf
        2. 拆包得到send、type、msg
        3. 判断数据类型
            1° 数据msgtype='m':
                a.解码得到原始msg，打印出来
                b.计算msg的hash值，得到hashstr
                c.打包recv+send+msgtype+s_hashstr
                d.发送给内核k
            2° hash校验值msgtype='h':
                a.msg和目前的hashstr[i]进行校验
                b.成功log抛出发送成功
                c.失败log抛出error，根据send的不同errno不同
            3° 来自内核的反馈消息：
                直接打印
            4° 来自客户端的请求类型消息msgtype='r'：
                连接请求：
                    准许连接
                        给内核信号，内核改变消息连接控制信号
                    禁止连接
                        给内核信号，内核不改变控制信号，将拒绝理由返还给请求者
                断开请求：
                    准许断开
                    禁止断开
        */

        if (netlink_recv_message(sock_fd, buf, &len) == 0)
        {
            unpack(buf, len, &send, &msgtype, encode_msg);
            switch (msgtype)
            {
            case DATA_KMSG:
                //printf("[kernel message]:%s\n", encode_msg);
                memset(encode_msg, 0, sizeof(encode_msg));
                break;
            case DATA_MSG:
                msg_decode(encode_msg, strlen(encode_msg), msg);
                printf("[message]recv the msg:%s ", msg);
                printf("[sender]%c\n", send);
                hash_calculate(msg, strlen(msg), hashstr1);
                printf("[HASH]");
                print_hexData(hashstr1, 16);
                pack(hashstr1, 16, send, NAME_A, DATA_HASH, hash_send);
                netlink_send_message(sock_fd, hash_send, strlen(hash_send) + 1, PID_A, 0, 0);
                memset(msg, 0, sizeof(msg));
                memset(hashstr1, 0, sizeof(hashstr1));
                memset(encode_msg, 0, sizeof(encode_msg));
                memset(hash_send, 0, sizeof(hash_send));
                break;
            case DATA_HASH:
                hash_verify(hashstr, encode_msg);
                memset(hashstr, 0, sizeof(hashstr));
                break;
            case DATA_CON:
                strcpy(replystr1, "y");
                strcpy(replystr2, "no");
                if (passwd_vertify(encode_msg, send))
                {
                    pack(replystr1, strlen(replystr1), send, NAME_A, DATA_CON, msg);
                    netlink_send_message(sock_fd, msg, strlen(msg) + 1, PID_A, 0, 0);
                }
                else
                {
                    pack(replystr2, strlen(replystr2), send, NAME_A, DATA_CON, msg);
                    netlink_send_message(sock_fd, msg, strlen(msg) + 1, PID_A, 0, 0);
                }
                bzero(replystr1, sizeof(replystr1));
                bzero(replystr2, sizeof(replystr2));
                break;
            case DATA_FILE:
                printf("file mode\n");
                printf("sending the file:%s\n", encode_msg);
                break;
            default:
                break;
            }
            bzero(buf, MAX_MSG_SIZE + 1);
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
    //creat socket
    sock_fd = netlink_init(PID_A);
    for (no = 0; no < THREAD_NUMBER; no++)
    {
        res = pthread_create(&tid[no], NULL, thread_recv_message, &no);
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
        2. 根据原始消息计算HASH值，依次存入字符串数组hashstr[50]中
        3. 将消息进行编码，得到encodesendbuf
        4. 打包，得到recv+send+msgtype+encodesendbuff
        5. 将消息发送给内核k
        */
        printf("\nplease enter the message:(enter the 'exit' to stop)\n");
        printf("[input msg]:");
        fgets(sendbuf, sizeof(sendbuf), stdin);

        find = strchr(sendbuf, '\n');
        if (find)
            *find = '\0';

        hash_calculate(sendbuf, strlen(sendbuf), hashstr);
        //printf("[HASH]The original hash value:");
        //print_hexData(s_hashstr[s_hashstr_i], 16);
        msg_encode(sendbuf, strlen(sendbuf), sendbuf_encode);
        memset(sendbuf, 0, sizeof(sendbuf));
        printf("[CODEC]the encoded message is:%s\n", sendbuf_encode);
        printf("please enter the recv:\n");
        printf("[input recvier]:");
        scanf("%c", &recv);
        getchar();

        pack(sendbuf_encode, strlen(sendbuf_encode), recv, 'a', 'm', sendbuf_pack);
        printf("[PACK]the packed message is:%s\n", sendbuf_pack);
        // if (strcmp(sendbuf, "exit\n") == 0)
        //     break;
        //send message
        netlink_send_message(sock_fd, sendbuf_pack, strlen(sendbuf_pack) + 1, PID_A, 0, 0);
        memset(sendbuf_pack, 0, sizeof(sendbuf_pack));
        memset(sendbuf_encode, 0, sizeof(sendbuf_encode));
    }

    close(sock_fd);
    return 0;
}
