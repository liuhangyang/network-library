//
// Created by yang on 16-8-4.
//

#ifndef TSHH_CONNECTIONPOLL_H
#define TSHH_CONNECTIONPOLL_H

#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <functional>

#include <mysql_driver.h>
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/connection.h>
#include <exception>

#include <mutex>
using namespace sql;
class ConnectionPoll {
private:
    int curSize;   //当前数据库中连接数量
    int maxSize;   //连接池中定义的最大的数据库连接池数
    std::string username;  //用户名
    std::string password;  //密码
    std::string url;  //连接url
    std::list<std::shared_ptr<Connection >> connlist;  //存放连接池
    std::mutex lock;  //锁
    static ConnectionPoll *connPool;  //连接池对象.
    Driver *driver;  //mysql driver;

    Connection* CreateConnection();   //创建一个连接
    void InitConnection(int iInitialSize);  //初始化数据库连接池
    void DestoryConnPool();  //销毁数据库连接池.
    void DestoryOneConn();  //销毁一条连接.
    void EnlargeConn(int size);  //扩大数据库连接池
    void ReduceConn(int size); //缩小数据库连接池
    void Addconn(int size);  //添加size个连接
    ConnectionPoll(std::string url,std::string user,std::string password,int maxSize);  //构造方法.
public:
    ~ConnectionPoll();
    int getCurSize();  //当前建立的数据库连接池中的数量.
    int getMaxSize();  //得到连接池中最大
    std::shared_ptr<Connection> GetConnection();//获得一条数据库连接.
    void ReleaseConnection(Connection *conn);  //将数据库连接放回连接池的容器中.
    static ConnectionPoll *GetInstance();     //获取数据库连接池对象
 };


#endif //TSHH_CONNECTIONPOLL_H
