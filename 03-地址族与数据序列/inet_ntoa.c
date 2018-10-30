#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
	struct sockaddr_in addr1, addr2;
	char *str_ptr;
	char str_arr[20];

	addr1.sin_addr.s_addr = htonl(0x1020304);
	addr2.sin_addr.s_addr = htonl(0x1010101);

	str_ptr = inet_ntoa(addr1.sin_addr);
	strcpy(str_arr, str_ptr);	//复制字符串
	printf("Dotted-Decimal ntoation1: %s \n", str_ptr);

	str_ptr = inet_ntoa(addr2.sin_addr);
	printf("Dotted-Decimal ntoation2: %s \n", str_ptr);
	printf("Dotted-Decimal ntoation3: %s \n", str_arr);

	return 0;
}