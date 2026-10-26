#include<myhead.h>
#include"http.h"
#include"custom_handle.h"

//定义线程体函数
void* msg_request(void* arg)
{
    //解析传进来的sock
    int sock=*(int*)arg;
    delete (int*)arg;

    //调用信息处理函数，该函数是所有请求的入口
    handler_msg(sock);
    close(sock);
    return NULL;
}


int main(int argc,const char* argv[])
{
    //确认端口号
    int port=80;

    if(argc>1)
    {
        port=atoi(argv[1]);
    }


    //初始化监听服务器
    int lis_socket=init_server(port);


    //循环接收客户端的连接请求
    //多线程实现并发服务器
    while(1)
    {
        //声明一个接受客户端地址信息的地址信息结构体
        struct sockaddr_in peer;
        socklen_t socklen=sizeof(peer);


        //套接字sock接收一个新的客户端连接请求
        //sock：与客户端通信的套接字
        int sock=accept(lis_socket,(struct sockaddr*)&peer,&socklen);
        if(sock==-1)
        {
            perror("accept error");
            return -1;
        }


        //创建分支线程处理客户端请求
        pthread_t tid=-1;
        if(pthread_create(&tid,NULL,msg_request,new int(sock))>0)//改成new int(sock)
        {
            printf("pthread_create error");
            return -1;
        }
        printf("您有新的客户端(%d)发来了连接请求\n",sock);


        //将线程设置为分离态
        pthread_detach(tid);

        
    }


    //关闭服务器
    close(lis_socket);
    return 0;
}