#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char *message);

int main(void)
{
	int fd;
	char buf[] = "Let's go!\n";
	//以只写模式打开data.txt并清空其内容，且必要时创建该文件
	fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC);
	if(fd == -1) {
		error_handling("open() error!");
	}
	printf("The file descriptor is: %d \n", fd);

	if(write(fd, buf, sizeof(buf)) == -1) {
		error_handling("write() error!");
	}

	close(fd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}