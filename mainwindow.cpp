#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "employee.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Employee emp = Employee("Wage","Slave",000,0.01);
    ui->empNameBox->setPlainText(QString::fromStdString(emp.getName()));
    ui->empSurNBox->setPlainText(QString::fromStdString(emp.getSurname()));
    ui->empWageBox->setPlainText(QString::number(emp.getWage()));
    ui->empIDBox->setPlainText(QString::number(emp.getId()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_emplBtn_clicked()
{
    ui->Tabs->setCurrentIndex(0);
}


void MainWindow::on_tktsBtn_clicked()
{
    ui->Tabs->setCurrentIndex(1);
}

