# 理解网络编程和套接字

## 概要

网络编程(套接字编程)就是编写程序使两台联网的计算机互相交换数据，而套接字就是用来连接网络的工具。

## 重要函数

### 生成套接字

```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);

返回:
	成功：套接字文件描述符
	失败：-1
```

### 绑定地址

```c
#include <sys/socket.h>
int bind(int socket, const struct sockaddr *address, socklen_t address_len);

返回：
	成功：0
	失败：-1
```

### 监听连接

```c
#include <sys/socket.h>
int listen(int socket, int backlog);

返回：
	成功：0
	失败：-1
```

### 接受请求

```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

返回：
	成功：返回请求连接端的套接字文件描述符
	失败：-1
```

### 请求连接

```c
#include <sys/socket.h>
int connect(int socket, const struct sockaddr *address, socklen_t address_len);

返回：
	成功：0
	失败：-1
```



> Tips:在Linux上，可使用man命令去查询每一个函数或者头文件



## Linux文件操作

在Linux里，socket也被认为是文件的一种，故在网络传输过程中亦可使用文件I/O的相关函数。

### 文件描述符(文件句柄)

文件描述符是系统分配给文件或套接字的整数。

#### 系统文件描述符

| 文件描述符 | 对象                      |
| ---------- | ------------------------- |
| 0          | 标准输入：Standard Input  |
| 1          | 标准输出：Standard Output |
| 2          | 标准错误：Standard Error  |

### I/O函数

#### 打开文件

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *path, int flag);

返回：
	成功：返回文件描述符
	失败：-1
```

文件打开模式(flag)：

| 打开模式 | 含义               |
| -------- | ------------------ |
| O_CREAT  | 必要时创建文件     |
| O_TRUNC  | 删除全部现有数据   |
| O_APPEND | 追加到已有数据后面 |
| O_RDONLY | 只读打开           |
| O_WRONLY | 只写打开           |
| O_RDWR   | 读写打开           |

#### 关闭文件

使用文件后必须关闭

```c
#include <unistd.h>
int close(int fd);

返回：
	成功：0
	失败：-1
```

#### 写文件

```c
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t nbytes);

返回：
	成功：返回写入的字节数
	失败：-1
```

#### 读文件

```c
#include <unistd.h>
int read(int fd, void *buf, size_t nbytes);

返回：
	成功：返回接收的字节数，遇到文件尾(EOF)则返回0
	失败：-1
```











































