#include "../hdr/encapsulation.h"
#include "../hdr/codec.h"
#include "../hdr/hash.h"
#include "../hdr/connect.h"
#include "../hdr/protocol.h"
#include "../hdr/netlink.h"
#include "../hdr/md5sum.h"
char s_hashstr[50][MAX_MSG_SIZE];
int s_hashstr_i = 0;
int s_hashstr_vertify_i = 0;
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
                printf("[kernel message]:%s\n", encode_msg);
                memset(encode_msg, 0, sizeof(encode_msg));
                break;
            case DATA_MSG:
                msg_decode(encode_msg, strlen(encode_msg), msg);
                printf("[message]recv the msg:%s ", msg);
                printf("[sender]%c\n", send);
                hash_calculate(msg, strlen(msg), hashstr1);
                printf("[HASH]");
                print_hexData(hashstr1, 16);
                pack(hashstr1, 16, send, NAME_C, DATA_HASH, hash_send);
                netlink_send_message(sock_fd, hash_send, strlen(hash_send) + 1, PID_C, 0, 0);
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
void *thread_recv_file(void *arg)
{
}
int main(int argc, char *argv[])
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
    char send;
    char passwd[16];
    char sendbuf_con[20];
    char replymsg[MAX_MSG_SIZE];
    char send1;
    char msgtype1;
    if (2 != argc)
    {
        printf("[file mode Usage]./client -f\n");
        printf("[message mode Usage]./client -m\n");
        return -1;
    }
    if ((strcmp("--help", argv[1]) == 0) || ((strcmp("-h", argv[1]) == 0)))
    {
        printf("help\n");
        return 0;
    }
    sock_fd = netlink_init(PID_C);

    if ((strcmp("--file", argv[1]) == 0) || ((strcmp("-f", argv[1]) == 0)))
    {
        int choose = 0;
        char *find;
        char filename[MAX_FILENAME_SIZE] = {0};
        char buffer_filename[MAX_FILENAME_SIZE] = {0};
        char buffer_write[MAX_MSG_SIZE];
        char buffer_read[MAX_MSG_SIZE];
        char buffer_encode[MAX_ENCODE_SIZE];
        char buffer_unpack[MAX_PACK_SIZE];
        char buffer_pack[MAX_PACK_SIZE];
        char send;
        char hashstr_file[64];
        char hashstr_file_cmp[64];
        printf("file mode\n");
        while (1)
        {

            printf("please choose the way:(1 or 2)\n");
            printf("1. download the file from server\n");
            printf("2. upload the file to server\n");
            scanf("%d", &choose);
            getchar();
            if (choose == 1)
            {
                bzero(filename, MAX_FILENAME_SIZE);
                printf("please enter the download file\n");
                fgets(filename, MAX_FILENAME_SIZE, stdin);
                find = strchr(filename, '\n');
                if (find)
                    *find = '\0';
                printf("prepare to download the file %s\n", filename);
                pack(filename, strlen(filename), NAME_A, NAME_C, 'f', buffer_filename);
                netlink_send_message(sock_fd, buffer_filename, strlen(buffer_filename) + 1, PID_C, 0, 0);
                FILE *fp = fopen(filename, "w");
                if (NULL == fp)
                {
                    printf("File:\t%s Can Not Open To Write\n", filename);
                    exit(1);
                }
                printf("file %s open success!\n", filename);
                while (1)
                {
                    if (netlink_recv_message(sock_fd, buffer_pack, &len) == 0)
                    {
                        unpack(buffer_pack, len, &send, &msgtype, buffer_encode);
                        bzero(buffer_pack, MAX_PACK_SIZE);
                        switch (msgtype)
                        {
                        case DATA_FILE:
                            msg_decode(buffer_encode, strlen(buffer_encode), buffer_write);
                            printf("[message]%s[len]%d\n", buffer_write, strlen(buffer_write));
                            int num = fwrite(buffer_write, sizeof(char), strlen(buffer_write), fp);
                            printf("success to write %d bytes\n", num);
                            break;
                        case DATA_FILE_END:
                            memcpy(hashstr_file_cmp, buffer_encode, strlen(buffer_encode));
                            //printf("[recv hash]%s[end]\n", hashstr_file_cmp);
                            goto end;
                            break;
                        default:
                            printf("other msgtype!\n");
                            break;
                        }
                        memset(buf, 0, sizeof(sock_fd));
                        bzero(buffer_encode, MAX_ENCODE_SIZE);
                        bzero(buffer_write, MAX_MSG_SIZE);
                        bzero(buffer_read, MAX_MSG_SIZE);
                        bzero(buffer_pack, MAX_PACK_SIZE);
                        bzero(buffer_unpack, MAX_PACK_SIZE);
                    }
                }
            //
            end:
                fclose(fp);
                int ret = 0;
                ret = md5_checksum(filename, hashstr_file);
                //printf("%s  %s\n", hashstr_file, filename);
                if (strcmp(hashstr_file, hashstr_file_cmp) == 0)
                {
                    printf("file %s download success\n", filename);
                }
                else
                    printf("file %s download failure\n", filename);

                bzero(filename, MAX_FILENAME_SIZE);
                bzero(buffer_encode, MAX_ENCODE_SIZE);
                bzero(buffer_filename, MAX_PACK_SIZE);
                bzero(buffer_write, MAX_MSG_SIZE);
                bzero(buffer_read, MAX_MSG_SIZE);
                bzero(buffer_pack, MAX_PACK_SIZE);
                bzero(buffer_unpack, MAX_PACK_SIZE);
            }
            if (choose == 2)
            {
                bzero(filename, MAX_FILENAME_SIZE);
                printf("please enter the upload file\n");
                fgets(filename, MAX_FILENAME_SIZE, stdin);
                find = strchr(filename, '\n');
                if (find)
                    *find = '\0';
                printf("prepare to upload the file %s\n", filename);
                bzero(filename, MAX_FILENAME_SIZE);
                bzero(buffer_encode, MAX_ENCODE_SIZE);
                bzero(buffer_filename, MAX_PACK_SIZE);
                bzero(buffer_write, MAX_MSG_SIZE);
                bzero(buffer_read, MAX_MSG_SIZE);
                bzero(buffer_pack, MAX_PACK_SIZE);
                bzero(buffer_unpack, MAX_PACK_SIZE);
            }
        }
    }

    if ((strcmp("--message", argv[1]) == 0) || ((strcmp("-m", argv[1]) == 0)))
    {
        printf("message mode\n");
        while (1)
        {
            printf("you are not yet been connected, please enter the password to connect\n");
            printf("[enter the passwd]:");
            fgets(passwd, sizeof(passwd), stdin);
            find = strchr(passwd, '\n');
            if (find)
                *find = '\0';
            pack(passwd, strlen(passwd), NAME_A, NAME_C, DATA_CON, sendbuf_con);
            netlink_send_message(sock_fd, sendbuf_con, strlen(sendbuf_con) + 1, PID_C, 0, 0);
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

            pack(sendbuf_encode, strlen(sendbuf_encode), recv, NAME_C, DATA_MSG, sendbuf_pack);
            printf("[PACK]the packed message is:%s\n", sendbuf_pack);
            // if (strcmp(sendbuf, "exit\n") == 0)
            //     break;
            //send message
            netlink_send_message(sock_fd, sendbuf_pack, strlen(sendbuf_pack) + 1, PID_C, 0, 0);
            memset(sendbuf_pack, 0, sizeof(sendbuf_pack));
            memset(sendbuf_encode, 0, sizeof(sendbuf_encode));
        }

        close(sock_fd);
        return 0;
    }

    return 0;
}
