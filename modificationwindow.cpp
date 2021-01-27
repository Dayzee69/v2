#include "modificationwindow.h"
#include "ui_modificationwindow.h"
#include <dbconnect.h>
#include <QMessageBox>

ModificationWindow::ModificationWindow(const int &id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModificationWindow),
    costValidator(0, 9999999, 2, this)
{
    ui->setupUi(this);
    subject->ID = id;
    QDate date;
    ui->dateEdit->setDate(date.currentDate());
}

ModificationWindow::~ModificationWindow()
{
    delete ui;
}
void ModificationWindow::setInfo()
{
    if(ui->costLine->text() == "")
    {
        throw runtime_error("Все поля обязательны для заполнения");
    }
    QString s = ui->costLine->text().replace(",", ".");

    subject->InitalCost = s.toDouble();
    subject->InitalCost = round(subject->InitalCost*100)/100;
    subject->Name = ui->nameLine->text();
    QString strdate = QString::number(ui->dateEdit->date().year()) + "-0" + QString::number(ui->dateEdit->date().month()) + "-01";
    subject->Date = strdate;
    subject->IsWorking = "Не эксплуатируется";
    addModification();
}
void ModificationWindow::addModification()
{
    DBConnect conn;
    conn.connOpen();
    QSqlQuery *query = new QSqlQuery();

         query->prepare("INSERT INTO Modification (ID, Modification_name, Modification_cost, Modification_date) "
                       "VALUES (?, ?, ?, ?);");
         query->addBindValue(subject->ID);
         query->addBindValue(subject->Name);
         query->addBindValue(subject->InitalCost);
         query->addBindValue(subject->Date);
         query->exec();
         conn.connClose();
}
void ModificationWindow::on_addButton_clicked()
{
    try {
        setInfo();
        this->close();
    } catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }
}
