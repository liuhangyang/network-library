//
// Created by yang on 16-8-5.
//

#include "DButil.h"
ConnectionPoll *connPool=ConnectionPoll::GetInstance();
int
DButil::runSQLCommand(const std::string sql) {
    int flag=0;
    if(sql.empty()){
        return  flag;
    }
    std::shared_ptr<Connection> conn=connPool->GetConnection();
    Statement *statement=conn->createStatement();
    statement->execute("use test");
    flag=statement->executeUpdate(sql);
    closed(conn,statement);
    return flag;
}
ResultSet* DButil::execQuery(const std::string sql) {
    ResultSet *result= nullptr;
    if(sql.empty()){
        return  result;
    }
    std::shared_ptr<Connection> conn=connPool->GetConnection();
    Statement *statement=conn->createStatement();
    statement->execute("use test");
    result=statement->executeQuery(sql);
    closed(conn,statement);
    return result;

}
void DButil::closed(std::shared_ptr<Connection> conn,Statement *statement) {
    statement->close();
    connPool->ReleaseConnection(conn);
}

