# 基于UDP的服务端/客户端

 ## 理解UDP

UDP在结构上比TCP简洁，在更注重性能而非可靠性的情况下，UDP是一种很好的选择。

>流控制是区分UDP与TCP最重要的标志。UDP也并非总是快于TCP，TCP比UDP慢的原因通常为以下两点:
>
>- 收发数据前后进行的连接设置及清除过程
>- 收发数据过程中为保证可靠性而添加的流控制
>
>如果收发的数据量小但需要频繁连接，UDP比TCP高效

### 1.UDP中服务端与客户端间没有连接：

UDP中只有创建套接字的过程和数据交换的过程

UDP不同于TCP,不存在请求连接和受理过程，因此在某种意义上无法明确区分服务器端与客户端

### 2.UDP服务端与客户端均只需一个套接字

只需一个UDP套接字即可向任意主机传送数据

## 基于UDP的数据I/O函数

### 传输数据

```c
#include <sys/socket.h>
ssize_t sendto(int sock, void *buff, size_t nbytes,
           int flags, struct sockaddr *to,socklen_t addrlen);

返回：
	成功：返回传输的字节数
	失败：-1
```

- sock：用于传输数据的UDP套接字文件描述符
- buff：待传输数据的缓冲地址值
- nbytes：待传输数据长度        
- flags：可选参数，若无则为0
- to：存有目标地址信息的sockaddr结构体变量的地址值
- addrlen：to变量的长度

### 接收数据

```c
#include <sys/socket.h>
ssize_t recvfrom(int sock, void *buff, size_t nbytes,
                int flags, struct sockaddr *from, socklen_t *addrlen);

返回:
	成功：返回接收的字节数
	失败：-1
```

- sock：用于接收数据的UDP套接字文件描述符
- buff：保存接收数据的缓冲地址值
- nbytes：可接收最大字节数，故无法超过buff所设缓冲大小        
- flags：可选参数，若无则为0
- from：存有发送端地址信息的sockaddr结构体变量的地址值
- addrlen：from变量的长度

## UDP的数据传输特性和调用connect函数

### UDP套接字存在数据边界

对于UDP，传输中的I/O函数的调用次数非常重要。输入函数与输出函数的调用次数应完全一致，才能保证全部接收已发数据。

### 已连接UDP套接字与未连接UDP套接字

UDP中无需注册待传输数据的目标IP和端口号，通过`sendto()`传输数据的过程大致可分为以下3个阶段：

- 第一阶段：向UDP套接字注册目标IP和端口号
- 第二阶段：传输数据
- 第三阶段：删除UDP套接字中注册的目标地址信息

每次调用`sendto()`会重复以上过程，每次都改变目标地址，因此可重复利用同一UDP套接字向不同目标传输数据。

未连接套接字：未注册目标地址信息的套接字（UDP默认）

已连接套接字：注册了目标地址信息的套接字（使用`connect()`，`connect()`作用只是为了注册目标地址信息），在需要长时间通信时，使用已连接UDP套接字更高效

#### 创建已连接套接字

```c
sock = socket(PF_INET, SOCK_DGRAM, 0);
memset(&adr, 0, sizeof(adr));
adr.sin_family = AF_INET;
adr.sin_addr.s_addr = ....;
adr.sin_port = ....;
connect(sock, (struct sockaddr*)&adr, sizeof(adr));
```