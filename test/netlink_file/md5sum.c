#include "./hdr/md5sum.h"

int md5_checksum(const char *filename, char *output)
{
    int ret = 0;
    int file_len = 0, read_len = 0;
    int loop = 0;
    FILE *fp = NULL;
    char *buf = NULL;
    unsigned char digest[16];
    struct stat stat_buf;
    MD5_CTX context;

    memset(&stat_buf, 0, sizeof(struct stat));
    ret = stat(filename, &stat_buf);
    if (ret < 0)
    {
        perror("stat");
        ret = -1;
        goto errexit;
    }
    file_len = stat_buf.st_size;

    buf = malloc(file_len);
    if (NULL == buf)
    {
        perror("malloc");
        return -1;
    }
    memset(buf, 0, file_len);

    fp = fopen(filename, "rb");
    if (NULL == fp)
    {
        perror("fopen");
        ret = -1;
        goto errexit;
    }
    read_len = fread(buf, 1, file_len, fp);

    MD5_Init(&context);
    MD5_Update(&context, buf, read_len);
    MD5_Final(digest, &context);

    for (loop = 0; loop < sizeof(digest) / sizeof(char); loop++)
    {
        sprintf(&(output[loop * 2]), "%02x", (unsigned char)digest[loop]);
        sprintf(&(output[loop * 2 + 1]), "%02x", (unsigned char)(digest[loop] << 4));
    }

    *(output + strlen(output) - 1) = '\0';
errexit:
    if (NULL != fp)
    {
        fclose(fp);
        fp = NULL;
    }
    if (NULL != buf)
    {
        free(buf);
        buf = NULL;
    }
    return ret;
}

// int main(int argc, char *argv[])
// {
//     int ret = 0;
//     char output[64] = {0};

//     if (2 != argc)
//     {
//         printf("Usage: ./a.out FILENAME\n.");
//         return -1;
//     }

//     ret = md5_checksum(argv[1], output);
//     printf("%s\n", output);

//     return ret;
// }
