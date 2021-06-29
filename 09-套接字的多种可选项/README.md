# 套接字的多种可选项

![套接字选项汇总.png](https://img30.360buyimg.com/pop/jfs/t1/182802/21/11562/1119647/60d93d1cEec5af9de/4966fa6304c2bb6a.png)

## getsockopt & setsockopt

`getsockopt()`:

```c
#include <sys/socket.h>
int getsockopt(int socket, int level, int option_name,
               void *restrict option_value, socklen_t *restrict option_len);

返回：
	成功：0
	失败：-1
```

- socket：用于查看选项套接字文件描述符
- level：要查看的可选项的协议层
- option_name：要查看的可选项名
- option_value：保存查看结果的缓冲地址值
- option_len：向第四个参数`option_value`传递的缓冲大小。调用函数后，该变量中保存通过第四个参数返回的可选项信息的字节数

`setsockopt()`

```c
#include <sys/socket.h>
int setsockopt(int socket, int level, int option_name,
               const void *option_value, socklen_t option_len);

返回：
    成功：0
    失败：-1
```

- 参数描述同上

## SO_SNDBUF & SO_RCVBUF

创建套接字时将同时生成I/O缓冲，而该缓存大小可由`SO_SNDBUF`与`SO_RCVBUF`可选项进行操作。

## SO_REUSEADDR

`time-wait`状态：在结束TCP连接时，会经过四次握手过程。但套接字在该过程后并非立即销毁，先断开连接的一端还要更早的经过一段时间的`time-wait`状态。这么设计的原因是为了确保四次握手中的最后一条`ACK`消息能够到达对端，避免对方反复重传自己的`FIN`消息。

由于`time-wait`状态的存在，若我们在服务端紧急停止后想要立即重启服务，可能会出现地址无法绑定（被占用）的情况，使用`SO_REUSEADDR`可选项则可将`time-wait`状态下的套接字端口重新分配给新的套接字。

## TCP_NODELAY

`Nagle`算法：TCP套接字默认使用该算法交换数据，因此会最大限度的对数据进行缓冲，直到收到`ACK`，这么做可以提高网络传输速率。但如大文件传输这样的场景下，关闭该算法会比较好。