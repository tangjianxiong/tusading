#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/* ABC PID号 */
#define PID_A (100)
#define PID_B (101)
#define PID_C (102)

/*The data type definition*/
#define DATA_MSG 'm'
#define DATA_CON 'r'
#define DATA_KMSG 'k'
#define DATA_HASH 'h'
#define DATA_FILE 'f'
#define DATA_FILE_END 'e'
#define NAME_A 'a'
#define NAME_B 'b'
#define NAME_C 'c'
#define NETLINK_TEST (25)
#define MAX_PAYLOAD (1400)
#define MAX_MSG_SIZE (1024)
#define MAX_FILENAME_SIZE (256)
#define MAX_ENCODE_SIZE (1390)
#define MAX_PACK_SIZE (1400)
#define THREAD_NUMBER 3
/* 最大数据内容长度 */
#define MAX_DATA_SIZE 256

/* 网络数据包 */
typedef struct NET_PACKET
{
	/* 发送方地址 */
	int src_id;

	/* 接收方地址 */
	int dst_id;

	/* 数据类型 */
	int data_type;

	/* 数据内容 */
	char data[MAX_DATA_SIZE];
} Net_packet;

#endif