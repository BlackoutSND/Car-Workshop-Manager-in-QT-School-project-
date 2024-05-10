#include "ticketviewer.h"
#include "ui_ticketviewer.h"



void TikcetViewer::updateDataVisual(){

    ui->tvPaNeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(this->toBeEdited)
    {
        ui->tvAddPartBtn->setDisabled(false);
        ui->tvRemPartBtn->setDisabled(false);
        ui->tvAcceptedByCliCheck->setDisabled(false);
        ui->tvEmpAddBtn->setDisabled(false);
        ui->tvEmpRmBtn->setDisabled(false);
        ui->tvConfirmBtn->setDisabled(false);
        ui->tvGoToDatesBtn->setDisabled(false);


        ui->tvFouProTeEdit->setReadOnly(false);
        ui->tvAmPaidByCliSpin->setReadOnly(false);
        ui->tvProDetTeEdit->setReadOnly(false);
        ui->tvCarModelLine->setReadOnly(false);
        ui->tvCarRegIdLine->setReadOnly(false);
        ui->tvCarBrandLine->setReadOnly(false);
        ui->tvApPriceSpin->setReadOnly(false);
    }
    else{
        ui->tvAddPartBtn->setDisabled(true);
        ui->tvRemPartBtn->setDisabled(true);
        ui->tvAcceptedByCliCheck->setDisabled(true);
        ui->tvEmpAddBtn->setDisabled(true);
        ui->tvEmpRmBtn->setDisabled(true);
        ui->tvConfirmBtn->setDisabled(true);
        ui->tvGoToDatesBtn->setDisabled(false);


        ui->tvFouProTeEdit->setReadOnly(true);
        ui->tvAmPaidByCliSpin->setReadOnly(true);
        ui->tvProDetTeEdit->setReadOnly(true);
        ui->tvCarModelLine->setReadOnly(true);
        ui->tvCarRegIdLine->setReadOnly(true);
        ui->tvCarBrandLine->setReadOnly(true);
        ui->tvApPriceSpin->setReadOnly(true);
    }

    ui->tvCarBrandLine->setText(ticket.getCarDetailes().getBrand());
    ui->tvCarBrandLine->setText(ticket.getCarDetailes().getBrand());
    ui->tvCarModelLine->setText(ticket.getCarDetailes().getModel());
    ui->tvCarRegIdLine->setText(ticket.getCarDetailes().getRegId());

    ui->tvProDetTeEdit->setPlainText(ticket.getProblemDetails());
    for (size_t i = 0; i < ticket.getParts().size(); ++i) {
        int row = ui->tvPaNeTable->rowCount();
        ui->tvPaNeTable->insertRow(row);
        ui->tvPaNeTable->setItem(i,0,new QTableWidgetItem(this->ticket.getParts()[i].getName()));
        ui->tvPaNeTable->setItem(i,1,new QTableWidgetItem(this->ticket.getParts()[i].getName()));
        ui->tvPaNeTable->setItem(i,2,new QTableWidgetItem(this->ticket.getParts()[i].getName()));
    }

    for (size_t i = 0; i < this->ticket.getassignedEmpIds().size(); ++i) {
        ui->tvEmpDropBox->addItem("Id: "+ QString::number(employee.find(this->ticket.getassignedEmpIds()[i])->second.getId())+" "+ employee.find(this->ticket.getassignedEmpIds()[i])->second.getName() + " " + employee.find(this->ticket.getassignedEmpIds()[i])->second.getSurname());
    }
    ui->tvFouProTeEdit->setPlainText(this->ticket.getDeducedProblem());
    ui->tvAmPaidByCliSpin->setValue(this->ticket.getPaidByClient());
    ui->tvApPriceSpin->setValue(this->ticket.getExpectedCost());
    this->redrawEmpsDropBox();
    this->redrawPartsList();
}

void TikcetViewer::redrawPartsList(){
    ui->tvPaNeTable->clear();
    ui->tvPaNeTable->setRowCount(ticket.getParts().size());
    int rowCounter=0;
    std::vector<CarParts> tempCPs = ticket.getParts();
    for (auto i = tempCPs.begin(); i != tempCPs.end(); i++) {

        QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(i->getAmount()));
        QTableWidgetItem *pricePerUnitItem = new QTableWidgetItem(QString::number(i->getUnitPrice()));
        QTableWidgetItem *nameItem = new QTableWidgetItem(i->getName());

        ui->tvPaNeTable->setItem(rowCounter, 0, quantityItem);
        ui->tvPaNeTable->setItem(rowCounter, 1, pricePerUnitItem);
        ui->tvPaNeTable->setItem(rowCounter, 2, nameItem);
        ui->tvApPriceSpin->setValue(this->ticket.getExpectedCost());
        rowCounter++;
    }
}
void TikcetViewer::redrawEmpsDropBox(){
    ui->tvEmpDropBox->clear();
    auto empsIds = this->ticket.getassignedEmpIds();
    for (auto i = empsIds.begin(); i < empsIds.end(); ++i) {
        ui->tvEmpDropBox->addItem("ID: "+QString::number(*i));
    }
}

TikcetViewer::TikcetViewer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TikcetViewer)
{
    ui->setupUi(this);
    QObject::connect(&this->TimePicker, SIGNAL (finishBtn_clicked()), this, SLOT(on_timeHasBeenSubmitted()));
    QObject::connect(&this->carPartsAdder, SIGNAL (partCreated()), this, SLOT(on_partCreated()));
    QObject::connect(&this->employeeAdder, SIGNAL (addEmployeeBtnClicked()), this, SLOT(on_empIsChosen()));

}

TikcetViewer::~TikcetViewer()
{
    delete ui;
}

void TikcetViewer::on_tvCloseBtn_clicked()
{
    this->reject();
}


void TikcetViewer::on_tvGoToDatesBtn_clicked()
{
    // for (auto i = this->ticket.getSchedRepairTime().begin(); i < this->ticket.getSchedRepairTime().end(); ++i) {
    //     qDebug()<<i->first.toString("dd.MM.yyyy")<<"";
    //     for (int j = 0; j < i->second.size(); ++j) {
    //         qDebug()<< QString::number(i->second[j])+" ";
    //     }
    // }
    this->TimePicker.setToBeEdited(this->toBeEdited);
    this->TimePicker.setModal(true);
    this->TimePicker.setTimeSlots(this->ticket.getSchedRepairTime());
    this->TimePicker.updateDataVisual();
    this->TimePicker.exec();
}


void TikcetViewer::on_tvConfirmBtn_clicked() //here I will send the update request to the db
{

    if(!(this->utilities.stringAcceptabilityCheck(ui->tvCarBrandLine->text())||
         this->utilities.stringAcceptabilityCheck(ui->tvCarModelLine->text())||
         this->utilities.stringAcceptabilityCheck(ui->tvCarRegIdLine->text()))){
        return;
    }


    DB_Connection.open();
    QSqlDatabase::database().transaction();

    QSqlQuery QueryUpdateTckData(DB_Connection);
    QueryUpdateTckData.prepare("UPDATE tickets SET car_details = :car_details, problem_details = :problem_details, assigned_employee_ids = :assigned_employee_ids, scheduled_repair_time = :scheduled_repair_time, parts_to_be_bought = :parts_to_be_bought, deduced_problem = :deduced_problem, aproximate_price = :aproximate_price, approx_accepted = :approx_accepted, price_paid =:price_paid, status =:status WHERE id = :id;");
    QString tckId = QString::number(ticket.getId());
    QString carDetails = ui->tvCarBrandLine->text()+";"+ui->tvCarModelLine->text()+";"+ui->tvCarRegIdLine->text()+";";
    QString probDetails = ui->tvProDetTeEdit->toPlainText();
    QString empsIdList ="";
    for (size_t i = 0; i < this->ticket.getassignedEmpIds().size(); ++i) {
        empsIdList+=QString::number(this->ticket.getassignedEmpIds()[i])+";";
    }
    QString schedDates = this->getDatesInString();
    QString carParts;
    std::vector<CarParts> tempCP = this->ticket.getParts();
    for (auto i = tempCP.begin(); i  != tempCP.end(); i++ ) {
        carParts+=i->getName()+":"+ QString::number(i->getAmount())+":"+QString::number(i->getUnitPrice())+";";
    }
    QString deducedProb = ui->tvFouProTeEdit->toPlainText();
    QString apPrice = QString::number(ui->tvApPriceSpin->value());
    int apAcc=0;
    if(ui->tvAcceptedByCliCheck->isChecked())
        apAcc=1;
    QString pricePaid = QString::number(ui->tvAmPaidByCliSpin->value());
    QString newStatus = "InProgress";
    QueryUpdateTckData.bindValue(":id", tckId);
    QueryUpdateTckData.bindValue(":car_details", carDetails);
    QueryUpdateTckData.bindValue(":problem_details", probDetails);
    QueryUpdateTckData.bindValue(":assigned_employee_ids",empsIdList );
    QueryUpdateTckData.bindValue(":scheduled_repair_time", schedDates);
    QueryUpdateTckData.bindValue(":parts_to_be_bought", carParts); // to be done
    QueryUpdateTckData.bindValue(":deduced_problem", deducedProb);
    QueryUpdateTckData.bindValue(":aproximate_price", apPrice);
    QueryUpdateTckData.bindValue(":approx_accepted", apAcc);
    QueryUpdateTckData.bindValue(":price_paid", pricePaid);
    QueryUpdateTckData.bindValue(":status",newStatus);
    if (!QueryUpdateTckData.exec()) {
        qDebug() << "Error executing query:" << QueryUpdateTckData.lastError().text();
    }
    QSqlDatabase::database().commit();

    if(empsIdList!=""){
        QString empBusyDates;
        for (size_t i = 0; i < this->ticket.getassignedEmpIds().size(); ++i) {

            QSqlDatabase::database().transaction();
            QSqlQuery QueryUpdateEmpData(DB_Connection);
            QueryUpdateEmpData.prepare("UPDATE employees SET busy_time_slot = :busy_time_slot WHERE id = :id;");
            int empId = this->ticket.getassignedEmpIds()[i];
            QueryUpdateEmpData.bindValue(":id",QString::number(empId) );
            std::vector<std::pair<QDate,std::vector<int>>> timeSlots = this->employee.find(empId)->second.getBusyDates();
            for (size_t i = 0; i < timeSlots.size(); ++i) {
                empBusyDates+=timeSlots[i].first.toString("dd.MM.yyyy")+":";
                for (size_t j = 0; j < timeSlots[i].second.size(); ++j) {
                    empBusyDates+= "[" +QString::number(timeSlots[i].second[j])+"]";
                }
                empBusyDates+=";";
            }
            empBusyDates =  this->getDatesInString();
            QueryUpdateEmpData.bindValue(":busy_time_slot", empBusyDates);
            if (!QueryUpdateEmpData.exec()) {
                qDebug() << "       Error executing Sub query:" << QueryUpdateEmpData.lastError().text();
            }
            QSqlDatabase::database().commit();
        }
    }


    DB_Connection.close();
    emit this->ticket_modified();
    this->accept();
    this->close();
}

void TikcetViewer::on_timeHasBeenSubmitted(){
    //qDebug()<<"Bruh, signal has worked!!!";
    if(toBeEdited){
        this->ticket.setSchedRepairTime(this->TimePicker.retTimeSlots());
        this->setDatesInString(this->TimePicker.retTimeSlotsString());
    }
    this->TimePicker.accept();
    this->TimePicker.close();
}

void TikcetViewer::on_partCreated(){
    this->ticket.addCarPart(this->carPartsAdder.getCarPart());
    this->ticket.recalExpectedCost();
    this->carPartsAdder.accept();
    this->carPartsAdder.close();
    this->redrawPartsList();
}


void TikcetViewer::on_tvRemPartBtn_clicked()
{
    if(ui->tvPaNeTable->rowCount()==0)
        return;
    CarParts tempCP;
    tempCP.setName(ui->tvPaNeTable->item(ui->tvPaNeTable->currentRow(),2)->text());
    tempCP.setAmount(ui->tvPaNeTable->item(ui->tvPaNeTable->currentRow(),0)->text().toDouble());
    tempCP.setUnitPrice(ui->tvPaNeTable->item(ui->tvPaNeTable->currentRow(),1)->text().toDouble());
    auto tempSaerchCP = ticket.getParts();
    this->ticket.remCarPart(tempCP);
    this->ticket.recalExpectedCost();
    //ticket.getParts().erase(std::find_if(tempSaerchCP.begin(),tempSaerchCP.end(),[&]( CarParts& CP) {return CP.getName()== tempCP.getName();}));
    redrawPartsList();
}


void TikcetViewer::on_tvAddPartBtn_clicked()
{
    this->carPartsAdder.setModal(true);
    this->carPartsAdder.resetState();
    this->carPartsAdder.exec();
}


void TikcetViewer::on_tvEmpAddBtn_clicked()
{

    this->employeeAdder.setModal(true);
    this->employeeAdder.employeeSuitabilityCheck(ticket.getSchedRepairTime());
    this->employeeAdder.exec();

    }

void TikcetViewer::on_empIsChosen(){
    auto tempempIds = this->ticket.getassignedEmpIds();
    if(std::find(tempempIds.begin(), tempempIds.end(), this->employeeAdder.getIdOfChosenEmp())!=tempempIds.end()){
        return;
    }
    this->ticket.addAssignedEmpIds(this->employeeAdder.getIdOfChosenEmp());
        this->employee[this->employeeAdder.getIdOfChosenEmp()].addBusyDatesList(this->ticket.getSchedRepairTime());


    this->employeeAdder.accept();
    this->employeeAdder.close();
    this->redrawEmpsDropBox();

}


void TikcetViewer::on_tvEmpRmBtn_clicked()  //probably works
{
    if(ui->tvEmpDropBox->currentText()=="")
        return;
    QString empEntry = ui->tvEmpDropBox->currentText();
    bool passedInSt = false;
    QString empIdS;
    for (int i = 0; i < empEntry.length(); i++ ) {
        if(empEntry[i]==' ')
            passedInSt = true;
        else if(passedInSt)
            empIdS+=empEntry[i];
    }
    int empId = empIdS.toInt();
    this->ticket.remAssignedEmpId(empId);
    Employee tempEmp = this->employee.find(empId)->second;
    for (size_t i = 0; i < this->ticket.getSchedRepairTime().size(); ++i) {
        tempEmp.remBusyDate(this->ticket.getSchedRepairTime()[i]);
    }
    this->employee.find(empId)->second = tempEmp;
    QString empBusyDates;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery QueryUpdateEmpData(DB_Connection);
    QueryUpdateEmpData.prepare("UPDATE employees SET busy_time_slot = :busy_time_slot WHERE id = :id;");
    QueryUpdateEmpData.bindValue(":id",QString::number(empId) );
    std::vector<std::pair<QDate,std::vector<int>>> timeSlots = this->employee.find(empId)->second.getBusyDates();
    for (size_t i = 0; i < timeSlots.size(); ++i) {
        empBusyDates+=timeSlots[i].first.toString("dd.MM.yyyy")+":";
        for (size_t j = 0; j < timeSlots[i].second.size(); ++j) {
            empBusyDates+= "[" +QString::number(timeSlots[i].second[j])+"]";
        }
        empBusyDates+=";";
    }
    empBusyDates =  this->getDatesInString();
    QueryUpdateEmpData.bindValue(":busy_time_slot", empBusyDates);
    if (!QueryUpdateEmpData.exec()) {
        qDebug() << "Error executing query to remove busy_dates from an employee:" << QueryUpdateEmpData.lastError().text();
    }
    QSqlDatabase::database().commit();
    DB_Connection.close();

    this->redrawEmpsDropBox();

}


void TikcetViewer::on_printButton_clicked()
{
    this->printDialogToPdf();

}

void TikcetViewer::printDialogToPdf(int margins) {
    this->print.setModal(true);
    this->print.setEstPrice(this->ticket.getExpectedCost());
    this->print.setTotPrice(this->ticket.getPaidByClient());
    this->print.show();

}
