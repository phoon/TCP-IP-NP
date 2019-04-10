#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(void)
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	FD_ZERO(&reads);
	FD_SET(0, &reads);	//0 is standard input(console)
	
	//不应在此处设置超时，在调用select()之后，
	//结构体timeval的两个成员会被替换为超时前剩余时间，
	//应在调用select前，都初始化一次超时。
	/*
	timeout.tv_sec = 5;
    timeout.tv_usec = 5000;	
	*/

	while(1) {
		//记住初始值
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		
		result = select(1, &temps, 0, 0, &timeout);
		if(result == -1) {
			puts("select() error");
			break;
		} else if(result == 0) {
			puts("Time-out!");
		} else {
			if(FD_ISSET(0, &temps)) {
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console: %s\n", buf);
			}
		}
	}

	return 0;
}
