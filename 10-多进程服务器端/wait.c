#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int status;
	pid_t pid = fork();
	
	if(pid == 0) {
		//子进程返回3
		return 3;
	}else {
		printf("Child PID: %d\n", pid);
		//回到本进程，再次fork一个子进程
		pid = fork();
		if(pid == 0) {
			//新子进程返回7
			exit(7);
		}else {
			printf("Child PID: %d\n", pid);
			wait(&status);
			//分离信息
			if(WIFEXITED(status)) {
				printf("Child send one: %d\n", WEXITSTATUS(status));
			}
			wait(&status);
			if(WIFEXITED(status)) {
				printf("Child send two: %d\n", WEXITSTATUS(status));
			}
			sleep(30);	//暂停主进程，以便查看子进程状态
		}
	}
	return 0;
}
