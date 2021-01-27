#include "xmlwindow.h"
#include <QMessageBox>

XmlWindow::XmlWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XmlWindow)
{
    ui->setupUi(this);
    QDate date;
    ui->dateEdit->setDate(date.currentDate());
}

XmlWindow::~XmlWindow()
{
    delete ui;
}

void XmlWindow::createXML(const int &m, const int &y)
{

    int month = m;
    int year = y;
    QString strdate;
    QString strMonth;
    if(month < 10)
    {
        strdate = QString::number(year) + "-0" + QString::number(month) + "-01";
        strMonth = "0" + QString::number(month);
    }else
    {
        strdate = QString::number(year) + "-" + QString::number(month) + "-01";
        strMonth = QString::number(month);
    }

    DBConnect connxml;
    connxml.connOpen();
    QSqlQuery *querycheck = new QSqlQuery();
    QSqlQueryModel *xmlModel = new QSqlQueryModel();

    querycheck->prepare("SELECT Orders.Depreciation, MainTable.Account FROM MainTable, Orders WHERE Orders.Order_date = :d AND MainTable.ID = Orders.ID AND Orders.Depreciation != 0 AND MainTable.Account != ''");
    querycheck->bindValue(":d", strdate);
    querycheck->exec();
    xmlModel->setQuery(*querycheck);

    if(xmlModel->rowCount() > 0)
    {

        QFile file(dialogOpen());
            file.open(QIODevice::WriteOnly);
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.setCodec("windows-1251");
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("payments");
        for (int i = 0;  i < xmlModel->rowCount(); i++)
        {
            xmlWriter.writeStartElement("payment");
                xmlWriter.writeAttribute("delivery-type", "электронно");
                xmlWriter.writeAttribute("doc-date", QString::number(year) + "-" + strMonth +
                                         "-" + QString::number(ui->dateEdit->date().day()));
                xmlWriter.writeAttribute("doc-number", "");
                xmlWriter.writeAttribute("document_type", "d");
                xmlWriter.writeAttribute("operation-type", "09");
                xmlWriter.writeAttribute("queue", "5");
            xmlWriter.writeStartElement("amount");
                xmlWriter.writeAttribute("currency", "RUB");
                xmlWriter.writeAttribute("value", xmlModel->record(i).value(0).toString());
            xmlWriter.writeEndElement();
            xmlWriter.writeStartElement("payment-info");
                xmlWriter.writeCharacters("Начислена амортизация за " + setMonth(month) + " " + QString::number(ui->dateEdit->date().year()));
            xmlWriter.writeEndElement();
            xmlWriter.writeStartElement("payer");
                xmlWriter.writeAttribute("inn", "7704019762");
                xmlWriter.writeAttribute("cpp", "770401001");
                xmlWriter.writeAttribute("name", "ООО РНКО \"РИБ\"");
                    xmlWriter.writeStartElement("account");
                        xmlWriter.writeAttribute("number", "70606810200004830101");
                            xmlWriter.writeStartElement("bank");
                                xmlWriter.writeAttribute("account-number", "30103810845250000793");
                                xmlWriter.writeAttribute("bic", "044525793");
                                xmlWriter.writeAttribute("name", "ООО РНКО \"РИБ\"");
                                    xmlWriter.writeStartElement("address");
                                        xmlWriter.writeAttribute("city", "Г.МОСКВА");
                                    xmlWriter.writeEndElement();
                            xmlWriter.writeEndElement();
                    xmlWriter.writeEndElement();
            xmlWriter.writeEndElement();
            xmlWriter.writeStartElement("payee");
                xmlWriter.writeAttribute("inn", "7704019762");
                xmlWriter.writeAttribute("cpp", "770401001");
                xmlWriter.writeAttribute("name", "ООО РНКО \"РИБ\"");
                    xmlWriter.writeStartElement("account");
                        xmlWriter.writeAttribute("number", xmlModel->record(i).value(1).toString());
                            xmlWriter.writeStartElement("bank");
                                xmlWriter.writeAttribute("account-number", "30103810845250000793");
                                xmlWriter.writeAttribute("bic", "044525793");
                                xmlWriter.writeAttribute("name", "ООО РНКО \"РИБ\"");
                                    xmlWriter.writeStartElement("address");
                                        xmlWriter.writeAttribute("city", "Г.МОСКВА");
                                    xmlWriter.writeEndElement();
                            xmlWriter.writeEndElement();
                    xmlWriter.writeEndElement();
            xmlWriter.writeEndElement();
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndDocument();
        file.close();
    }else
    {
        throw runtime_error("За выбранный период отчёт не найден");
    }
   connxml.connClose();
}
QString XmlWindow::setMonth(const int &m)
{
    if(m == 1)
    {
        return "январь";
    }else if (m == 2)
    {
        return "февраль";
    }else if (m == 3)
    {
        return "март";
    }else if (m == 4)
    {
        return "апрель";
    }else if (m == 5)
    {
        return "май";
    }else if (m == 6)
    {
        return "июнь";
    }else if (m == 7)
    {
        return "июль";
    }else if (m == 8)
    {
        return "август";
    }else if (m == 9)
    {
        return "сентябрь";
    }else if (m == 10)
    {
        return "октябрь";
    }else if (m == 11)
    {
        return "ноябрь";
    }else if (m == 12)
    {
        return "декабрь";
    } else     throw runtime_error("Не указан месяц");
}
QString XmlWindow::dialogOpen()
{

    QString filename = QFileDialog::getSaveFileName(this,
                                           tr("Save Xml"), ".",
                                           tr("Xml files (*.xml)"));

    if(filename == "")
    {
        throw runtime_error("Не указано имя файла");

    }
    return filename;
}

void XmlWindow::on_createXmlBrutton_clicked()
{
    try{

        createXML(ui->dateEdit->date().month(),ui->dateEdit->date().year());

        this->close();
    } catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }

}
