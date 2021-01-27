#include "updatewindow.h"

#include <QMessageBox>

UpdateWindow::UpdateWindow(const int &r, Subjects *sub ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UpdateWindow),
  accountValidator(QRegExp("^\\d{20,20}$")),
  numbValidator(QRegExp("^\\d{4,4}$")),
  utimeValidator(QRegExp("^[0-9]*[.]?[0-9]+$")),
  costValidator(0, 9999999, 2, this)
{
    ui->setupUi(this);
    for (int i = 1; i < 11; i++)
    {
        ui->cbGroup->addItem(QString::number(i));
    }
    ui->costLine->setValidator(&costValidator);
    ui->accountLine->setValidator(&accountValidator);
    ui->numbLine->setValidator(&numbValidator);
    ui->utimeLine->setValidator(&utimeValidator);
    ui->cbIsWorking->addItem("Эксплуатируется");
    ui->cbIsWorking->addItem("Списано");
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


    subject = sub;
    row = r;
    setInfo(subject);

}

void UpdateWindow::setInfo(Subjects *subject)
{
    ui->nameLine->setText(subject->Name);
    ui->numbLine->setText(QString::number(subject->Number));
    QString str = QString::number(subject->InitalCost,'f',2).replace(".", ",");
    ui->costLine->setText(str);
    ui->utimeLine->setText(QString::number(subject->UsefulTime));
    ui->accountLine->setText(subject->Account);
    ui->cbIsWorking->setCurrentText(subject->IsWorking);
    ui->cbIsDepartment->setCurrentText(subject->Department);
    QDate date = QDate::fromString(subject->Date,"MM.yyyy");
    ui->dateEdit->setDate(date);
    ui->cbGroup->setCurrentIndex(subject->Group - 1);


}
void UpdateWindow::updateInfo()
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
    subject->Name = ui->nameLine->text();
    subject->Number = ui->numbLine->text().toInt();
    QString s = ui->costLine->text().replace(",", ".");
    subject->InitalCost = s.toDouble();
    subject->InitalCost = round(subject->InitalCost*100)/100;
    subject->Date = QString::number(ui->dateEdit->date().year()) + "-" + QString::number(ui->dateEdit->date().month()) + "-01";
    subject->UsefulTime = ui->utimeLine->text().toDouble();
    subject->IsWorking = ui->cbIsWorking->currentText();
    subject->Department = ui->cbIsDepartment->currentText();
    subject->Group = ui->cbGroup ->currentText().toInt();
    subject->Account = ui->accountLine->text();
}
void UpdateWindow::updateDB()
{
    DBConnect conn;
    conn.connOpen();
    QSqlQuery *query = new QSqlQuery();
         query->prepare("UPDATE MainTable SET Name = :n, Individual_Number = '"+QString::number(subject->Number)+"', "
                        "Initial_cost = :ic,"
                        "Date_add = :date, Department = :d, Useful_time = '"+QString::number(subject->UsefulTime)+"', "
                        "Group_Number = '"+QString::number(subject->Group)+"', IsWorking = '"+subject->IsWorking+"', Account = '"+subject->Account+"'"
                        "WHERE ID = '"+QString::number(subject->ID)+"';");
         query->bindValue(":n", subject->Name);
         query->bindValue(":ic", subject->InitalCost);
         query->bindValue(":d", subject->Department);
         query->bindValue(":date", subject->Date);
         query->exec();
         conn.connClose();
}
UpdateWindow::~UpdateWindow()
{
    emit sendRow_ID(row,subject->ID);
    delete ui;
}

void UpdateWindow::on_updateButton_clicked()
{
    try
    {
        updateInfo();
        updateDB();
        this->close();
    } catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }
}
