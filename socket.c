#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

const int Port = 8888;

int main(void){
    int sock_fd;
    struct sockaddr_in mysock;
    struct sockaddr_in client_addr;
    int new_fd;
    socklen_t sin_size
    char buf[1024];
    char sedbuf[1024] = "recv successfully.\n";

    //初始化socket
    sock_fd = socket(AF_INET,SOCK_STREAM,0);

    //编辑地址信息
    memset(&mysock,0,sizeof(mysock));
    mysock.sin_family = AF_INET;
    mysock.sin_port = htons(Port);
    mysock.sin_addr.s_addr = INADDR_ANY;

    //绑定地址，然后监听
    bind(sock_fd,(struct sockaddr *)&mysock,sizeof(struct sockaddr));
    if(listen(sock_fd,10) < -1){
        printf("listen error.\n");
    }

    sin_size = sizeof(struct sockaddr_in);
    
    printf("listening...\n");
    new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &sin_size);//accpet
    while(1){
        int len = recv(new_fd,buf,sizeof(buf),0);
        fputs(buf,stdout);
        send(new_fd, sedbuf, sizeof(sedbuf), 0);
        if(strcmp(buf,"exit\n") == 0){
            break;
        }
        memset(buf,0,sizeof(buf));
    }
    close(new_fd);
    close(sock_fd);
    return 0;