#ifndef SUBJECTS_H
#define SUBJECTS_H

#include <QString>
#include <vector>

using namespace std;

class Subjects
{
public:
    int ID;
    QString Name;
    int Number;
    double InitalCost;
    double Depreciation;
    int Group;
    int MonthAdd;
    int YearAdd;
    QString Date;
    double UsefulTime;
    double ResidualCost;
    double DepreciationCost;
    int ResidualDate;
    QString Department;
    QString Account;
    QString IsWorking;
    static vector<Subjects*> ReadMainTable();
};

#endif // SUBJECTS_H
