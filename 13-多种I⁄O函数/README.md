# 多种I/O函数

## Linux 的 send & recv

`send`:

```c
#include <sys/socket.h>
ssize_t send(int socket, const void *buffer, size_t length, int flags);

返回：
    成功：返回发送的字节数
    失败：-1
```

- socket：与数据传输对象的链接的套接字文件描述符
- buffer：保存待传输数据的缓冲池地址
- length：待传输的字节数
- flags：传输数据时指定的可选项

`recv`：

```c
#include <sys/socket.h>
ssize_t recv(int socket, void *buffer, size_t length, int flags);

返回：
    成功：接收的字节数（收到EOF则返回0）
    失败：-1
```

- 参数大致同上

可选项：

|  Option Name  |                        含义                        | send | recv |
| :-----------: | :------------------------------------------------: | :--: | :--: |
|    MSG_OOB    |        用于传输带外数据（Out-of-band data）        |  *   |  *   |
|   MSG_PEEK    |          验证输入缓存中是否存在接收的数据          |      |  *   |
| MSG_DONTROUTE | 数据传输过程中不参照路由表，在本地网络中寻找目的地 |  *   |      |
| MSG_DONTWAIT  |   调用I/O函数时不阻塞，用于使用Non-blocking I/O    |  *   |  *   |
|  MSG_WAITALL  |       阻止函数返回，知道接收全部请求的字节数       |      |  *   |

这里的OOB含义为：通过完全不同的通信路径进行传输的数据，真正的OOB需要单独的通信路径来高速传输数据，但TCP并不提供，只利用TCP的紧急模式来进行传输。

同时设置`MSG_PEEK` 与 `MSG_DONTWAIT`选项，可以用于验证输入缓存中是否存在需要接收的数据（不用真正读取）。

## readv & writev

通过一个`iovec`，其内部存储了多个缓冲池（数组）的地址，配合`v`型函数可对数据进行整合传输及发送，提高数据通信效率。

`readv`:

```c
#include <sys/uio.h>
ssize_t readv(int fildes, const struct iovec *iov, int iovcnt);

返回：
    成功：读取到的字节数
    失败：-1
```

- fildes：套接字文件描述符，当然也可以是文件或标准输出等描述符
- iov：iovec结构体数组的地址值，该结构体中包含了待发送数据的位置和大小信息
- iovcnt：向第二个参数传递的数组长度

`writev`:

```
#include <sys/uio.h>
ssize_t writev(int fildes, const struct iovec *iov, int iovcnt);

返回：
    成功：发送的字节数
    失败：-1
```

`iovec`结构体：

```c
struct iovec
{
    void * iov_base; // 缓冲地址
    size_t iov_len;  // 缓冲大小
}
```

```c
// example
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
...
ssize_t bytes_written;
int fd;
char *buf0 = "short string\n";
char *buf1 = "This is a longer string\n";
char *buf2 = "This is the longest string in this example\n";
int iovcnt;
struct iovec iov[3];
iov[0].iov_base = buf0;
iov[0].iov_len = strlen(buf0);
iov[1].iov_base = buf1;
iov[1].iov_len = strlen(buf1);
iov[2].iov_base = buf2;
iov[2].iov_len = strlen(buf2);
...
iovcnt = sizeof(iov) / sizeof(struct iovec);
bytes_written = writev(fd, iov, iovcnt);
...
```
