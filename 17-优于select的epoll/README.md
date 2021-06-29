# 优于SELECT的EPOLL

在连接数较小的情况下，select具有不错的性能，跨平台兼容性也不错。但随着对单机性能提升的不断追求，select已经无法满足我们的需求。其暴露出的缺点也越来越不被人们所忍受：需循环查询所有监视对象、无法动态添加新的监视对象，需重新调用select并添加新目标等等。而Linux平台上的对select进行改进的epoll实现了：无需编写以监视状态变化为目的的针对所有文件描述符的循环语句、以及无需每次传递监视对象信息，可监视的描述符数量也几乎没有了限制。

`epoll api`:

```c
#include <sys/epoll.h>
// 创建epoll instance
int epoll_create(int size);
// 将感兴趣的fd注册进epoll实例的interest list中
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
// 阻塞等待当前线程上的IO事件
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout);

typedef union epoll_data {
	void    *ptr;
	int      fd;
	uint32_t u32;
	uint64_t u64;
} epoll_data_t;

struct epoll_event {
	uint32_t     events;    /* Epoll events */
	epoll_data_t data;      /* User data variable */
};

```

## LT 与 ET

LT（水平触发）是默认的模式，只要输入缓冲中有数据可读，就会注册事件并进行通知，而ET（边缘触发）则只在文件描述符上有I/O事件时，才进行通知，用户必须自己对数据进行有效的处理，该模式能减少不必要的重复通知从而提高效率，一般与non-blocking配合食用（避免未读取完全而造成长时间的阻塞 -- 除非fd上有新事件）。

### 设置非阻塞模式

```c
#include <fcntl.h>
int flag = fcntl(fd, F_GETFL, 0);
fcntl(fd, F_SETFL, flag|O_NONBLOCK);
```
