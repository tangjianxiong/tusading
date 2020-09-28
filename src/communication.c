#include "../hdr/communication.h"

/* send message */
int communication_send_message(int sock_fd, const unsigned char *message, int len, unsigned int send_pid, unsigned int recv_pid, unsigned int group)
{
	return netlink_send_message(sock_fd, message, len, send_pid, recv_pid, group);
}
/* recv message*/
int communication_recv_message(int sock_fd, unsigned char *message, int *len)
{
	return netlink_recv_message(sock_fd, message, len);
}
/* communication initialization*/
int communication_init(int id)
{
	return netlink_init(id);
}