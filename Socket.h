//
// Created by yang on 16-8-9.
//
/*  封装socket*/
#ifndef TSHH_SOCKET_H
#define TSHH_SOCKET_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
class Socket {
public:
       ~Socket();
       Socket(sa_family_t family=AF_INET,int listen_num=100);
       int listen(int listen_num);
       int setnonblocking();   /*文件设置非阻塞*/
       bool bindAddressPort(); /*绑定IP和端口*/
       bool setReuseAddr();    /*设置地址重用*/
       bool setReusePort();    /*设置端口重用*/
       bool setKeepAlive(); /*设置保活机制*/
       int  return_fd();
private:
    int  sockfd;  /*网络套接字文件描述符*/
    int backlog;  /*监听的客户端的最大文件描述符数*/
    struct  sockaddr_in serveraddress;
    struct  sockaddr_in clientaddr;
};


#endif //TSHH_SOCKET_H
