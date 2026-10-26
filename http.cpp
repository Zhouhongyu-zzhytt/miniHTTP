#include<myhead.h>
#include"http.h"
#include"custom_handle.h"

//================================get_line函数=========================================
int get_line(int sock,char* buf)
{
    int i=0;
    char ch='\0';
    int ret=0;
    
    while(i<SIZE&&ch!='\n')
    {
        int ret=recv(sock,&ch,1,0);
        if(ret>0&&ch=='\r')
        {
            int s=recv(sock,&ch,1,MSG_PEEK);
            if(s>0&&ch=='\n')
            {
                recv(sock,&ch,1,0);
            }
            else
            {
                ch='\n';
            }
        }

        buf[i]=ch;
        i++;
    }
    buf[i]='\0';
    return i;
    
}
//================================clear_header函数========================================
void clear_header(int sock)
{
    char buf[SIZE]="";
    int ret=0;
    do
    {
        ret=get_line(sock,buf);
    } while (ret!=1&&strcmp(buf,"\n")!=0);
    
    
}
//================================echo_error函数===========================================
void show_404(int sock)
{
    //将获取的消息头部全部清除掉，上面解析数据时，只解析了请求行
    clear_header(sock);

    //组装首部信息
    const char* msg = "HTTP/1.1 404 Not Found\r\n";
    //发送首部
    send(sock,msg,strlen(msg),0);
    send(sock,"\r\n",strlen("\r\n"),0);

    //打开一个html文件
    struct stat st;
    stat("../wwwroot/404.html",&st);
    int fd=open("../wwwroot/404.html",O_RDONLY);
    if(fd==-1)
    {
        perror("open 404 error");
        return;
    }

    //将整个文件发送出去
    //函数原型：ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
    //功能：将一个文件描述符中的数据发送到另一个文件描述符中
    //参数1：要被拷贝到的文件描述符
    //参数2：从哪个文件中拷贝
    //参数3：从该文件的哪个位置开始拷贝
    //参数4：拷贝的文件大小
    sendfile(sock,fd,NULL,st.st_size);
    close(fd);
}
void echo_error(int sock,int err_no)
{
    switch(errno)
    {
        case 403:
            break;
        case 404:
            show_404(sock);
            break;
        case 405:
            break;
        case 500:
            break;
        default:
            break;    
    }
}
//=================================echo_www函数===========================================
int echo_www(int sock,char* path,size_t s)
{
    //以只读的形式打开文件
    int fd = open(path, O_RDONLY);
    if(fd == -1)
    {
        perror("open error");
        return -1;
    }

    //组装响应首部
    const char *msg = "HTTP/1.1 200 OK\r\n";
    send(sock, msg, strlen(msg), 0);        //发送给客户端网页
    send(sock, "\r\n", strlen("\r\n"), 0);  //发送空行

    //将整个文件发送给网页端
    if(sendfile(sock, fd, NULL, s) ==-1)
    {
        echo_error(sock, 500);        //返回错误界面
        return -1;
    }

    close(fd);
    return 0;
}
//=================================handle_request函数====================================
int handle_request(int sock,const char* method,const char* path,const char* querry_string)
{
    char line[SIZE]="";//存储一行信息
    int ret=0;//接收读取的内容
    int content_len=-1;//文本长度

    if(strcasecmp(method,"GET")==0)
    {
        clear_header(sock);
    }
    else
    {
        //获取POST请求的参数大小
        do
        {
            ret=get_line(sock,line);
            if(strncasecmp(line,"content-length",14)==0)
            {
                content_len=atoi(line+16);
            }
        } while (ret!=1&&(strcmp(line,"\n"))!=0);
        
    }

    //输出相关数据
    printf("method = %s\n", method);
    printf("query_string = %s\n", querry_string);
    printf("content_len = %d\n", content_len);

    char req_buf[SIZE]="";
    if(strcasecmp(method,"POST")==0)
    {
        int len=recv(sock,req_buf,content_len,0);
        printf("len = %d\n", len);      
        printf("req_buf = %s\n", req_buf);
    }
    
    //发送给客户端消息
    const char *msg="HTTP/1.1 200 OK\r\n\r\n";
    send(sock,msg,strlen(msg),0);

    //请求交给自定义的函数来处理业务逻辑
    parse_and_process(sock,querry_string,req_buf);
}


//===================================定义初始化服务器函数================================
int init_server(int port)
{
    //创建服务器套接字sock
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1)
    {
        perror("socket error");
        return -1;
    }


    //端口号快速重用
    int reuse=1;
    if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))==-1)
    {
        perror("setsockopt error");
        return -1;
    }


    //绑定ip和端口号
    //填充地址信息结构体
    struct sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);
    sin.sin_addr.s_addr=INADDR_ANY;


    //bind
    if(bind(sock,(struct sockaddr*)&sin,sizeof(sin))==-1)
    {
        perror("bind error");
        return -1;
    }


    //listen
    if(listen(sock,128)==-1)
    {
        perror("listen error");
        return -1;
    }


    //程序执行至此，表示服务器创建成功，返回sock
    return sock;
}






//===========================定义处理客户端信息的函数==========================
int handler_msg(int sock)
{
    //仅查看客户端发来的数据，不读取
    char del_buf[SIZE];

    recv(sock,del_buf,sizeof(del_buf),MSG_PEEK);

#if 1
    printf("客户端收到请求信息：\n");
    printf("=============================\n");
    printf("%s\n",del_buf);
    printf("=============================\n");
#endif


    //解析http请求：


    //获取请求行
    char buf[SIZE]="";
    int count=get_line(sock,buf);

    //解析请求方法
    char method[32]="";
    int k=0;

    int i;
    for(i=0;i<count;i++)//遍历请求行的首部
    {
        if(isspace(buf[i]))
        {
            break;
        }
        else
        {
            method[k++]=buf[i];
        }
    }
    method[k]=0;//程序执行至此，method数组中就存储了请求方法

    while(isspace(buf[i])&& i<SIZE)
    {
        i++;
    }   ////程序执行至此，i就记录了buf中后面的第一个非空字符串

    //判断请求方法
    if(strcasecmp(method,"GET")!=0&&strcasecmp(method,"POST")!=0)
    {//说明该请求方法既不是GET请求也不是POST请求
        printf("method error\n");
        //echo_error(sock,405);
        close(sock);
        return -1;
    }

    //判断是否为POST请求，若是，则需手动操作
    int need_handle=0;
    if(strcasecmp(method,"POST")==0)
    {
        need_handle=1;
    }

    //拿出要处理的url以及发送的数据（如果有？的话）
    char url[SIZE];
    int t=0;
    char* querry_string=NULL;

    for(i;i<count;i++)
    {
        if(isspace(buf[i]))//表示usr读取结束
        {
            break;
        }

        if(buf[i]!='?')
        {
            url[t++]=buf[i];
        }
        else
        {
            url[t]='\0';
            querry_string=&url[i];
            querry_string++;
        }
    }
    url[t]='\0';

    printf("url=%s\n",url);
    printf("querry_string=%s\n",querry_string);
    //判断是否为附带数据的GET请求，若是，则需手动操作
    if(strcasecmp(method,"GET")==0&&querry_string!=NULL)
    {
        need_handle=1;
    }

    //把请求资源路径固定为wwwroot下的资源
    char path[SIZE]="";
    sprintf(path,"../wwwroot%s",url);

    //如果请求没有地址没有任何携带资源，那么默认返回index.html
    if(path[strlen(path)-1]=='/')
    {
        strcat(path,"index.html");
    }
    printf("path=%s\n",path);

    //判断当前服务器中是否有该path
    //如果指定的文件存在，则会把该文件的信息放入st结构体中，如果不存在，函数返回-1
    //如果不存在则响应404错误码
    struct stat st;
    if(stat(path,&st)==-1)
    {
        //说明该path不存在
        printf("can not find file\n");
        echo_error(sock,404);
        close(sock);
        return -1;
    }

    //程序执行至此，表示能够确定是否需要自己来处理后续逻辑了
    //如果是POST请求或者是携带数据的GET请求，都需要手动书写逻辑进行处理
    if(need_handle==1)
    {
        handle_request(sock,method,path,querry_string);
        //调用处理请求函数
        //参数1：套接字文件描述符
        //参数2：请求方法
        //参数3：请求的路径
        //参数4：请求附带的数据
    }
    else
    {
        //清除首部剩余内容
        clear_header(sock);//bug
        ////此时表示是GET请求,并且没有附加数据，则直接返回请求的界面即可
        echo_www(sock,path,st.st_size);
    }


    //表示所有请求都正常处理了
    close(sock);
    return 0;

}



