#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int status;
	pid_t pid = fork();

	if(pid == 0) {
		sleep(15);
		return 24;
	}else {
		/*第一个参数-1表示可等待任意子进程终止*/
		while(!waitpid(-1, &status, WNOHANG)) {
			sleep(3);
			puts("sleep 3 sec.");
		}
		if(WIFEXITED(status)) {
			printf("Child send %d\n", WEXITSTATUS(status));
		}
	}
	return 0;
}