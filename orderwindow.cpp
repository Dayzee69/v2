#include "orderwindow.h"
#include <dbconnect.h>
#include <QTableWidget>
#include <QMessageBox>


OrderWindow::OrderWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    QDate date;
    ui->dateEdit->setDate(date.currentDate());
}

OrderWindow::~OrderWindow()
{
    delete ui;
}

void OrderWindow::createOrder(const int &m, const int &y)
{
    int month = m;
    int year = y;
    QString strdate;
    if(month > 9)
    {
        strdate = QString::number(year) + "-" + QString::number(month) + "-01";
    }else
    {
        strdate = QString::number(year) + "-0" + QString::number(month)+ "-01";
    }

    DBConnect conn;
    conn.connOpen();
    QSqlQuery *querycheck = new QSqlQuery();
    querycheck->prepare("SELECT COUNT(*) FROM Orders WHERE Order_date = :d ");
    querycheck->bindValue(":d", strdate);
    querycheck->exec();
    querycheck->next();
     if(querycheck->value(0).toInt() == 0)
     {
        vector<Subjects*> subjects = Subjects().ReadMainTable();
        for (int i = 0; i < subjects.size(); i++)
        {

            setDepreciation(subjects[i], month, year);


                QSqlQuery *query = new QSqlQuery();
                 query->prepare("INSERT INTO Orders (ID, Residual_cost, Residual_date, Depreciation, Depreciation_cost, Order_date) "
                               "VALUES (?, ?, ?, ?, ?, ?)");
                 query->addBindValue(subjects[i]->ID);
                 query->addBindValue(subjects[i]->ResidualCost);
                 query->addBindValue(subjects[i]->ResidualDate);
                 query->addBindValue(subjects[i]->Depreciation);
                 query->addBindValue(subjects[i]->DepreciationCost);
                 query->addBindValue(strdate);
                 query->exec();
                 conn.connClose();
        }


     }

}

void OrderWindow::setDepreciation(Subjects *subject, int &month , int &year)
{
    DBConnect conn;
    conn.connOpen();
    QSqlQuery *query = new QSqlQuery();
    QString date;
    if(month > 9)
    {
        date = QString::number(year) + "-" + QString::number(month) + "-01";
    }else
    {
        date = QString::number(year) + "-0" + QString::number(month) + "-01";
    }

    query->prepare("SELECT * FROM Modification WHERE ID = :id AND :d > Modification_date");
    query->bindValue(":id", subject->ID);
    query->bindValue(":d", date);
    query->exec();
    QStringList strdate = subject->Date.split("-");

    int summMonth = ((year - strdate[0].toInt()) * 12);
    summMonth += month;
    summMonth -= strdate[1].toInt();


    subject->Depreciation = 0;
    subject->DepreciationCost = 0;
    if(!query->first())
    {
        subject->Depreciation = (subject->InitalCost / subject->UsefulTime);
        subject->Depreciation = round(subject->Depreciation * 100)/100;
         subject->ResidualCost = subject->InitalCost;
        for (int i = 0; i < summMonth; i++)
        {
             subject->ResidualCost -= subject->Depreciation;
             subject->ResidualCost = round(subject->ResidualCost * 100)/100;
             subject->DepreciationCost += subject->Depreciation;
             if(subject->ResidualCost <= 0)
                 break;
        }
        subject->ResidualDate = subject->UsefulTime - summMonth;
        if (subject->ResidualCost <= 0)
            {
                subject->ResidualCost = 0;
                subject->ResidualDate = 0;
                subject->Depreciation = 0;
            }
    }else
    {

        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(*query);

        for (int i =0; i < model->rowCount(); i++)
        {
            date = model->record(i).value(3).toString();
            QStringList datelist = date.split("-", QString::SkipEmptyParts);

            if(strdate[1].toInt() == datelist.at(1).toInt() && strdate[0].toInt() == datelist.at(0).toInt())
            {

                subject->Depreciation = (subject->InitalCost + model->record(i).value(2).toDouble()) / subject->UsefulTime;

                subject->Depreciation = round(subject->Depreciation * 100)/100;

                subject->ResidualCost = subject->InitalCost + model->record(i).value(2).toDouble();

                for (int j = 0; j < summMonth; j++)
                {
                    subject->ResidualCost -= subject->Depreciation;
                    subject->ResidualCost = round(subject->ResidualCost * 100)/100;
                    subject->DepreciationCost += subject->Depreciation;
                    if(subject->ResidualCost <= 0)
                        break;
                }
                subject->ResidualDate = subject->UsefulTime - summMonth;
                if (subject->ResidualCost <= 0)
                    {
                        subject->ResidualCost = 0;
                        subject->ResidualDate = 0;
                        subject->Depreciation = 0;
                    }
            }else
            {
                int data1 = 0;

                data1 = (datelist.at(0).toInt() - strdate[0].toInt()) * 12;

                if(datelist.at(0).toInt() >= strdate[1].toInt())
                {
                    data1 += datelist.at(1).toInt() - strdate[1].toInt();

                }else
                {
                    data1 += strdate[1].toInt() - datelist.at(0).toInt();

                }

                    int data2 = 0;
                    data2 = (year - datelist.at(0).toInt()) * 12;

                        data2 -= (datelist.at(1).toInt() - month);
                    if(subject->Depreciation == 0)
                    {

                        subject->Depreciation = subject->InitalCost / subject->UsefulTime;


                        subject->ResidualCost = subject->InitalCost - subject->Depreciation * data1;
                        subject->DepreciationCost += subject->Depreciation * data1;
                        subject->ResidualCost = round(subject->ResidualCost * 100)/100;

                        if (subject->ResidualCost < 0)
                            subject->ResidualCost = 0;
                        subject->ResidualCost += model->record(i).value(2).toDouble();
                        subject->ResidualCost = round(subject->ResidualCost*100)/100;

                        subject->ResidualDate = subject->UsefulTime - data1;
                        subject->Depreciation = subject->ResidualCost / subject->ResidualDate;

                        subject->Depreciation = round(subject->Depreciation * 100)/100;


                        for (int i = 0; i < data2; i++)
                        {
                             subject->ResidualCost -= subject->Depreciation;
                             subject->ResidualCost = round(subject->ResidualCost * 100)/100;
                             subject->DepreciationCost += subject->Depreciation;
                             if(subject->ResidualCost <= 0)
                                 break;
                        }

                        subject->ResidualDate -= data2;
                        if (subject->ResidualCost <= 0)
                            {
                                subject->ResidualCost = 0;
                                subject->ResidualDate = 0;
                                subject->Depreciation = 0;
                            }

                    }else
                    {

                         subject->ResidualCost += subject->Depreciation * (data2);
                         subject->DepreciationCost -= subject->Depreciation * (data2);
                         subject->ResidualCost += model->record(i).value(2).toDouble();

                         subject->Depreciation = subject->ResidualCost / (subject->ResidualDate + data2);

                         subject->Depreciation = round(subject->Depreciation * 100)/100;

                         for (int i = 0; i < data2; i++)
                         {
                              subject->ResidualCost -= subject->Depreciation;
                              subject->ResidualCost = round(subject->ResidualCost * 100)/100;
                              subject->DepreciationCost += subject->Depreciation;
                              if(subject->ResidualCost <= 0)
                                  break;
                         }

                         if (subject->ResidualCost <= 0)
                             {
                                 subject->ResidualCost = 0;
                                 subject->ResidualDate = 0;
                                 subject->Depreciation = 0;
                             }

                    }

            }

        }
    }
}

QTableWidget* OrderWindow::createTable(const int &m, const int &y)
{
    int month = m;
    int year = y;
    QString strdate;
    if(month > 9)
    {
        strdate = QString::number(year) + "-" + QString::number(month) + "-01";
    }else
    {
        strdate = QString::number(year) + "-0" + QString::number(month) + "-01";
    }
    DBConnect conn;
    conn.connOpen();

    QSqlQuery *query = new QSqlQuery();
    query->prepare("SELECT  MainTable.ID, MainTable.Name, MainTable.Individual_number, MainTable.Initial_cost, "
                   "Orders.Depreciation, Orders.Depreciation_cost, Orders.Residual_cost, "
                   "Orders.Residual_date, MainTable.Group_Number, MainTable.Account "
                   "FROM MainTable, Orders "
                   "WHERE MainTable.ID = Orders.ID AND  Orders.Order_date = :d "
                   "ORDER BY MainTable.Group_Number, MainTable.Individual_number "
                   );

         query->bindValue(":d", strdate);
         query->exec();
         QTableWidget *tableWidget = new QTableWidget();

         tableWidget->setColumnCount(11);
         tableWidget->setFocusPolicy(Qt::NoFocus);
         tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
         tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
         tableWidget->verticalHeader()->setVisible(false);
         tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

         tableWidget->setSortingEnabled(1);
          QStringList headers = {"№","Наименование", "Индивидуальный\nномер",
          "Первоначальная\nстоимоть","Сумма\nмодернизации",
                                 "Износ\nза месяц",
          "Начисленный\nизнос", "Остаточная\nстоимость",
         "Остаточный срок\nиспользования","Группа", "Лицевой счет" };
          tableWidget->setHorizontalHeaderLabels(headers);

              tableWidget->setShowGrid(true);
    QSqlQuery *query_mod = new QSqlQuery();
    for(int i = 0; query->next(); i++){
            tableWidget->insertRow(i);
            query_mod->prepare("SELECT SUM(Modification_cost) "
                               "FROM Modification "
                               "WHERE :i = Modification.ID AND :d > Modification.Modification_date "
                               "GROUP BY Modification.ID "
                           );
            query_mod->bindValue(":i", QString::number(query->value(0).toInt()));
            query_mod->bindValue(":d", strdate);
            query_mod->exec();
            query_mod->next();

            QTableWidgetItem *item = new QTableWidgetItem(QString::number(query_mod->value(0).toDouble(),'f',2));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,4, item);
            item = new QTableWidgetItem(query->value(1).toString());
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,1, item);
            item = new QTableWidgetItem(QString::number(query->value(2).toInt()));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,2, item);
            item = new QTableWidgetItem(QString::number(query->value(3).toDouble(),'f',2));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,3, item);
            item = new QTableWidgetItem(QString::number(query_mod->value(0).toDouble(),'f',2));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,4, item);
            item = new QTableWidgetItem(QString::number(query->value(4).toDouble(),'f',2));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,5, item);
            item = new QTableWidgetItem(QString::number(query->value(5).toDouble(),'f',2));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,6, item);
            item = new QTableWidgetItem(QString::number(query->value(6).toDouble(),'f',2));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,7, item);
            item = new QTableWidgetItem(QString::number(query->value(7).toInt()));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,8, item);
            item = new QTableWidgetItem(QString::number(query->value(8).toInt()));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,9, item);
            item = new QTableWidgetItem(query->value(9).toString());
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tableWidget->setItem(i,10, item);
    }



    conn.connClose();
    tableWidget->hideColumn(0);
    tableWidget->resizeColumnsToContents();
    tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    return tableWidget;
}

void OrderWindow::on_createOrderBrutton_clicked()
{

    try{
        createOrder(ui->dateEdit->date().month(),ui->dateEdit->date().year());
        QString m;
        if(ui->dateEdit->date().month() < 10)
    {
        m = "0" + QString::number(ui->dateEdit->date().month());
    }else
    {
        m = QString::number(ui->dateEdit->date().month());
    }
        emit SendTable(
                  createTable(ui->dateEdit->date().month(), ui->dateEdit->date().year()), m, QString::number(ui->dateEdit->date().year())
                );
    this->close();
    }catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }

}
