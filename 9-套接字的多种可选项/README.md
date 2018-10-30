# 套接字的多种可选项

## getsockopt & setsockopt

`getsockopt()`:

```c
#include <sys/socket.h>
int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optnlen);

返回：
	成功：0
	失败：-1
```

- sock: