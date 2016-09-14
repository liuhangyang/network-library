//
// Created by yang on 16-8-5.
//

#ifndef TSHH_DBUTIL_HPP
#define TSHH_DBUTIL_HPP

#include "ConnectionPoll.h"
#include <string.h>
class DButil {
public:
    DButil()= default;
    ~DButil(){}
    void init();
    int  runSQLCommand(const std::string sql);
    ResultSet * execQuery(const std::string sql);
    void closed(std::shared_ptr<Connection> conn,Statement *statement);


private:

    std::shared_ptr<Connection> conn;
    Statement *statement;

};


#endif //TSHH_DBUTIL_HPP
