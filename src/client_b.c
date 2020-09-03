#include "../hdr/encapsulation.h"
#include "../hdr/codec.h"
#include "../hdr/hash.h"
#include "../hdr/connect.h"
#include "../hdr/protocol.h"
#include "../hdr/netlink.h"
char s_hashstr[50][MAX_MSG_SIZE];
int s_hashstr_i = 0;
int s_hashstr_vertify_i = 0;
void *thread_recv_message(void *arg)
{
    int thrd_num = *((int *)arg);
    int len;
    int sock_fd = 3;
    tangjianxiong char send = 0;
    char msgtype = 0;
    unsigned char buf[MAX_MSG_SIZE];
    unsigned char encode_msg[MAX_MSG_SIZE];
    unsigned char msg[MAX_MSG_SIZE];
    unsigned char hashstr1[16];
    unsigned char hash_send[20];
    //printf("recv_thread %d start receiving messages...\n", thrd_num);
    while (1)
    {
        if (netlink_recv_message(sock_fd, buf, &len) == 0)
        {
            //printf("[thread]thread %d has recv the msg.\n", thrd_num);
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
                pack(hashstr1, 16, send, NAME_B, DATA_HASH, hash_send);
                netlink_send_message(sock_fd, hash_send, strlen(hash_send) + 1, PID_B, 0, 0);
                memset(msg, 0, sizeof(msg));
                memset(hashstr1, 0, sizeof(hashstr1));
                memset(encode_msg, 0, sizeof(encode_msg));
                memset(hash_send, 0, sizeof(hash_send));
                break;
            case DATA_HASH:
                printf("the %d hash vertify\n", s_hashstr_vertify_i);
                hash_verify(s_hashstr[s_hashstr_vertify_i], encode_msg);
                s_hashstr_vertify_i++;
                break;
            case DATA_CON:
                break;
            default:
                break;
            }
            memset(buf, 0, sizeof(sock_fd));
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
    char passwd[16];
    char sendbuf_con[20];
    char replymsg[MAX_MSG_SIZE];
    char send1;
    char msgtype1;
    sock_fd = netlink_init(PID_B);
    while (1)
    {
        printf("you are not yet been connected, please enter the password to connect\n");
        printf("[enter the passwd]:");
        fgets(passwd, sizeof(passwd), stdin);

        find = strchr(passwd, '\n');
        if (find)
            *find = '\0';
        pack(passwd, strlen(passwd), NAME_A, NAME_B, DATA_CON, sendbuf_con);
        netlink_send_message(sock_fd, sendbuf_con, strlen(sendbuf_con) + 1, PID_B, 0, 0);
        memset(sendbuf_con, 0, sizeof(sendbuf_con));
        printf("waitting the server...\n");
        netlink_recv_message(sock_fd, buf, &len);
        netlink_recv_message(sock_fd, buf, &len);
        unpack(buf, strlen(buf), &send1, &msgtype1, replymsg);
        if (replymsg[0] == 'y')
        {
            printf("connect success!\n");
            break;
        }
        memset(buf, 0, sizeof(buf));
        memset(replymsg, 0, sizeof(replymsg));
    }
    //pthread_create(&tid, NULL, thread_recv_message, NULL);
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
        printf("\nplease enter the message:(enter the 'exit' to stop)\n");
        printf("[input msg]:");
        fgets(sendbuf, sizeof(sendbuf), stdin);

        find = strchr(sendbuf, '\n');
        if (find)
            *find = '\0';

        hash_calculate(sendbuf, strlen(sendbuf), s_hashstr[s_hashstr_i]);
        printf("[HASH]The original hash value:");
        print_hexData(s_hashstr[s_hashstr_i], 16);
        s_hashstr_i++;
        msg_encode(sendbuf, strlen(sendbuf), sendbuf_encode);
        memset(sendbuf, 0, sizeof(sendbuf));
        printf("[CODEC]the encoded message is:%s\n", sendbuf_encode);
        printf("please enter the recv:\n");
        printf("[input recvier]:");
        scanf("%c", &recv);
        getchar();

        pack(sendbuf_encode, strlen(sendbuf_encode), recv, NAME_B, DATA_MSG, sendbuf_pack);
        printf("[PACK]the packed message is:%s[end]", sendbuf_pack);
        // if (strcmp(sendbuf, "exit\n") == 0)
        //     break;
        //send message
        netlink_send_message(sock_fd, sendbuf_pack, strlen(sendbuf_pack) + 1, PID_B, 0, 0);
        memset(sendbuf_pack, 0, sizeof(sendbuf_pack));
        memset(sendbuf_encode, 0, sizeof(sendbuf_encode));
    }

    close(sock_fd);
    return 0;
}
