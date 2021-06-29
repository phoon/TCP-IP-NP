#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char **argv) {
    int acpt_sock, recv_sock;
    struct sockaddr_in recv_adr, acpt_adr;
    int str_len, state;
    socklen_t recv_adr_sz;
    char buf[BUF_SIZE];

    if(argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1) {
       error_handling("bind() error!");
    }
    listen(acpt_sock, 5);

    recv_adr_sz = sizeof(recv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_adr, &recv_adr_sz);

    while(1) {
        str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_PEEK | MSG_DONTWAIT);
        if(str_len > 0)  {
            break;
        }
    }

    buf[str_len] = 0;
    printf("Buffering %d bytes: %s \n", str_len, buf);

    str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_PEEK | MSG_DONTWAIT);
    buf[str_len] = 0;
    printf("Read again: %s \n", buf);

    close(recv_sock);
    close(acpt_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
