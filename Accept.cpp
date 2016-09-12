//
// Created by yang on 16-8-10.
//

#include <iostream>
#include "Accept.h"
/**
 *
 * @param fd　传入的是sockfd.
 * @return 构造函数
 */
Accept::Accept(int fd) {
    sockfd=fd;
    socklen_t cliaddrlen;
    connfd=::accept(sockfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
    std::cout << "tid: "<<gettid()<<"IP"<<IPToString(cliaddr.sin_addr.s_addr)<<"port: "<<cliaddr.sin_port<<std::endl;
    if(connfd < 0){
        std::cout << "accept errno(ps:错误日志)"<<std::endl;
    }
}
pid_t
Accept::gettid() {
    return syscall(SYS_gettid);
}
inline
std::string Accept::IPToString(unsigned ipv4) {
    char buf[INET_ADDRSTRLEN]={0};
    struct in_addr in;
    in.s_addr=ipv4;
    if(inet_ntop(AF_INET,&in,buf,sizeof(buf))){
        return  std::string(buf);
    }else{
        return  std::string("");
    }
}
/**
 * 返回connfd
 * @return connfd
 */
int
Accept::Accept_return(){
    return  connfd;
}
/**
 * 把数据写进connfd
 * @param str
 * @return 0 success -1 failed
 */
int
Accept::Accept_write(std::string str) { /*需要改进,可以定义自己的buffer类,把没有写进去的先写到自己的缓存区中*/
    int length=str.length();
    const char *buf=str.c_str();
    int ret=write(connfd,buf,length);
    if(ret==length){
        return  1;
    }
    return  -1;
}
/**
 *
 * @param str
 * @return 1:没有读完,继续读;  0:读完了,-1:出错
 */
int
Accept::Accept_read(char *str){
    char buf[1024];
    str=buf;
    int ret=read(connfd,buf,1024);
    if(ret < 1024 || ret == 0){
        return 0;
    }
    if(ret == 1024){
        return  1;
    }
    return -1;
}
