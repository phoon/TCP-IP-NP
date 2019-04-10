#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, copy_reads;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	
	if(argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
		error_handling("bind() error");
	}

	if(listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}
	
	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while(1) {
		copy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if(fd_num = select(fd_max + 1, &copy_reads, NULL, NULL, &timeout) == -1) {
			//出现错误
			break;
		}

		if(fd_num == 0) {
			//超时期限内无事发生，重新开始监控
			continue;
		}

		//遍历文件描述符
		for(i=0; i<fd_max+1; i++) {
			//有I/O事件的文件描述符
			if(FD_ISSET(i, &copy_reads)) {
				if(i == serv_sock) {	//connection request
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					if(fd_max < clnt_sock) {
						fd_max = clnt_sock;
					}
					printf("connected client: %d \n", clnt_sock);
				} else {	//read message
					str_len = read(i, buf, BUF_SIZE);
					//读到EOF
					if(str_len == 0) {	//close request
						FD_CLR(i, &reads);
						close(i);
						printf("close client: %d \n", i);
					} else {
						write(i, buf, str_len);
					}
				}
			}
		}
	}

	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}
