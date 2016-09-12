//
// Created by yang on 16-8-9.
//

#include <fcntl.h>
#include "Socket.h"
Socket::Socket(sa_family_t family, int listen_num) {
    backlog=listen_num;
    sockfd=socket(family,SOCK_STREAM,0);
    if(sockfd < 0){
        std::cout << "创建sockfd失败(ps:应该写在日志中)"<<std::endl;
    }
    setReusePort();
    setReuseAddr();
    setKeepAlive();
    memset(&serveraddress,0,sizeof(serveraddress));
    serveraddress.sin_family=family;
    serveraddress.sin_addr.s_addr=htonl(INADDR_ANY);
    serveraddress.sin_port=htons(8888);
    if(bindAddressPort()){
            if(listen(listen_num)){
              /*  if(setnonblocking()){
                    return;
                }*/
                return ;
            }
    }
}
Socket::~Socket() {
    ::close(sockfd);  /*调用的是系统调用的close*/
}
/**
 * 设置套接字文件描述符为非阻塞
 * @return
 */
int
Socket::setnonblocking() {
    int old_option=fcntl(sockfd,F_GETFL);
    int new_option=old_option | O_NONBLOCK;
    fcntl(sockfd,F_SETFL,new_option);
    return  old_option;
}
int
Socket::listen(int listen_num) { /*设置监听*/
    int ret;
    ret=::listen(sockfd,listen_num);
    if(ret < 0){
        std::cout <<"listen error(ps:错误日志)"<<std::endl;
        return false;
    }
    return true;
}
/**
 * 设置端口重用
 * @return
 */
bool
Socket::setReusePort() {
    int optval=1;
    ::setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,&optval,sizeof(optval));
    return true;
}
/**
 * 设置地址重用
 * @return
 */
bool
Socket::setReuseAddr() {
    int optval=1;
    ::setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
    return true;
}
/**
 * 类似于心跳包.
 * @return
 */
bool
Socket::setKeepAlive() {
    int optval=1;
    ::setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,&optval,sizeof(optval));
    return  true;

}
bool
Socket::bindAddressPort() {
    int ret=bind(sockfd,(struct sockaddr *)&serveraddress,sizeof(serveraddress));
    if(ret<0){
        std::cout <<"bind error(ps::日志)"<<std::endl;
        return false;
    }
    return true;
}
int
Socket::return_fd() {
    return sockfd;
}
