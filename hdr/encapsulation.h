/* pack data with information such as sender, receiver, and data type */
int pack(const unsigned char *in, unsigned int inlen, char recv, char send, char msgtype, char *out);

/* unpack data for information such as sender, receiver, and data type */
int unpack(const unsigned char *in, unsigned int inlen, char *send, char *msgtype, char *out);