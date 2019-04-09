#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define BUF_SIZE 100

int main(void)
{
	int fds[2];
	char str[] = "HELLO!";
	char buf[BUF_SIZE];
		
	if(pipe(fds) == -1) {
		perror("pipe() error");
		exit(EXIT_FAILURE);
	}

	switch(fork()) {
	case -1:
			perror("fork() error");
			break;
	case 0:
			//子进程执行区域
			//子进程仅写入数据
			close(fds[0]);						//关闭复制来的无用的管道出口文件描述符
			write(fds[1], str, sizeof(str));	//向管道入口写入数据
			close(fds[1]);						//不再使用管道
			exit(EXIT_SUCCESS);
	default:
			//父进程执行区域
			//父进程仅读取数据
			close(fds[1]);								//关闭无用的管道入口文件描述符
			read(fds[0], buf, BUF_SIZE);				//从管道出口读取数据
			printf("Message from child: %s\n", buf);	
			close(fds[0]);								//不再使用
	}

	return 0;
}
