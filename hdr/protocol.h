/*
 * Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 * All rights reserved.
 * 
 * CreateDate   : 2020-09-22
 * Author       : Jianxiong Tang
 * LastEditTime : 2020-09-30
 * FilePath     : /tusading/hdr/protocol.h
 * Description  : protocol header file
 */
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/* ABC Port ID */
#define PID_A (100)
#define PID_B (101)
#define PID_C (102)

/*The data type definition*/
#define DATA_MSG 'm'
#define DATA_CON 'r'
#define DATA_KMSG 'k'
#define DATA_HASH 'h'
#define DATA_FILE_DOWNLOAD 'f'
#define DATA_FILE_END 'e'
#define DATA_FILE_UPLOAD 'u'
#define DATA_FILE_TXT 't'

/* application name */
#define NAME_A 'a'
#define NAME_B 'b'
#define NAME_C 'c'
/*  Maximum field length*/
#define NETLINK_TEST (25)
#define MAX_PAYLOAD (1400)
#define MAX_MSG_SIZE (1024)
#define MAX_FILENAME_SIZE (256)
#define MAX_ENCODE_SIZE (1390)
#define MAX_PACK_SIZE (1400)
#define THREAD_NUMBER 3
#define MAX_DATA_SIZE 256

#endif