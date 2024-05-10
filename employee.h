#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <vector>
#include <QTSql>
#include <QDebug>
#include <QFileInfo>

class Employee {
public:
    Employee(const QString& name = "Jhon", const QString& surname = "Doe", int id = -1, double wage = 0)
        : name(name), surname(surname), id(id), wage(wage) {}


    // Setters
    void setName(const QString& name) { this->name = name; }
    void setSurname(const QString& surname) { this->surname = surname; }
    void setId(int id) { this->id = id; }
    void setWage(double wage) { this->wage = wage; }
    void addBusyDate(const std::pair<QDate,std::vector<int>>& date) { busyDates.push_back(date); }
    void remBusyDate(const std::pair<QDate,std::vector<int>>& date) {
        //this->busyDates.erase(std::find(this->busyDates.begin(),this->busyDates.end(),date));
        auto it = std::find(this->busyDates.begin(),this->busyDates.end(),date);
        if(it == this->busyDates.end())
            return;
        else
            this->busyDates.erase(it);
    }
    void addBusyDatesList(const std::vector<std::pair<QDate,std::vector<int>>>& dates){
        for (auto i = dates.begin(); i < dates.end(); ++i) {
            this->addBusyDate(*i);
        }
    }
    // Getters
    QString getName() const { return name; }
    QString getSurname() const { return surname; }
    int getId() const { return id; }
    double getWage() const { return wage; }
    std::vector<std::pair<QDate,std::vector<int>>> getBusyDates() const { return busyDates; }

    inline bool operator==( const Employee& other) {
        if(this->getId()==other.getId()){
            return true;
        }
        return false;
    }




private:
    QString name;
    QString surname;
    int id;
    double wage;
    std::vector<std::pair<QDate,std::vector<int>>> busyDates;
};



#endif // EMPLOYEE_H
