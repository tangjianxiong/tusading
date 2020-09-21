#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>
#include <pthread.h>
/* create netlink socket */
int netlink_create_socket(void);
/* netlink address bind */
int netlink_bind(int sock_fd, int p_id);
/*  send  netlink message*/
int netlink_send_message(int sock_fd, const unsigned char *message, int len, unsigned int send_pid, unsigned int recv_pid, unsigned int group);
/*  recv  netlink message*/
int netlink_recv_message(int sock_fd, unsigned char *message, int *len);
/*  netlink initialization*/
int netlink_init(int id);