#include <stdio.h>
#include <unistd.h>

int main(void)
{
	//pid_t pid = fork(); 此行gcc8.2编译不通过
	__pid_t pid = fork();
	//子程序开始
	if(pid == 0) {
		puts("Hi,I am a child process.");
	} else {
		printf("Child Process ID: %d \n", pid);
		sleep(30);
	}

	if(pid == 0) {
		puts("End child process");
	} else {
		puts("End parent process");
	}

	return 0;
}
