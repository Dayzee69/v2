#ifndef XMLWINDOW_H
#define XMLWINDOW_H

#include <QWidget>
#include "ui_xmlwindow.h"
#include "dbconnect.h"
#include "mainwindow.h"
#include <QFileDialog>

namespace Ui {
class XmlWindow;
}

class XmlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit XmlWindow(QWidget *parent = nullptr);
    ~XmlWindow();

private slots:
    void on_createXmlBrutton_clicked();

private:
    Ui::XmlWindow *ui;
    void createXML(const int &m, const int &y);
    QString dialogOpen();
    QString setMonth(const int &m);
};

#endif // XMLWINDOW_H
