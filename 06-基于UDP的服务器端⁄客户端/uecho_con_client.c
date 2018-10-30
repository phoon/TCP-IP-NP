/*
* 此程序为uecho_client的已连接UDP套接字版
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;
	
	if(argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1) {
		error_handling("socket() error!");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

	while(1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
			break;
		}

		//已连接UDP套接字，可直接使用write()和read()代替recvfrom()与sendto()
		write(sock, message, strlen(message));
		str_len = read(sock, message, sizeof(message) - 1);
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}