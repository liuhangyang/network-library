//
// Created by yang on 16-8-10.
//

#ifndef TSHH_EPOLL_H
#define TSHH_EPOLL_H

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include "ThreadPool.hpp"
class Epoll {
public:
    Epoll()= default;
    Epoll(int fd);
    ~Epoll();
    bool Epoll_loop();  /*epoll的wait函数*/
    bool  addFd(int fd, bool enable_et,bool oneshot);
    bool addListenFd(int fd,bool enable_et);
    int setNonBlocking(int fd);
    bool delFd(int fd);
    bool reset_one_shot(int fd);
    bool setEpollout(int fd);
    void startPool();

private:
   int epoll_fd;
    epoll_event events[100];
    int sockfd;
    int EPOLLEVENTS=100;  //默认是1000,
     ThreadPool pool;
};


#endif //TSHH_EPOLL_H
