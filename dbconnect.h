#ifndef DBCONNECT_H
#define DBCONNECT_H
#include <QtSql>


class DBConnect
{
public:
    DBConnect();
    QSqlDatabase mydb;
    void connClose();
    void connOpen();

};

#endif // DBCONNECT_H
