# 多线程服务器端的实现

多进程模型的缺点：

- 创建开销大
- 为完成进程间数据交换，引入IPC技术
- 进程上下文切换相对较慢

与之对比，线程：

- 线程的创建及上下文切换较进程更快
- 线程间交换数据无需特殊技术

多个线程之间可以共享父进程的数据区与堆。

## 线程创建及运行

`fork-join模型`

```c
// Compile and link with -pthread.
#include <pthread.h>
int pthread_create(pthread_t *restrict thread,
					const pthread_attr_t *restrict attr,
					void *(*start_routine)(void *),
					void *restrict arg);

返回：
    成功：0
    失败：非0值
```

- thread：保存新创建线程ID的变量地址值
- attr：传递线程属性的参数，传递NULL则表示默认
- start_routine：函数指针，用于在线程中执行
- arg：传递上面参数函数指针运行所需的参数等

```c
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);

返回：
    成功：0
    失败：非0值
```

- 该参数值ID的线程终止后才会从该函数返回
- 保存线程start_routine函数返回值的指针变量地址值

编译时加`-D_REENTRANT`可自动将一些非线程安全的函数调用替换为其线程安全的版本：

如`gethostbyname()` -> `gethostbyname_r()`

## 线程同步

从两个方面来考虑同步的情况：

- 同时访问同一内存空间时发生的情况 （使用互斥锁）
- 需要指定访问同一内存空间的线程执行顺序的情况 （使用信号量）

### 互斥量

```c
#include <pthread.h>
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
						const pthread_mutexattr_t *restrict attr);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

### 信号量

```c
#include <semaphore.h>
int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);
int sem_post(sem_t *sem);
int sem_wait(sem_t *sem);
```

`sem`传递保存信号量读取值的变量地址值，传递给`sem_post`时信号量+1,传递给`sem_wait`时信号量-1
