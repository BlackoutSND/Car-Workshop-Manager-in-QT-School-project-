#include "carpartsadder.h"
#include "ui_carpartsadder.h"

CarPartsAdder::CarPartsAdder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CarPartsAdder)
{
    ui->setupUi(this);
    ui->cpaPriceSpin->setMinimum(0);
    ui->cpaQuantitySpin->setMinimum(0);
    this->setWindowTitle("Car Parts adder");
}

void CarPartsAdder::resetState(){
    ui->cpaPriceSpin->setValue(0);
    ui->cpaQuantitySpin->setValue(0);
    ui->cpaPaNaLine->setText("");
}

//make an additional class with utility functions!~!!!
// void throughAMsgBox(QString errMsg){
//     QMessageBox messageBox;
//     messageBox.critical(0,"Error","An error has occured! \n"+ errMsg);
//     messageBox.setFixedSize(500,200);
// }

CarPartsAdder::~CarPartsAdder()
{
    delete ui;
}

void CarPartsAdder::on_cpaAddBtn_clicked()
{
    if(ui->cpaPaNaLine->text()==""){
        this->utilities.throughAnErrorMsgBox("Part's name cannot be empty!");
        return;
    }
    else if(!this->utilities.stringAcceptabilityCheck(ui->cpaPaNaLine->text())){
        return;
    }
    else if(ui->cpaQuantitySpin->value()==0){
        this->utilities.throughAnErrorMsgBox("Part's quantity cannot be equal to '0'!");
        return;
    }
    carPart.setName(ui->cpaPaNaLine->text());
    carPart.setAmount(ui->cpaQuantitySpin->value());
    carPart.setUnitPrice(ui->cpaPriceSpin->value());
    emit partCreated();
    this->hide();
}


void CarPartsAdder::on_cpaCnclBtn_clicked()
{
    this->reject();
}

