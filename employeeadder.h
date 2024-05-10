#ifndef EMPLOYEEADDER_H
#define EMPLOYEEADDER_H

#include <QtSql>
#include <QDialog>
#include "employee.h"
#include "utilities.h"

namespace Ui {
class EmployeeAdder;
}

class EmployeeAdder : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeAdder(QWidget *parent = nullptr);
    ~EmployeeAdder();
    bool employeeSuitabilityCheck( const std::vector<std::pair<QDate,std::vector<int>>>& dates);
    void setSQLDb (QSqlDatabase DB_Connection){this->DB_Connection = DB_Connection;}
    int getIdOfChosenEmp()const{return chosenEmpId;}
signals:
    void addEmployeeBtnClicked();

private slots:
    void on_addEmpBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::EmployeeAdder *ui;
    std::vector<Employee> employees;
    QSqlDatabase DB_Connection;
    int chosenEmpId;
    Utilities utilities;

};

#endif // EMPLOYEEADDER_H
