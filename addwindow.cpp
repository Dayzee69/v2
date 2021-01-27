#include "addwindow.h"
#include "mainwindow.h"
#include "ui_addwindow.h"
#include <dbconnect.h>
#include <QString>
#include <QMessageBox>

AddWindow::AddWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddWindow),
    accountValidator(QRegExp("^\\d{20,20}$")),
    numbValidator(QRegExp("^\\d{4,4}$")),
    utimeValidator(QRegExp("^[0-9]*[.]?[0-9]+$")),
    costValidator(0, 9999999, 2, this)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    ui->accountLine->setValidator(&accountValidator);
    ui->numbLine->setValidator(&numbValidator);
    ui->utimeLine->setValidator(&utimeValidator);
    ui->costLine->setValidator(&costValidator);
    ui->cbIsDepartment->addItem("Правление");
    ui->cbIsDepartment->addItem("Бухгалтерия");
    ui->cbIsDepartment->addItem("Юридический отдел");
    ui->cbIsDepartment->addItem("Отдел развития");
    ui->cbIsDepartment->addItem("Отдел финансового мониторинга");
    ui->cbIsDepartment->addItem("Операционно - кассового упавления");
    ui->cbIsDepartment->addItem("АХО");
    ui->cbIsDepartment->addItem("Коммерческий отдел");
    ui->cbIsDepartment->addItem("Отдел автоматизации");
    ui->cbIsDepartment->addItem("Служба внутреннего контроля");
    ui->cbIsDepartment->addItem("Общее");
    QDate date;
    ui->dateEdit->setDate(date.currentDate());

}

int AddWindow::SetGroup(const double &check){

    if (check <= 24)
    {
        return 1;
    }
    else if (check > 24 && check <= 36)
    {
        return 2;
    }
    else if (check > 36 && check <= 60)
    {
        return 3;
    }
    else if (check > 60 && check <= 84)
    {
        return 4;
    }
    else if (check > 84 && check <= 120)
    {
        return 5;
    }
    else if (check > 120 && check <= 180)
    {
        return 6;
    }
    else if (check > 180 && check <= 240)
    {
        return 7;
    }
    else if (check > 240 && check <= 300)
    {
        return 8;
    }
    else if (check > 300 && check <= 360)
    {
        return 9;
    }
    else if (check > 360)
    {
        return 10;
    }

    else return 0;
}
void AddWindow::GetInfo()
{
    if(ui->costLine->text() == "" || ui->nameLine->text() == "" || ui->numbLine->text() == "" || ui->utimeLine->text() == "")
    {
        throw runtime_error("Все поля обязательны для заполнения");
    }
    if(ui->numbLine->text().count() < 4)
    {
        throw runtime_error("Индивидуальный номер должен быть больше 4 символов");
    }
    if(ui->accountLine->text() != "")
    {
        if(ui->accountLine->text().count() < 20)
        {
            throw runtime_error("Лицевой счёт должен состоять из 20 цифр");
        }
    }

    Subjects *subject = new Subjects;
    QString s = ui->costLine->text().replace(",", ".");
    subject->InitalCost = s.toDouble();
    subject->InitalCost = round(subject->InitalCost*100)/100;
    subject->Name = ui->nameLine->text();
    subject->Number = ui->numbLine->text().toInt();
    subject->UsefulTime = ui->utimeLine->text().toInt();
    subject->Account = ui->accountLine->text();
    subject->Date = QString::number(ui->dateEdit->date().year()) + "-" + QString::number(ui->dateEdit->date().month()) + "-01";
    subject->Department = ui->cbIsDepartment->currentText();
    subject->Group = SetGroup(subject->UsefulTime);
    subject->IsWorking = "Эксплуатируется";

    AddSub(subject);
}
void AddWindow::AddSub(Subjects *subject)
{
    DBConnect conn;
    conn.connOpen();
    QSqlQuery *query = new QSqlQuery();
         query->prepare("INSERT INTO MainTable (Name, Individual_number, Initial_cost, Date_add, Department, Useful_time, Group_Number, IsWorking, Account) "
                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
         query->addBindValue(subject->Name);
         query->addBindValue(subject->Number);
         query->addBindValue(subject->InitalCost);
         query->addBindValue(subject->Date);
         query->addBindValue(subject->Department);
         query->addBindValue(subject->UsefulTime);
         query->addBindValue(subject->Group);
         query->addBindValue(subject->IsWorking);
         query->addBindValue(subject->Account);
         query->exec();
         conn.connClose();
}
AddWindow::~AddWindow()
{
    delete ui;
}

void AddWindow::on_addButton_clicked()
{
    try {
        GetInfo();
        this->close();
    } catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }


}
