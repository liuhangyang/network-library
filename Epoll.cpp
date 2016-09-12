//
// Created by yang on 16-8-10.
//

#include <iostream>
#include "Epoll.h"
#include "Accept.h"
//#include "SyncQueue.hpp"
#include "Task.h"
#include "objectToJson.hpp"
/**
 *
 * @param fd 构造函数
 * @return
 */
Epoll::Epoll(int fd) {
    epoll_fd=epoll_create1(0);
    if(epoll_fd < 0){
        std::cout <<"epoll_create1 failed!"<<std::endl;
    }
    sockfd=fd;
    //events=new epoll_event[EPOLLEVENTS];
}
/**
 * 析构函数,析构前先释放events动态分配的数组.
 */
 Epoll::~Epoll(){
    //delete events;
}
/**
 *
 * @param fd
 * @param enable_et 是否设置边缘触发;
 * @param oneshot  是否设置EPOLLONESHOT
 * @return
 */
bool
Epoll::addFd(int fd,bool enable_et,bool oneshot){
    epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;
    if(enable_et){
        event.events |=EPOLLET;
    }
    if(oneshot){
        event.events |=EPOLLONESHOT;
    }
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&event);
    setNonBlocking(fd);
    return  true;
}
/**
 *  添加监听fd的事件.
 * @param fd
 * @param enable_et 是否设置边缘触发
 * @return
 */
bool
Epoll::addListenFd(int fd, bool enable_et) {
    epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;
    if(enable_et){
        event.events |=EPOLLET;
    }
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&event);
    return true;
}
/**
 *
 * @param fd 删除套接字
 * @return
 */
bool
Epoll::delFd(int fd) {
    epoll_event event;
    event.data.fd=fd;
    epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,&event);
    return true;
}
/**
 *
 * @param fd
 * @return
 */
bool
Epoll::reset_one_shot(int fd) {
    epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&event);
    return  true;
}
int
Epoll::setNonBlocking(int fd) {
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return  old_option;
}
/**
 *
 * @param fd 设置fd的写事件
 * @return
 */
bool
Epoll::setEpollout(int fd) {

    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&event);
    return true;
}
/**
 *
 * @return
 */
void
Epoll::startPool() {
    pool.Start(4);
}
bool
Epoll::Epoll_loop() {
    addListenFd(sockfd,false);
    int connfd;
    while(1){
        int ret=epoll_wait(epoll_fd,events,EPOLLEVENTS,-1);
        //std::cout << "11"<<std::endl;
        if(ret < 0){
            std::cout << "epoll failure!"<<std::endl;
            break;
        }
        for(int i=0;i<ret;i++){
            int sock_fd=events[i].data.fd;
            if(sock_fd==sockfd){
               Accept accept(sockfd);
                connfd=accept.Accept_return();
                addFd(connfd,false,false);
            }else if(events[i].events & EPOLLIN){
                /*
                 * 把读任务放进线程池中的任务队列
                 *线程抢锁,执行线程函数
                 */
                //std::cout << "get EPOLLIN "<<std::endl;
                //char buf[100];
                //memset(buf,0,sizeof(buf));
               // int n=read(connfd,buf,100);
                //std::cout << "n:"<<n<<std::endl;
               // std::cout << "n: "<<n<<std::endl;
                //std::cout << buf << std::endl;mZ                
                objectToJson *obj;
                int length=obj->getlength(events[i].data.fd,events,i,epoll_fd);
                //std::cout <<"length::--->"<<length<<std::endl;
                int type=obj->getType(events[i].data.fd,events,i,epoll_fd);
                //std::cout << "type:----->"<<type<<std::endl;
                std::string buf=obj->getJson(events[i].data.fd,events,i,epoll_fd,length);
                //std::cout <<"vvfvf"<<std::endl;
                //std::cout <<"buf::::----->"<<buf<<std::endl;
                Task task(events[i].data.fd,type,buf);
                //std::cout << "Task########################"<<std::endl;
                pool.AddTask(task);
                std::cout << "添加任务之后"<<std::endl;
            }/*else if(events[i].events & EPOLLOUT){

            }else{

            }*/

        }
    }
    close(sockfd);
    close(connfd);
    return true;
}
