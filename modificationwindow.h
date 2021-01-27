#ifndef MODIFICATIONWINDOW_H
#define MODIFICATIONWINDOW_H

#include <QWidget>
#include <subjects.h>
#include <QRegExpValidator>


namespace Ui {
class ModificationWindow;
}

class ModificationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ModificationWindow(const int &id, QWidget *parent = nullptr);
    ~ModificationWindow();

private slots:
    void on_addButton_clicked();

private:
    Ui::ModificationWindow *ui;
    QDoubleValidator costValidator;
    Subjects *subject = new Subjects;
    void setInfo();
    void addModification();
};

#endif // MODIFICATIONWINDOW_H
