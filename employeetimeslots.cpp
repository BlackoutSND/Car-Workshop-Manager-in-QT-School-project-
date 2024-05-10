#include "employeetimeslots.h"
#include "ui_employeetimeslots.h"


void EmployeeTimeSlots::redrawSlotsInTable(){
    for (int i = 0; i < ui->timeTable->rowCount(); i++) {
        ui->timeTable->item(i,0)->setCheckState(Qt::Unchecked);
    }
    for (auto i = this->dSlots.begin(); i != this->dSlots.end(); i++) {
        ui->timeTable->item(*i- 8,0)->setCheckState(Qt::Checked);
    }

}



EmployeeTimeSlots::EmployeeTimeSlots(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmployeeTimeSlots)
{
    ui->setupUi(this);
    this->setWindowTitle("Time slots");
}

EmployeeTimeSlots::~EmployeeTimeSlots()
{
    delete ui;
}

void EmployeeTimeSlots::on_closeButton_clicked()
{
    this->dSlots.clear();
    this->accept();
    this->close();
}

