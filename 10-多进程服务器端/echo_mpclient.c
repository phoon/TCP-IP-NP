#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if(argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
		error_handling("connect() error");
	}

	pid = fork();
	if(pid == 0) {
		write_routine(sock, buf);
	} else {
		read_routine(sock, buf);
	}
	
	close(sock);
	return 0;
}
/*
	分割I/O程序
	父进程负责读数据，子进程负责写数据
	分割I/O的一个好处是提高频繁交换数据的程序的性能
*/

void read_routine(int sock, char *buf)
{
	while(1) {
		int str_len = read(sock, buf, BUF_SIZE);
		if(str_len == 0) {
			return;
		}

		buf[str_len] = 0;
		printf("Message from server: %s\n", buf);
	}
}

void write_routine(int sock, char *buf)
{
	while(1) {
		fgets(buf, BUF_SIZE, stdin);
		if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	}
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}
