/*************************************************************************
    > File Name: thread_hello_world.c
    > Author:
    > Mail:  
    > Created Time: 2013年12月14日 星期六 11时48分50秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

void print_message_function(void *ptr);

int main()
{
    int tmp1, tmp2;
    void *retval;
    pthread_t thread1, thread2;
    char *message1 = "thread1";
    //char *message2 = "thread2";

    int ret_thrd1, ret_thrd2;

    ret_thrd1 = pthread_create(&thread1, NULL, (void *)&print_message_function, (void *)message1);
    //ret_thrd2 = pthread_create(&thread2, NULL, (void *)&print_message_function, (void *)message2);

    // 线程创建成功，返回0,失败返回失败号
    if (ret_thrd1 != 0)
    {
        printf("thread1 failed\n");
    }
    else
    {
        printf("thread1 success\n");
    }

    // if (ret_thrd2 != 0)
    // {
    //     printf("thread2 failed\n");
    // }
    // else
    // {
    //     printf("thread2 success\n");
    // }

    //同样，pthread_join的返回值成功为0
    // tmp1 = pthread_join(thread1, &retval);
    // printf("thread1 return value(retval) is %d\n", (int)retval);
    // printf("thread1 return value(tmp) is %d\n", tmp1);
    // if (tmp1 != 0)
    // {
    //     printf("cannot join with thread1\n");
    // }
    // printf("thread1 end\n");

    // tmp2 = pthread_join(thread1, &retval);
    // printf("thread2 return value(retval) is %d\n", (int)retval);
    // printf("thread2 return value(tmp) is %d\n", tmp1);
    // if (tmp2 != 0)
    // {
    //     printf("cannot join with thread2\n");
    // }
    // printf("thread2 end\n");
}

void print_message_function(void *ptr)
{
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));           //每个字节都用0填充
    serv_addr.sin_family = AF_INET;                     //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //具体的IP地址
    serv_addr.sin_port = htons(1234);                   //端口
    char bufSend[100] ={0};
    char buffer[40] = {0};
    strncpy(bufSend,(char*)ptr,sizeof(bufSend));
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    printf("wait for connect\n");
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("connected\n waiting for input");
    //printf("Input a string: ?");
    write(sock, bufSend, sizeof(bufSend));
    //读取服务器传回的数据
    //read(sock, buffer, 39);
    //printf("Message form server: %s\n", buffer);
    //memset(bufSend, 0, 100);
    //memset(buffer, 0, 40);
    //关闭套接字
    close(sock);
}
