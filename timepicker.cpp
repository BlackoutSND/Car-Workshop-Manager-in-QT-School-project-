#include "timepicker.h"
#include "qtextformat.h"
#include "ui_timepicker.h"
#include <QTableWidgetItem>



void timePicker::updateDataVisual(){
    ui->calendarWidget->setDateTextFormat( QDate() , QTextCharFormat() );
    QTextCharFormat format;
    format.setBackground(Qt::darkGreen);
    for (auto i = this->timeSlots.begin(); i < this->timeSlots.end(); ++i) {
        //qDebug()<<i->first.toString("dd.MM.yyyy");
        ui->calendarWidget->setDateTextFormat(i->first,format);
    }

    for (int i = 0; i < ui->timeTable->rowCount(); i++) {
        ui->timeTable->item(i,0)->setCheckState(Qt::Unchecked);
    }

    for (auto i = this->timeSlots.begin();i!=this->timeSlots.end();i++) {
        if(QDate::currentDate().addDays(1) == i->first){
            for (auto j = i->second.begin();j!=i->second.end();j++) {
                ui->timeTable->item(*j- 8,0)->setCheckState(Qt::Checked);
            }
            break;
        }
    }
    if(!this->toBeEdited){
        ui->submitBtn->setDisabled(true);
        ui->finishBtn->setDisabled(false);
    }
    else{
        ui->submitBtn->setDisabled(false);
        ui->finishBtn->setDisabled(true);
    }


}


timePicker::timePicker(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::timePicker)
{
    ui->setupUi(this);
    ui->calendarWidget->setMinimumDate(QDate::currentDate().addDays(1));



}

timePicker::~timePicker()
{
    delete ui;
}




void timePicker::on_submitBtn_clicked()
{

    QTextCharFormat format;
    format.setBackground(Qt::darkGreen); // Set background color for the marked date
    QDate s = ui->calendarWidget->selectedDate();
    std::vector<int> timeS;
    for (int i = 0; i < ui->timeTable->rowCount(); i++) {
        if(ui->timeTable->item(i,0)->checkState()!=false){
            timeS.push_back(i+8);
            QTableWidgetItem it;
            ui->timeTable->item(i,0)->setCheckState(Qt::Unchecked);
            //ui->calendarWidget->setDateTextFormat(s,format);
        }
    }




    for (auto i = this->timeSlots.begin(); i < this->timeSlots.end(); ++i) {
        if(i->first==s){
            this->timeSlots.erase(i);
            QColor color("#313131");
            format.setBackground(color);
            ui->calendarWidget->setDateTextFormat(s,format);
            break;
        }
    }

    if(timeS.size()!=0){
        this->addTimeSlots(s,timeS);
        ui->finishBtn->setDisabled(false);
        ui->calendarWidget->setDateTextFormat(s,format);
    }

}


void timePicker::on_finishBtn_clicked()
{
    if(this->toBeEdited){
        emit finishBtn_clicked();
        this->hide();
    }
    else
        this->close();

}


void timePicker::on_calendarWidget_clicked(const QDate &date)
{

    for (int i = 0; i < ui->timeTable->rowCount(); i++) {
        ui->timeTable->item(i,0)->setCheckState(Qt::Unchecked);
    }

    for (auto i = this->timeSlots.begin();i!=this->timeSlots.end();i++) {
        if(date == i->first){
            for (auto j = i->second.begin();j!=i->second.end();j++) {
                ui->timeTable->item(*j- 8,0)->setCheckState(Qt::Checked);
            }
            break;
        }
    }
}


void timePicker::print(int margins) {
    QString filePath = "Schedule.pdf";
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setFullPage(true);
    printer.setPageSize(QPageSize::A4);


    QPainter painter;
    if (!painter.begin(&printer)) {
        qWarning("failed to open file, is it writable?");
        return;
    }

    QRect printableRect = printer.pageRect(QPrinter::Millimeter).toRect();
    printableRect = printableRect.adjusted(margins, margins, -margins, -margins);


    painter.translate(printableRect.center().x(), printableRect.center().y());
    painter.scale(12, 12);


    this->render(&painter);

    painter.end();
}

void timePicker::on_printBtn_clicked()
{
    this->print();
}

