#ifndef EMPLOYEETIMESLOTS_H
#define EMPLOYEETIMESLOTS_H

#include <QDialog>

namespace Ui {
class EmployeeTimeSlots;
}

class EmployeeTimeSlots : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeTimeSlots(QWidget *parent = nullptr);
    ~EmployeeTimeSlots();
    void setDSlots(const std::vector<int>& dSlots){ this->dSlots = dSlots;}
    void redrawSlotsInTable();
private slots:
    void on_closeButton_clicked();

private:
    Ui::EmployeeTimeSlots *ui;
    std::vector<int> dSlots;
};

#endif // EMPLOYEETIMESLOTS_H
