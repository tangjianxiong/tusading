#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/* ABC PID号 */
#define PID_A (100)
#define PID_B (101)
#define PID_C (102)

/* 数据类型定义 */
#define DATA_NONE	0x00
#define DATA_ONLINE 0x01 
#define DATA_CHAT	0x02
#define DATA_LOGIN	0x04
#define DATA_LOGOUT	0x08
#define DATA_FILE	0x10

/* 最大数据内容长度 */
#define MAX_DATA_SIZE 256

/* 网络协议包 */
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