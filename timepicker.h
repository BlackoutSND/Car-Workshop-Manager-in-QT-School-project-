#ifndef TIMEPICKER_H
#define TIMEPICKER_H

#include <QDialog>
#include <vector>
#include <QDate>
#include <QPrinter>
#include "utilities.h"
#include <QPrinter>
#include <QPainter>

namespace Ui {
class timePicker;
}

class timePicker : public QDialog
{
    Q_OBJECT

public:
    explicit timePicker(QWidget *parent = nullptr);
    ~timePicker();

    void addTimeSlots(QDate date, std::vector<int> dSlots){
        std::pair<QDate,std::vector<int>> p {date, dSlots};
        timeSlots.push_back(p);
    };

    QString retTimeSlotsString(){
        QString s;
        for (size_t i = 0; i < timeSlots.size(); ++i) {
            s+=timeSlots[i].first.toString("dd.MM.yyyy")+":";
            for (size_t j = 0; j < timeSlots[i].second.size(); ++j) {
                s+= "[" +QString::number(timeSlots[i].second[j])+"]";
            }
            s+=";";
        }
        return s;
    };
    void setToBeEdited(const bool& toBeEdited){this->toBeEdited=toBeEdited;}

    std::vector<std::pair<QDate,std::vector<int>>> retTimeSlots(){return timeSlots;};

    void setTimeSlots(std::vector<std::pair<QDate,std::vector<int>>> timeSlots){
        this->timeSlots=timeSlots;
    }
    void updateDataVisual();
    void print(int margins = 10);
    void resetCalendar();
signals:
    void finishBtn_clicked();


private slots:


    void on_submitBtn_clicked();

    void on_finishBtn_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_printBtn_clicked();

private:
    Ui::timePicker *ui;
    std::vector<std::pair<QDate,std::vector<int>>> timeSlots;
    bool toBeEdited;

};

#endif // TIMEPICKER_H
