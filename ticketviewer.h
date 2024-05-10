#ifndef TICKETVIEWER_H
#define TICKETVIEWER_H

#include <QDialog>
#include <QEvent>
#include <QDebug>
#include "ticket.h"
#include "car.h"
#include "carParts.h"
#include "employee.h"
#include "timepicker.h"
#include "carpartsadder.h"
#include "employeeadder.h"
#include "utilities.h"
#include "print.h"
#include <QPrinter>
#include <QPainter>

namespace Ui {
class TikcetViewer;
}

class TikcetViewer : public QDialog
{
    Q_OBJECT

public:
    explicit TikcetViewer(QWidget *parent = nullptr );
    ~TikcetViewer();

    Ticket getTicket()const{return ticket;}
    QString getDatesInString()const{return datesInString;}

    void setTicket(const Ticket& ticket){
        this->ticket = ticket;
    }

    void setEmployee(const std::map<int,Employee>& employee){
        this->employee = employee;
    }

    void updateDataVisual ();
    void setDB(const QSqlDatabase& DB_Connection){ this->DB_Connection = DB_Connection;}
    void setDatesInString(const QString& datesInString){this->datesInString=datesInString;}

    void setToBeEdited(const bool& toBeEdited){this->toBeEdited=toBeEdited;};

    void setUpdatedTimeSLots(std::vector<std::pair<QDate,std::vector<int>>> timeSlots){
        this->ticket.setSchedRepairTime(timeSlots);
    }
    void redrawPartsList();
    void redrawEmpsDropBox();
    void printDialogToPdf(int margins = 10);
signals:
    void ticket_modified();

private slots:
    void on_tvCloseBtn_clicked();

    void on_tvGoToDatesBtn_clicked();

    void on_tvConfirmBtn_clicked();

    void on_timeHasBeenSubmitted();

    void on_partCreated();
    void on_tvRemPartBtn_clicked();

    void on_tvAddPartBtn_clicked();

    void on_tvEmpAddBtn_clicked();

    void on_empIsChosen();

    void on_tvEmpRmBtn_clicked();

    void on_printButton_clicked();

private:
    Ui::TikcetViewer *ui;
    Ticket ticket;
    std::map<int,Employee>employee;
    bool toBeEdited;
    timePicker TimePicker;
    EmployeeAdder employeeAdder;
    CarPartsAdder carPartsAdder;
    QSqlDatabase DB_Connection;
    QString datesInString;
    Utilities utilities;
    Print print;

};

#endif // TICKETVIEWER_H
