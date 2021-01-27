#include "printwindow.h"
#include "ui_printwindow.h"
#include <QMessageBox>
#include <QtPrintSupport/QPrintDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QFile>
#include <QFileDialog>

PrintWindow::PrintWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintWindow)
{
    ui->setupUi(this);
    QDate date;
    ui->dateEdit->setDate(date.currentDate());
}

PrintWindow::~PrintWindow()
{


    delete ui;
}

void PrintWindow::createHTML(const QString &month, const QString &year)
{
    html = "<html><body><table width=\"100%\" border=1 cellspacing=0>";
    html += "<caption>Ведомость остатков ОС на " + month + "." + year + "</caption>";
    html += "<tr>";
    html += "<th>№</th>";
    html += "<th>Наименование</th>";
    html += "<th>Индивидуальный "
            "номер</th>";
    html += "<th>Первоначальная "
            "стоимоть</th>";
    html += "<th>Сумма модернизации</th>";
    html += "<th>Износ за месяц</th>";
    html += "<th>Начисленный износ</th>";
    html += "<th>Остаточная стоимость</th>";

    html += "<th>Остаточный срок "
            "использования</th>";
    html += "<th>Группа</th>";
    html += "</tr>";

    QString strdate = year + "-" + month + "-" + "01";
    DBConnect conn;
    conn.connOpen();
    QSqlQuery *query = new QSqlQuery();
    query->prepare("SELECT  MainTable.ID, MainTable.Name, MainTable.Individual_number, MainTable.Initial_cost, "
                   "Orders.Depreciation, Orders.Depreciation_cost, Orders.Residual_cost, "
                   "Orders.Residual_date, MainTable.Group_Number "
                   "FROM MainTable, Orders "
                   "WHERE MainTable.ID = Orders.ID AND  Orders.Order_date = :d "
                   "ORDER BY MainTable.Group_Number, MainTable.Individual_number "
                   );
         query->bindValue(":d", strdate);
         query->exec();
         QSqlQueryModel *checkModel = new QSqlQueryModel();
         checkModel->setQuery(*query);
         if(checkModel->rowCount() > 0)
         {

             for (int i =0 ; i < checkModel->rowCount(); i++)
             {

                 html += "<tr>";
                     QSqlQuery *query_mod = new QSqlQuery();
                     query_mod->prepare("SELECT SUM(Modification_cost) "
                                        "FROM Modification "
                                        "WHERE :i = Modification.ID AND :d > Modification.Modification_date "
                                        "GROUP BY Modification.ID "
                                    );
                     query_mod->bindValue(":i", QString::number(checkModel->record(i).value(0).toInt()));
                     query_mod->bindValue(":d", strdate);
                     query_mod->exec();
                     query_mod->next();

                     html += "<th>";
                     html += QString::number(i+1);
                     html += "</th>";
                     html += "<th>";
                     html += checkModel->record(i).value(1).toString();
                     html += "</th>";
                     html += "<th>";
                     html += checkModel->record(i).value(2).toString();
                     html += "</th>";
                     html += "<th>";
                     html += checkModel->record(i).value(3).toString();
                     html += "</th>";
                     html += "<th>";
                     html += query_mod->value(0).toString();
                     html += "</th>";
                     html += "<th>";
                     html += checkModel->record(i).value(4).toString();
                     html += "</th>";
                     html += "<th>";
                     html += checkModel->record(i).value(5).toString();
                     html += "</th>";
                     html += "<th>";
                     html += checkModel->record(i).value(6).toString();
                     html += "</th>";
                     html += "<th>";
                     html += checkModel->record(i).value(7).toString();
                     html += "</th>";
                     html += "<th>";
                     html += checkModel->record(i).value(8).toString();
                     html += "</th>";
                 html += "</tr>";
             }
             html += "</table></body></html>";

             conn.connClose();
         }else
         {
             throw runtime_error("За выбранный период отчёт не найден");
         }

}

void PrintWindow::on_PrintButton_clicked()
{
    try
    {
        QString m;
        if(ui->dateEdit->date().month() < 10)
        {
            m = "0" + QString::number(ui->dateEdit->date().month());
        }else
        {
            m = QString::number(ui->dateEdit->date().month());
        }

        createHTML(m,QString::number(ui->dateEdit->date().year()));
        qDebug()<<html;
        QPrinter printer;
        QPrintDialog *dialog = new QPrintDialog(&printer);
        if(dialog->exec() == QDialog::Accepted) {
          QTextDocument document;
          document.setHtml(html);
          document.print(&printer);
        }
        this->close();

    } catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }

}

void PrintWindow::on_excelButton_clicked()
{
    try
    {
        QString m;
        if(ui->dateEdit->date().month() < 10)
        {
            m = "0" + QString::number(ui->dateEdit->date().month());
        }else
        {
            m = QString::number(ui->dateEdit->date().month());
        }

        createHTML(m,QString::number(ui->dateEdit->date().year()));

        QString filename = QFileDialog::getSaveFileName(this,
                                               tr("Save Excel"), ".",
                                               tr("Xls files (*.xls)"));

        if(filename == "")
        {
            throw runtime_error("Не указано имя файла");

        }

        QFile file(filename);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(html.toUtf8());
            file.close();
        }
        this->close();

    } catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }
}
