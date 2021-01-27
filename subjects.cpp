#include "subjects.h"
#include "dbconnect.h"

vector<Subjects*> Subjects::ReadMainTable()
{

    DBConnect conn;
    conn.connOpen();
    vector<Subjects*> vec;
    QSqlQuery query(conn.mydb);
    query.exec("SELECT * FROM MainTable WHERE IsWorking = 'Эксплуатируется'");
    while(query.next()){
            Subjects *subject = new Subjects();
            subject->ID = query.value(0).toInt();
            subject->Name = query.value(1).toString();
            subject->Number = query.value(2).toInt();
            subject->InitalCost = query.value(3).toDouble();
            subject->InitalCost = round(subject->InitalCost*100)/100;
            subject->Date = query.value(4).toString();
            subject->UsefulTime = query.value(5).toDouble();
            subject->Group = query.value(6).toInt();
            subject->Department = query.value(7).toString();
            subject->IsWorking = query.value(8).toString();
            subject->Account = query.value(9).toString();
            vec.push_back(subject);
    }
    conn.connClose();
    return vec;

}
