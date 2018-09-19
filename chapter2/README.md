# 套接字类型与协议设置

## 概要

协议就是为了完成数据交换而定好的约定

## 协议族（Protocol Family）

| 名称      | 协议族               |
| --------- | -------------------- |
| PF_INET   | IPv4互联网协议族     |
| PF_INET6  | IPv6互联网协议族     |
| PF_LOCAL  | 本地通信的UNIX协议族 |
| PF_PACKET | 底层套接字的协议族   |
| PF_IPX    | IPX Novell协议族     |

来看函数`int socket(int domain, int type, int protocol);`，参数domain设置套接字使用的协议族信息，type设置套接字数据传输类型信息，protocol设置计算机通信使用的协议族。另外，套接字中实际采用的最终协议信息通过protocol参数传递，且其由参数domain决定

## 套接字类型（Type）

### 面向连接的套接字（SOCK_STREAM）

- 传输数据过程中数据不会消失

- 按序传输数据

- 传输的数据不存在数据边界（Boundary）

### 面向消息的套接字（SOCK_DGRAM）

- 传输的数据可能会消失或销毁
- 强调传输速度而非传输顺序
- 传输的数据存在边界
- 限制每次传输数据的大小


