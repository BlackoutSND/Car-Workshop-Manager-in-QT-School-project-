#ifndef PICKTIMESLOT_H
#define PICKTIMESLOT_H

#include <QWidget>

namespace Ui {
class PickTimeSlot;
}

class PickTimeSlot : public QWidget
{
    Q_OBJECT

public:
    explicit PickTimeSlot(QWidget *parent = nullptr);
    ~PickTimeSlot();

private:
    Ui::PickTimeSlot *ui;
};

#endif // PICKTIMESLOT_H
