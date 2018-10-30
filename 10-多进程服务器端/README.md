# 多进程服务器端

 并发服务器端实现模型和方法：

- 多进程服务器：通过创建多个进程提供服务（Windows不支持）
- 多路复用服务器：通过捆绑并统一管理I/O对象提供服务
- 多线程服务器：通过生成与客户端等量的线程提供服务

## 进程

进程的定义为：“占用内存的正在运行的程序“，进程同时也是操作系统进行资源分配的一个基本单位

> 处理器核心数与可同时运行的进程数相等，若进程数 > 核心数，处理器将进行分时处理

### 通过fork函数创建进程

```c
#include <unistd.h>
pid_t fork(void);

返回：
	成功：0
	失败：-1
```

fork函数创建的子进程将会获得其父进程（fork函数调用者）的数据段、堆和栈的副本，并与其父进程共享代码段

区分父进程与子进程：通过fork函数的返回值

- 父进程：fork函数返回子进程pid
- 子进程：fork函数返回0

### 进程的状态

- 可运行状态（TASK_RUNNING，简称R）
- 可中断的睡眠状态（TASK_INTERRUPTIBLE，简称S）
- 不可中断的睡眠状态（TASK_UNINTERRUPTIBLE，简称D）
- 暂停或追踪状态（TASK_STOPPED/TASK_TRACED，简称T）
- 僵尸状态（TASK_DEAD-EXIT_ZOMBIE，简称Z）
- 退出状态（TASK_DEAD-EXIT_DEAD，简称X）

## 僵尸进程

进程在完成工作后应被销毁，但有时这些进程会进入僵尸状态，占用系统的资源，成为僵尸进程。

### 僵尸进程产生原因

给进程设置僵尸状态的目的是维护子进程的信息，以便父进程在以后某个时间获取。这些信息包括子进程的进程ID、终止状态以及资源利用信息(CPU时间，内存使用量等等)。如果一个进程终止，而该进程有子进程处于僵尸状态，那么它的所有僵尸子进程的父进程ID将被重置为1（init进程）。继承这些子进程的init进程将清理它们(init进程将wait它们，从而去除僵尸状态)。

### 避免僵尸进程

避免产生僵尸进程方法：

- fork()两次并杀死其父进程，使其变成孤儿进程，从而移交init处理
- 在fork()后调用wait()/waitpid()函数主动取得子进程的退出状态

#### wait()

```c
#include <sys/wait.h>
pid_t wait(int *statloc);

返回：
	成功：返回终止的子进程pid
	失败：-1
```

调用此函数时如果已有子进程终止，那么子进程终止时传递的返回值将保存在该函数参数所指内存空间，但其中还包含其他信息，需要使用以下宏进行分离：

- WIFEXITED 子进程正常终止时返回true
- WEXITSTSTUS 返回子进程的返回值

宏使用：在调用wait()函数后，编写如下代码

```c
if(WIFEXITED(status)) { //是否是正常终止
    puts("Normal termination!");
    printf("Child pass num: %d", WEXITSTATUS(status));	//子进程返回值
}
```

> 调用wait()函数时，如果没有已终止的子进程，那么程序将阻塞直至有子进程终止

#### waitpid()

```c
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *statloc, int options);

返回：
	成功：返回终止的子进程pid（或0）
	失败：-1
```

- pid：等待终止的目标子进程pid，若传递-1,则与wait()函数相同，可等待任意子进程终止
- statloc：保存子进程的返回值
- options：传递头文件`sys/wait.h`中声明的常量`WNOHANG`，即使无终止的子进程也不会进入阻塞状态，而是返回0并退出函数

## 信号处理

父进程无法知道子进程的具体终止时间，因此wait()/waitpid()并不是一个很好的解决方式。而如果当在子进程退出时，向操作系统发送信号，再由操作系统提示父进程处理子进程相关事宜则不失为一个合理实用的方式

### 信号与signal函数

信号注册函数：

```c
#include <signal.h>
void (*signal(int signo, void (*func)(int)))(int);

返回：返回函数指针
```

可在signal函数中注册的部分特殊情况和对应的常数：

- SIGALRM：已到通过调用`alarm`函数注册的时间
- SIGINT：输入CTRL+C
- SIGCHLD：子进程终止信号

信号处理函数应有一个`int`参数，且返回值为`void`

alarm函数：

```c
#include <unistd.h>
unsigned int alarm(unsigned int seconds);

返回：返回0或者以秒为单位的距SIGALRM信号发生所剩时间
```

### sigaction函数

```c
#include <signal.h>
int aigaction(int signo, const struct sigaction *act, struct sigaction *oldact);

返回：
	成功：0
	失败：-1
```

- signo：信号信息
- act：信号处理函数信息
- oldact：获取之前注册的信号处理函数指针，不需要则传0

sigaction结构体如下：

```c
struct sigaction
{
    void (*sa_handler)(int);
    sigset_t sa_mask;
    int sa_flags;
}
```