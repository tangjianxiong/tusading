#include "../hdr/encapsulation.h"
#include "../hdr/codec.h"
#include "../hdr/hash.h"
#include "../hdr/connect.h"
#include "../hdr/protocol.h"
#include "../hdr/netlink.h"

char g_hashstr[MAX_MSG_SIZE] = {0};
char g_hashstr_file[64] = {0};
char g_filename_upload_b[MAX_FILENAME_SIZE] = {0};
char g_filename_upload_c[MAX_FILENAME_SIZE] = {0};
FILE *g_fp1 = NULL;
FILE *g_fp2 = NULL;

void *thread_recv_message(void *arg)
{
    int len = 0;
    int sock_fd = 3;
    char send = 0;
    char msgtype = 0;
    unsigned char buf[MAX_PACK_SIZE];
    unsigned char encode_msg[MAX_ENCODE_SIZE];
    unsigned char msg[MAX_MSG_SIZE];
    unsigned char hashstr1[16];
    unsigned char hash_send[20];
    unsigned char replystr1[4] = {0};
    unsigned char replystr2[3] = {0};
    char filename_download[MAX_FILENAME_SIZE] = {0};

    char buffer_filename[MAX_PACK_SIZE] = {0};
    char buffer_write[MAX_MSG_SIZE];
    char buffer_read[MAX_MSG_SIZE];
    char buffer_encode[MAX_ENCODE_SIZE];
    char buffer_unpack[MAX_PACK_SIZE];
    char buffer_pack[MAX_PACK_SIZE];
    char buffer_hash[64];
    //printf("recv_thread %d start receiving messages...\n", thrd_num);
    while (1)
    {
        if (netlink_recv_message(sock_fd, buf, &len) == 0)
        {
            unpack(buf, len, &send, &msgtype, encode_msg);
            switch (msgtype)
            {
            case DATA_KMSG:
                memset(encode_msg, 0, sizeof(encode_msg));
                break;
            case DATA_MSG:
                msg_decode(encode_msg, strlen(encode_msg), msg);
                printf("[recv message]%s[end]\n", msg);
                printf("[sender]%c\n", send);
                hash_str(msg, strlen(msg), hashstr1);
                // printf("[HASH]");
                // print_hexData(hashstr1, 16);
                pack(hashstr1, 16, send, NAME_A, DATA_HASH, hash_send);
                netlink_send_message(sock_fd, hash_send, strlen(hash_send) + 1, PID_A, 0, 0);
                memset(msg, 0, sizeof(msg));
                memset(hashstr1, 0, sizeof(hashstr1));
                memset(encode_msg, 0, sizeof(encode_msg));
                memset(hash_send, 0, sizeof(hash_send));
                break;
            case DATA_HASH:
                hash_verify(g_hashstr, encode_msg);
                memset(g_hashstr, 0, sizeof(g_hashstr));
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
            case DATA_FILE_DOWNLOAD:
                bzero(filename_download, MAX_FILENAME_SIZE);
                strncpy(filename_download, encode_msg, strlen(encode_msg));
                printf("prepare to send file:%s\n", filename_download);
                // printf("file mode\n");
                printf("sending the file:%s\n", filename_download);
                bzero(g_hashstr_file, sizeof(g_hashstr_file));
                hash_file(filename_download, g_hashstr_file);
                printf("%s  %s\n", g_hashstr_file, filename_download);
                //open file and send to client
                FILE *fp = fopen(filename_download, "r");
                if (NULL == fp)
                {
                    printf("File:%s Not Found\n", filename_download);
                }
                else
                {
                    bzero(buffer_read, MAX_MSG_SIZE);
                    int length = 0;
                    /* Read the data and send it to the client, looping until the file is read */
                    while ((length = fread(buffer_read, sizeof(char), MAX_MSG_SIZE, fp)) > 0)
                    {
                        //usleep(5000);
                        msg_encode(buffer_read, strlen(buffer_read), buffer_encode);
                        pack(buffer_encode, strlen(buffer_encode), send, NAME_A, DATA_FILE_DOWNLOAD, buffer_pack);
                        netlink_send_message(sock_fd, buffer_pack, strlen(buffer_pack) + 1, PID_A, 0, 0);
                        printf("send len %d msg\n", length);
                        bzero(buffer_read, MAX_MSG_SIZE);
                        bzero(buffer_encode, MAX_MSG_SIZE);
                        bzero(buffer_pack, MAX_PACK_SIZE);
                    }
                    /* After the message is sent, the file hash value is sent for verification. */
                    pack(g_hashstr_file, strlen(g_hashstr_file), send, NAME_A, 'e', buffer_pack);
                    netlink_send_message(sock_fd, buffer_pack, strlen(buffer_pack) + 1, PID_A, 0, 0);
                    fclose(fp);
                    printf("File:%s Transfer Successful!\n", filename_download);
                }

                bzero(encode_msg, MAX_MSG_SIZE);
                bzero(filename_download, MAX_FILENAME_SIZE);
                bzero(buffer_encode, MAX_ENCODE_SIZE);
                bzero(buffer_filename, MAX_PACK_SIZE);
                bzero(buffer_write, MAX_MSG_SIZE);
                bzero(buffer_read, MAX_MSG_SIZE);
                bzero(buffer_pack, MAX_PACK_SIZE);
                bzero(buffer_unpack, MAX_PACK_SIZE);
                break;
            case DATA_FILE_UPLOAD:
                switch (send)
                {
                case NAME_B:
                    bzero(g_filename_upload_b, MAX_FILENAME_SIZE);
                    strncpy(g_filename_upload_b, encode_msg, strlen(encode_msg));
                    printf("recving the file:%s from B\n", g_filename_upload_b);
                    g_fp2 = fopen(g_filename_upload_b, "w");
                    if (NULL == g_fp2)
                    {
                        printf("File:\t%s Can Not Open To Write\n", g_filename_upload_b);
                    }
                    break;
                case NAME_C:
                    bzero(g_filename_upload_c, MAX_FILENAME_SIZE);
                    strncpy(g_filename_upload_c, encode_msg, strlen(encode_msg));
                    printf("recving the file:%s from C\n", g_filename_upload_c);
                    g_fp1 = fopen(g_filename_upload_c, "w");
                    if (NULL == g_fp1)
                    {
                        printf("File:\t%s Can Not Open To Write\n", g_filename_upload_c);
                    }
                    break;
                default:
                    break;
                }
                break;
            case DATA_FILE_TXT:
                switch (send)
                {
                case NAME_B:
                    bzero(buffer_encode, MAX_ENCODE_SIZE);
                    strncpy(buffer_encode, encode_msg, strlen(encode_msg));
                    msg_decode(buffer_encode, strlen(buffer_encode), buffer_write);
                    //printf("[message]%s[len]%d\n", buffer_write, strlen(buffer_write));
                    int num1 = fwrite(buffer_write, sizeof(char), strlen(buffer_write), g_fp2);
                    printf("success to write %d bytes\n", num1);
                    bzero(buffer_write, MAX_MSG_SIZE);
                    bzero(encode_msg, MAX_ENCODE_SIZE);
                    bzero(buffer_encode, MAX_ENCODE_SIZE);
                    break;
                case NAME_C:
                    bzero(buffer_encode, MAX_ENCODE_SIZE);
                    strncpy(buffer_encode, encode_msg, strlen(encode_msg));
                    msg_decode(buffer_encode, strlen(buffer_encode), buffer_write);
                    //printf("[message]%s[len]%d\n", buffer_write, strlen(buffer_write));
                    int num = fwrite(buffer_write, sizeof(char), strlen(buffer_write), g_fp1);
                    printf("success to write %d bytes\n", num);
                    bzero(buffer_write, MAX_MSG_SIZE);
                    bzero(encode_msg, MAX_ENCODE_SIZE);
                    bzero(buffer_encode, MAX_ENCODE_SIZE);
                    break;
                default:
                    break;
                }
                break;
            case DATA_FILE_END:
                switch (send)
                {
                case NAME_B:
                    fclose(g_fp2);
                    hash_file(g_filename_upload_b, buffer_hash);
                    printf("success recv from B:[hash]%s[file]%s\n", buffer_hash, g_filename_upload_b);
                    pack(buffer_hash, strlen(buffer_hash), NAME_B, NAME_A, DATA_FILE_END, buffer_pack);
                    netlink_send_message(sock_fd, buffer_pack, strlen(buffer_pack) + 1, PID_A, 0, 0);
                    bzero(buffer_hash, sizeof(buffer_hash));
                    bzero(buffer_pack, MAX_PACK_SIZE);
                    break;
                case NAME_C:
                    fclose(g_fp1);
                    hash_file(g_filename_upload_c, buffer_hash);
                    printf("success recv from C:[hash]%s[file]%s\n", buffer_hash, g_filename_upload_c);
                    pack(buffer_hash, strlen(buffer_hash), NAME_C, NAME_A, DATA_FILE_END, buffer_pack);
                    netlink_send_message(sock_fd, buffer_pack, strlen(buffer_pack) + 1, PID_A, 0, 0);
                    bzero(buffer_hash, sizeof(buffer_hash));
                    bzero(buffer_pack, MAX_PACK_SIZE);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            bzero(buf, MAX_MSG_SIZE);
            bzero(encode_msg, MAX_MSG_SIZE);
        }
    }
}
int main()
{
    int sock_fd;
    unsigned char sendbuf[MAX_MSG_SIZE];
    unsigned char sendbuf_encode[MAX_MSG_SIZE];
    unsigned char sendbuf_pack[MAX_MSG_SIZE];

    pthread_t tid[THREAD_NUMBER];
    int no;
    char recv;
    /* Communication initialization */
    sock_fd = netlink_init(PID_A);
    for (no = 0; no < THREAD_NUMBER; no++)
    {
        int res = pthread_create(&tid[no], NULL, thread_recv_message, &no);
        if (res != 0)
        {
            printf("create msg_recv_thread %d failed\n", no);
            exit(res);
        }
    }

    while (1)
    {
        char *find;
        printf("\nplease enter the message:(enter the 'exit' to stop)\n");
        printf("[input msg]:");
        fgets(sendbuf, sizeof(sendbuf), stdin);

        find = strchr(sendbuf, '\n');
        if (find)
            *find = '\0';

        hash_str(sendbuf, strlen(sendbuf), g_hashstr);
        //printf("[HASH]The original hash value:");
        //print_hexData(s_hashstr[s_hashstr_i], 16);
        msg_encode(sendbuf, strlen(sendbuf), sendbuf_encode);
        memset(sendbuf, 0, sizeof(sendbuf));
        //printf("[CODEC]the encoded message is:%s\n", sendbuf_encode);
        //printf("please enter the recv:\n");
        printf("[input recvier]:");
        scanf("%c", &recv);
        getchar();

        pack(sendbuf_encode, strlen(sendbuf_encode), recv, 'a', 'm', sendbuf_pack);
        //printf("[PACK]the packed message is:%s\n", sendbuf_pack);

        netlink_send_message(sock_fd, sendbuf_pack, strlen(sendbuf_pack) + 1, PID_A, 0, 0);
        memset(sendbuf_pack, 0, sizeof(sendbuf_pack));
        memset(sendbuf_encode, 0, sizeof(sendbuf_encode));
    }

    close(sock_fd);
    return 0;
}
