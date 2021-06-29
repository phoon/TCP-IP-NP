# 套接字和标准I/O

使用标准I/O函数的优点：

- 标准I/O函数具有良好的移植性
- 标准I/O函数可以利用缓冲提高性能

缺点：

- 不容易进行双向通信
- 由于缓冲的缘故，有时可能频繁调用`fflush`函数
- 需要以`FILE`结构体指针的形式返回文件描述符

## 使用标准I/O函数

```c
#include <stdio.h>
FILE *fdopen(int fd, const char *mode);

返回：
    成功：返回转换后的FILE结构体指针
    失败： NULL
```

```c
#include <stdio.h>
int fileno(FILE *stream);

返回：
    成功：返回转换后的文件描述符
    失败：-1
```