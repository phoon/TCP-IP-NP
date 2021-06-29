#include<stdio.h>
#include<unistd.h>

int gval = 10;

int main(void)
{
	pid_t pid;
	int lval = 20;
	gval++; //gval = 11
	lval += 5; //lval = 25

	pid = fork();
	//子进程开始
	if(pid == 0) {
		gval += 2;
		lval += 2;
	} else {
		gval -= 2;
		lval -= 2;
	}

	if(pid == 0) {
		printf("Child Proc: [%d, %d] \n", gval, lval);
	} else {
		printf("Parent Proc: [%d, %d] \n", gval, lval);
	}

	return 0;
}
