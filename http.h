#ifndef HTTP_H
#define HTTP_H

#define SIZE 4096

//声明初始化服务器的函数
int init_server(int port);

//声明消息处理函数，用于处理客户端发来的消息
int handler_msg(int sock);




#endif