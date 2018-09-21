# 地址族与数据序列

## 概要

IP地址分两类：IPv4（4字节）IPv6（16字节）

IPv4地址族:

```
   +--------+--------+--------+--------+
A: |0aaaaaaa|bbbbbbbb|bbbbbbbb|bbbbbbbb|  首字节:00000000~01111111=0~127
   +--------+--------+--------+--------+

   +--------+--------+--------+--------+
B: |10aaaaaa|aaaaaaaa|bbbbbbbb|bbbbbbbb|  首字节:10000000~10111111=128~191
   +--------+--------+--------+--------+

   +--------+--------+--------+--------+
C: |110aaaaa|aaaaaaaa|aaaaaaaa|bbbbbbbb|  首字节:1100000~,11011111=192~223
   +--------+--------+--------+--------+
   
以上为常用的3种类别的IP地址，其中，包含有a的部分和称为网络号，b的部分则为主机号。网络号字段中的0,10,110为类别位(即A,B,C类地址)
在A类地址中，0段与127段是不使用的(0段为保留地址，表示本网络；而127段为环回地址)，故A类地址范围为:1.0.0.0 ~ 126.255.255.255
B类地址的范围为:128.0.0.0 ~ 191.255.255.255
C类地址的范围为:192.0.0.0 ~ 223.255.255.255
```

## 地址信息表示

### 表示IPv4的结构体

```c
struct sockaddr_in
{
    sa_family		sin_family;		//地址族(AF_INET|AF_INET6|...)
    uint16_t		sin_port;		//16位端口号
    struct in_addr	sin_addr;		//32位IP地址
    char		sin_zero[8];		//占位用(必须填充为0)
}

struct in_addr
{
    In_addr_t	s_addr;				//32位IPv4地址
}
```

### 网络字节序与地址转换

#### 字节序

大端序：高位字节存放到地位地址

小端序：高位字节存放到高位地址

```
对于0x12345678, 0x12为高位字节，0x78为低位字节。
其大端序：
      +----+
      |0x78|
0x03: +----+
      |0x56|
0x02: +----+
      |0x34|
0x01: +----+
      |0x12|
0x00: +----+

其小端序：
      +----+
      |0x12|
0x03: +----+
      |0x34|
0x02: +----+
      |0x56|
0x01: +----+
      |0x78|
0x00: +----+


```

网络字节序使用大端序

#### 字节序转换

```c
unsigned short htons(unsigned short);	//主机字节序转网络字节序（转换端口）
unsigned short ntohs(unsigned short);	//网络字节序转主机字节序（转换端口）
unsigned long htonl(unsigned long);	//主机字节序转网络字节序（转换IP）
unsigned long ntohl(unsigned long);	//网络字节序转主机字节序（转换IP）
```

### 网络地址初始化及分配

#### 字符串转网络字节序

```c
#include <arpa/inet.h>
int_addr_t inet_addr(const char *string);

返回：
	成功：返回32位大端序整型数值
	失败：INADDR_NONE
```

```c
#include <arpa/inet.h>
int inet_aton(const char *string, struct in_addr *addr);

返回：
	成功：1，并将结果保存在addr里
	失败：0
```

#### 网络字节序转字符串

```c
#include <arpa/inet.h>
char *inet_ntoa(struct in_addr __in);

返回：
	成功：返回转换的字符串地址
	失败：-1
```

#### INADDR_ANY

可利用此常数来自动分配服务器端的IP地址（适用于单网卡情况）