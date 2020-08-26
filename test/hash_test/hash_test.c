#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../hdr/hash.h"

//以16进制方式打印目标字符串

int main()
{
	char md5_plaintext[1024]; //明文为字符串
	char md5_result[16];	  //MD5计算除的摘要
	char md5_plaintext1[1024];
	char md5_result1[16]; //MD5计算除的摘要
	int md5_len, md5_len1;

	printf("要计算的原始消息是：\n");
	puts(md5_plaintext);
	md5_len = strlen(md5_plaintext); //计算明文的长度

	Hash_Calculate(md5_plaintext, md5_len, md5_result); //调用MD5函数
	printf("明文长度:%d\n", md5_len);
	printf("明文:%s \n密文:%s\n", md5_plaintext, md5_result);
	Print_HexData(md5_result, 16);

	printf("要计算的原始消息是：\n");
	puts(md5_plaintext1);
	md5_len1 = strlen(md5_plaintext1); //计算明文的长度

	Hash_Calculate(md5_plaintext1, md5_len1, md5_result1); //调用MD5函数
	printf("明文长度:%d\n", md5_len);
	printf("明文:%s \n密文:%s\n", md5_plaintext1, md5_result1);

	Print_HexData(md5_result1, 16);
	Hash_verify(md5_result1, md5_result);
}
