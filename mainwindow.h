#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "subjects.h"
#include "updatewindow.h"
#include "xmlwindow.h"
#include "dbconnect.h"
#include "orderwindow.h"
#include "addwindow.h"
#include "modificationwindow.h"
#include "printwindow.h"
#include <QTableWidget>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void SendSubject(Subjects*);
private slots:

    void slotCustomMenuRequested(QPoint pos);

    void slotUptade();

    void slotModern();

    void on_addSubject_triggered();

    void on_openOrder_triggered();

    void on_action_XML_triggered();

    void on_tabWidget_tabCloseRequested(const int &index);

    void ReadDBup(const int &row, const int &id);

    void on_action_Print_triggered();

private:
    Ui::MainWindow *ui;
    DBConnect conn;
    QString html;
    void ReadDB();
    void ReadDBadd();

    QWidget* createWidget(QTableWidget *tableWidget);

};

#endif // MAINWINDOW_H
