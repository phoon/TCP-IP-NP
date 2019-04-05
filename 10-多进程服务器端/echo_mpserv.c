/*
	echo服务器多进程版
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

/*
	多进程版echo服务器主要有三个阶段：
	1. 服务器端（父进程）通过调用accept函数受理连接请求
	2. 获取的套接字文件描述符创建并传递给子进程
	3. 子进程利用传递来的文件描述符提供服务
*/

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];

	if(argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	//回收完成任务的子进程，避免其成为僵尸进程
	state = sigaction(SIGCHLD, &act, 0);
	if(state == -1) {
		error_handling("sigaction() error");
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
		error_handling("bind() error");
	}
	if(listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}
	
	while(1) {
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock == -1) {
			continue;
		}else {
			puts("New client connected...");
		}

		pid = fork();
		if(pid == -1) {
			close(clnt_sock);
			continue;
		}

		if(pid == 0) {
			//子进程执行区域
			
			/*
				调用fork()之后，由于子进程复制了父进程的文件描述符，
				使得同一套接字中存在2个文件描述符，必须关闭无关的套接字文件描述符
				
				子进程仅需clnt_sock来与客户端进行通信，同理父进程在将通信分交给子
				进程后，无需拥有clnt_sock
			*/
			//子进程关闭监听套接字
			close(serv_sock);
			while(str_len = read(clnt_sock, buf, BUF_SIZE) != 0) {
				write(clnt_sock, buf, str_len);
			}
			
			close(clnt_sock);
			puts("Client disconnected...");
			return EXIT_SUCCESS;
		}else {
			//父进程关闭无需用的clnt_sock
			close(clnt_sock);
		}
	}
	
	close(serv_sock);
	return EXIT_SUCCESS;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("Remove proc id: %d \n", pid);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}
