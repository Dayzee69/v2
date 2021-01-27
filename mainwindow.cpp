#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlError>





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
    try
    {
        ReadDB();
    } catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }

    ui->tabWidget->tabBar()->setTabButton(0,QTabBar::RightSide, nullptr);
    ui->tableWidget->setSortingEnabled(1);

}

void MainWindow::ReadDB()
{


    conn.connOpen();

     ui->tableWidget->setColumnCount(10);
     ui->tableWidget->setFocusPolicy(Qt::NoFocus);
     QStringList headers = {"№","Наименование","Индивидуальный\nномер","Первоначальная\nстоимоть","Дата добавления","Срок\nиспользования",
                            "Группа","Подразделение","В эксплуатации","Лицевой счет"};
     ui->tableWidget->setHorizontalHeaderLabels(headers);
     ui->tableWidget->hideColumn(0);


    ui->tableWidget->setShowGrid(true);
    QSqlQuery query("SELECT * FROM MainTable ORDER BY Group_number");
     for(int i = 0; query.next(); i++){
             ui->tableWidget->insertRow(i);
             for (int j = 0 ; j< 10; j++) {

                 if(j == 4)
                 {
                     QString str =query.value(j).toString();
                     QStringList s = str.split("-");
                     str = s[1]+"."+s[0];
                     QTableWidgetItem *item = new QTableWidgetItem(str);
                     item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                     ui->tableWidget->setItem(i,j, item);

                 }
                 else if(j == 3)
                 {
                     QTableWidgetItem *item = new QTableWidgetItem(QString::number(query.value(3).toDouble(),'f',2));
                     item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                     ui->tableWidget->setItem(i,j, item);

                 }
                 else
                 {
                     QTableWidgetItem *item = new QTableWidgetItem(query.value(j).toString());
                     item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                     ui->tableWidget->setItem(i,j, item);

                 }
             }
     }
         ui->tableWidget->resizeColumnsToContents();
         ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    conn.connClose();

}
void MainWindow::ReadDBadd()
{

    conn.connOpen();

    QSqlQuery *query_check = new QSqlQuery();
    query_check->prepare("SELECT COUNT(*) FROM MainTable");
    query_check->exec();
    query_check->next();
    int check = query_check->value(0).toInt();

   if(check > ui->tableWidget->rowCount())

   {
       QSqlQuery *query = new QSqlQuery();
              query->prepare("SELECT TOP 1 * FROM MainTable ORDER BY ID DESC");

       query->exec();
       query->next();
       ui->tableWidget->insertRow(1);
       for (int i =0 ; i< 10; i++) {
           if(i==4)
           {
               QString str =query->value(i).toString();
               QStringList s = str.split("-");
               str = s[1]+"."+s[0];
               QTableWidgetItem *item = new QTableWidgetItem(str);
               item->setFlags(item->flags() ^ Qt::ItemIsEditable);
               ui->tableWidget->setItem(1,i, item);
           }
           else if(i == 3)
           {
               QTableWidgetItem *item = new QTableWidgetItem(QString::number(query->value(i).toDouble(),'f',2));
               item->setFlags(item->flags() ^ Qt::ItemIsEditable);
               ui->tableWidget->setItem(1,i, item);
           }
           else
           {
               QTableWidgetItem *item = new QTableWidgetItem(query->value(i).toString());
               item->setFlags(item->flags() ^ Qt::ItemIsEditable);
               ui->tableWidget->setItem(1,i, item);
           }
       }

   }


     ui->tableWidget->resizeColumnsToContents();
         ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    conn.connClose();

}

void MainWindow::ReadDBup(const int &row, const int &id)
{

    conn.connOpen();

    QSqlQuery *query = new QSqlQuery();
            query->prepare("SELECT * FROM MainTable WHERE ID = :id");
             query->bindValue(":id", id);
             query->exec();
             query->next();
             for (int j = 0 ; j< 10; j++) {

                 if(j==4)
                 {
                     QString str =query->value(4).toString();
                     QStringList s = str.split("-");
                     str = s[1]+"."+s[0];
                     QTableWidgetItem *item = new QTableWidgetItem(str);
                     item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                     ui->tableWidget->setItem(row,4, item);
                 }
                 else if(j == 3)
                 {
                     QTableWidgetItem *item = new QTableWidgetItem(QString::number(query->value(3).toDouble(),'f',2));
                     item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                     ui->tableWidget->setItem(row,3, item);
                 }
                 else
                 {
                     QTableWidgetItem *item = new QTableWidgetItem(query->value(j).toString());
                     item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                     ui->tableWidget->setItem(row,j, item);
                 }


             }
    conn.connClose();



         ui->tableWidget->resizeColumnsToContents();
         ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);


}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    QMenu * menu = new QMenu(this);
    QAction * editUpdate = new QAction("Изменить", this);
    QAction * editModern = new QAction("Добавить модернизацию", this);
    connect(editUpdate, SIGNAL(triggered()), this, SLOT(slotUptade()));
    connect(editModern, SIGNAL(triggered()), this, SLOT(slotModern()));
    menu->addAction(editUpdate);
    menu->addAction(editModern);
    if(ui->tableWidget->selectionModel()->currentIndex().row() >= 0){
        menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::slotUptade()
{
    Subjects *subject = new Subjects;
    QModelIndex index = ui->tableWidget->currentIndex();
    int setRow = index.row();
    QVariant obj = ui->tableWidget->model()->data(index.sibling(setRow,1));
    subject->Name = obj.toString();
    obj = ui->tableWidget->model()->data(index.sibling(setRow,0));
    subject->ID = obj.toInt();
    obj = ui->tableWidget->model()->data(index.sibling(setRow,2));
    subject->Number = obj.toInt();
    obj = ui->tableWidget->model()->data(index.sibling(setRow,3));
    subject->InitalCost = obj.toDouble();
    subject->InitalCost = round(subject->InitalCost*100)/100;
    obj = ui->tableWidget->model()->data(index.sibling(setRow,4));
    subject->Date = obj.toString();
    obj = ui->tableWidget->model()->data(index.sibling(setRow,5));
    subject->UsefulTime = obj.toDouble();
    obj = ui->tableWidget->model()->data(index.sibling(setRow,6));
    subject->Group = obj.toInt();
    obj = ui->tableWidget->model()->data(index.sibling(setRow,7));
    subject->Department = obj.toString();
    obj = ui->tableWidget->model()->data(index.sibling(setRow,8));
    subject->IsWorking = obj.toString();
    obj = ui->tableWidget->model()->data(index.sibling(setRow,9));
    subject->Account = obj.toString();

    try{
        UpdateWindow *upWin = new UpdateWindow(setRow,subject);
        upWin->setAttribute(Qt::WA_DeleteOnClose);
        connect(upWin, SIGNAL(sendRow_ID(int,int)), this, SLOT(ReadDBup(int,int)));

        upWin->show();
    }catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }

}

void MainWindow::slotModern()
{
    try{
        QModelIndex index = ui->tableWidget->currentIndex();
        int setRow = index.row();
        ModificationWindow *modWin = new ModificationWindow(ui->tableWidget->model()->data(index.sibling(setRow,0)).toInt());
        modWin->show();
    }catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addSubject_triggered()
{
    try{
    AddWindow *addWin = new AddWindow();
    addWin->setAttribute(Qt::WA_DeleteOnClose);
    connect(addWin, &AddWindow::destroyed, this, &MainWindow::ReadDBadd);
    addWin->show();
    }catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }
}

QWidget* MainWindow::createWidget(QTableWidget *tableWidget)
{
    QWidget *widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(tableWidget);
    widget->setLayout(layout);
    return widget;
}

void MainWindow::on_openOrder_triggered()
{
    try{
    OrderWindow *orderWin = new OrderWindow();
    QString date;
    connect(orderWin, &OrderWindow::SendTable, this, [&](QTableWidget *tableWidget, QString month, QString year)
    {

        ui->tabWidget->addTab(createWidget(tableWidget), "Отчёт за "+month+"."+year);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

    });
    orderWin->show();
    }catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }

}

void MainWindow::on_action_XML_triggered()
{
    try{
    XmlWindow *xmlWin = new XmlWindow();
    xmlWin->show();
    }catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(const int &index)
{
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_action_Print_triggered()
{


    try{
    PrintWindow *printWin = new PrintWindow();
    printWin->setAttribute(Qt::WA_DeleteOnClose);
    printWin->show();
    }catch (exception& ex) {
        QMessageBox::warning(this, "Исключение", ex.what());
    }



}
