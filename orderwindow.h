#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QWidget>
#include "ui_orderwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subjects.h"


namespace Ui {
class OrderWindow;
}

class OrderWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OrderWindow(QWidget *parent = nullptr);
    ~OrderWindow();

signals:
    void SendTable(QTableWidget*,QString, QString);
private slots:
    void on_createOrderBrutton_clicked();

private:
    Ui::OrderWindow *ui;
    static void createOrder(const int &month, const int &year);
    QTableWidget* createTable(const int &m, const int &y);
    static void setDepreciation(Subjects *subject, int &month , int &year);

};

#endif // ORDERWINDOW_H
