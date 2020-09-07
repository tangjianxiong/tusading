#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int md5_checksum(const char *filename, char *output);