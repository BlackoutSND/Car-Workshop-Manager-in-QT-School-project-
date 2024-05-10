#include "picktimeslot.h"
#include "ui_picktimeslot.h"

PickTimeSlot::PickTimeSlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PickTimeSlot)
{
    ui->setupUi(this);
}

PickTimeSlot::~PickTimeSlot()
{
    delete ui;
}
