#include "netlink.h"

/* send message */
int communication_send_message(int sock_fd, const unsigned char *message, int len, unsigned int send_pid, unsigned int recv_pid, unsigned int group);
/* recv message*/
int communication_recv_message(int sock_fd, unsigned char *message, int *len);
/* communication initialization*/
int communication_init(int id);