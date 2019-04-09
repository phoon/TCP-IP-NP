#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc ,char *argv[])
{
	int fds[2];
	char str[] = "Who are you?";
	char buf[BUF_SIZE];

	pid_t pid;

	pipe(fds);
	//注意！fork之后，子进程复制的并非是管道，而是用于管道I/O的文件描述符
	pid = fork();
	if(pid == 0) {
		//子进程仅传输数据
		write(fds[1], str, sizeof(str));
	} else {
		//父进程仅读取数据
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}
	return 0;
}
