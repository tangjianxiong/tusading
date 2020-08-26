int Pack(const unsigned char *in, unsigned int inlen, char recv, char send,char msgtype, char *out);
int UnPack(const unsigned char *in, unsigned int inlen, char *send, char *msgtype, char *out);