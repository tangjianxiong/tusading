int pack(const unsigned char *in, unsigned int inlen, char recv, char send, char msgtype, char *out);
int unpack(const unsigned char *in, unsigned int inlen, char *send, char *msgtype, char *out);