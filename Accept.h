//
// Created by yang on 16-8-10.
//

#ifndef TSHH_ACCEPT_H
#define TSHH_ACCEPT_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <sys/syscall.h>
class Accept {
public:
    Accept()= default;
    Accept(int fd);
    ~Accept()= default;
    pid_t gettid();
    int Accept_return();    /*返回connfd*/
    int Accept_write(std::string str);     /*执行写操作*/
    int Accept_read(char *str);      /*执行读操作*/
    std::string IPToString(unsigned ipv4);
private:
    int sockfd;
    int connfd;
    struct  sockaddr_in cliaddr;
};


#endif //TSHH_ACCEPT_H
