# 域名及网络地址

## IP地址与域名之间的转换

IP地址发生更改的几率比域名发生更改的几率高，因此在程序中使用域名不失为一个好的方法

### 利用域名获取IP地址

```c
#include <netdb.h>
struct hostent * gethostbyname(const char *hostname);

返回：
	成功：返回hostent结构体地址
	失败：返回NULL指针
```

hostent结构体:

```c
struct hostent
{
    char * h_name;	//官方域名
    char ** h_aliases;	//别名列表
    int h_addrtype;	//IP协议类型
    int h_length;	//IP地址长度
    char ** h_addr_list;	//域名对应IP地址列表
}
```

### 利用IP地址获取域名

```c
#include <netdb.h>
struct hostent * gethostbyaddr(const char *addr, socklen_t len, int family);

返回：
	成功：返回hostent结构体地址
	失败：返回NULL指针
```

- addr：in_addr 结构体指针
- len：addr的字节数，IPv4为4，IPv6为16
- family：地址族信息，IPv4为AF_INET，IPv6为AF_INET6