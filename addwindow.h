#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QWidget>
#include <QRegExpValidator>
#include <subjects.h>

namespace Ui {
class AddWindow;
}

class AddWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AddWindow(QWidget *parent = nullptr);
    ~AddWindow();

private slots:
    void on_addButton_clicked();

private:
    Ui::AddWindow *ui;
    QRegExpValidator accountValidator;
    QRegExpValidator numbValidator;
    QRegExpValidator utimeValidator;
    QDoubleValidator costValidator;
    int SetGroup(const double &uTime);
    void GetInfo();
    void AddSub(Subjects *sub);
};

#endif // ADDWINDOW_H
