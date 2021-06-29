#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 3

int main(int argc, char **argv) {
    int fd1, fd2;
    int len;
    char buf[BUF_SIZE];

    // dd if=/dev/urandom of=300MB.test bs=1M count=300 iflag=fullblock
    // 简单用time命令测试一下运行时间
    fd1 = open("300MB.test", O_RDONLY);
    fd2 = open("300MB.cpy", O_WRONLY|O_CREAT|O_TRUNC);
    
    while((len = read(fd1, buf, sizeof(buf))) > 0) {
        write(fd2, buf, len);
    }

    close(fd1);
    close(fd2);
    return 0;
}
