#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

#include <QWidget>
#include "subjects.h"
#include "ui_updatewindow.h"
#include "dbconnect.h"
#include "mainwindow.h"
#include <QRegExpValidator>

namespace Ui {
class UpdateWindow;
}

class UpdateWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UpdateWindow(const int &r, Subjects *sub, QWidget *parent = nullptr);
    ~UpdateWindow();

signals:
    void sendRow_ID(int row, int id);

private slots:
    void on_updateButton_clicked();

private:
    Ui::UpdateWindow *ui;
    QRegExpValidator accountValidator;
    QRegExpValidator numbValidator;
    QRegExpValidator utimeValidator;
    QDoubleValidator costValidator;
    void setInfo(Subjects *subject);
    void updateInfo();
    void updateDB();
    Subjects *subject;
    int row;

};

#endif // UPDATEWINDOW_H
