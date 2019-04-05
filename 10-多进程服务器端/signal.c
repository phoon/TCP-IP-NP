#include <stdio.h>
#include <unistd.h>
#include <signal.h>

//签名函数
void timeout(int sig)
{
	if(sig == SIGALRM) {
		puts("Time out!");
	}
	//generate a SIGALRM signal
	alarm(2);
}

//签名函数
void keycontrol(int sig)
{
	if(sig == SIGINT) {
		puts("CTRL+C pressed");
	}
}

int main(void)
{
	int i;
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);

	for(i = 0; i < 3; i++) {
		puts("wait...");
		sleep(100);
	}

	return 0;
}
