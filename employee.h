#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>

class Employee {
public:
    Employee(const std::string& name, const std::string& surname, int id, double wage)
        : name(name), surname(surname), id(id), wage(wage) {}

    // Setters
    void setName(const std::string& name) { this->name = name; }
    void setSurname(const std::string& surname) { this->surname = surname; }
    void setId(int id) { this->id = id; }
    void setWage(double wage) { this->wage = wage; }
    void addBusyDate(const std::string& date) { busyDates.push_back(date); }

    // Getters
    std::string getName() const { return name; }
    std::string getSurname() const { return surname; }
    int getId() const { return id; }
    double getWage() const { return wage; }
    std::vector<std::string> getBusyDates() const { return busyDates; }

private:
    std::string name;
    std::string surname;
    int id;
    double wage;
    std::vector<std::string> busyDates;
};



#endif // EMPLOYEE_H
