#include "employeeadder.h"
#include "ui_employeeadder.h"


bool EmployeeAdder::employeeSuitabilityCheck(const std::vector<std::pair<QDate,std::vector<int>>>& dates){

    employees.clear();
    ui->empList->clear();
    //ui->empListDropBox->clear();

    DB_Connection.transaction();

    QSqlQuery queryLoadData(DB_Connection);
    /*queryLoadData.prepare(*/
    QString query = "SELECT * FROM employees WHERE  busy_time_slot IS NULL";
    if(dates.size()!=0){
        query+=" OR";
        for (auto i = dates.begin(); i != dates.end(); ++i) {
            query+=" busy_time_slot NOT LIKE";
            query+="'%"+i->first.toString("dd.MM.yyyy")+":";
            for (auto j = i->second.begin(); j < i->second.end(); ++j) {
                query+="%["+QString::number(*j)+"]";
            }
            query+="%'";
            if(i+1!=dates.end())
                query+="AND";
            //else

        }
        query+=";";
    }
    qDebug()<<"The resulting query is: "+query;
    queryLoadData.prepare(query);
    if (!queryLoadData.exec()) {
        qDebug() << "Error executing query:" << queryLoadData.lastError().text();
        throw; //Should add db connection error pop up window
    }

    //QString s;
    //QString st;
    while (queryLoadData.next()) {
        Employee tempEmp;
        tempEmp.setId(queryLoadData.value(0).toInt());
        tempEmp.setName( queryLoadData.value(1).toString());
        tempEmp.setSurname(queryLoadData.value(2).toString());
        tempEmp.setWage(queryLoadData.value(3).toDouble());

        this->employees.push_back( tempEmp);
        //qDebug() << queryLoadData.value(0).toInt() <<" "<< queryLoadData.value(1).toString()<<"\n";
    }

    DB_Connection.close();
    for (auto i = this->employees.begin(); i != this->employees.end(); ++i) {
        ui->empList->addItem("["+QString::number(i->getId())+"]: "+  i->getName()+" "+i->getSurname());
    }
    if(this->employees.size()!=0)
        return true;
    else
        return false;
    // for(auto i = emps.begin();i!=emps.end();i++){
    //     ui->empListDropBox->addItem("ID:" + QString::number(i->second.getId())+ " Name: " + i->second.getName() + " " + i->second.getSurname());
    // }

    // Employee tempEmp = emps.find(cutEmpIdSelected)->second;
    // setEmployeeDataInUI(tempEmp,ui);

}


EmployeeAdder::EmployeeAdder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmployeeAdder)
{
    ui->setupUi(this);
    this->setWindowTitle("Employee adder");
}

EmployeeAdder::~EmployeeAdder()
{
    delete ui;
}

void EmployeeAdder::on_addEmpBtn_clicked()
{
    try{
    QString st;
    QString s = ui->empList->currentItem()->text();
    for (int i = 0; i < s.length(); ++i) {
        if(s[i]==']')
            break;
        else if(s[i]=='[')
            continue;
        st+=s[i];

    }

    this->chosenEmpId = st.toInt();


    emit addEmployeeBtnClicked();
    this->hide();
    }
    catch(...){
        this->utilities.throughAnErrorMsgBox("The picked employee can not be assigned to this task!");
        this->close();
    }
}


void EmployeeAdder::on_cancelBtn_clicked()
{
    this->reject();
    this->close();
}

