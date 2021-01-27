#ifndef PRINTWINDOW_H
#define PRINTWINDOW_H

#include <QWidget>
#include <dbconnect.h>
#include <mainwindow.h>

namespace Ui {
class PrintWindow;
}

class PrintWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PrintWindow(QWidget *parent = nullptr);
    ~PrintWindow();

private slots:
    void on_PrintButton_clicked();

    void on_excelButton_clicked();

private:
    Ui::PrintWindow *ui;
    QString html;
    void createHTML(const QString &m, const QString &y);
};

#endif // PRINTWINDOW_H
