#ifndef CARPARTSADDER_H
#define CARPARTSADDER_H

#include <QDialog>
#include "carParts.h"
#include <QMessageBox>
#include "utilities.h"

namespace Ui {
class CarPartsAdder;
}

class CarPartsAdder : public QDialog
{
    Q_OBJECT

public:
    explicit CarPartsAdder(QWidget *parent = nullptr);
    ~CarPartsAdder();
    CarParts getCarPart(){return carPart;};
    void resetState();
signals:
    void partCreated();
private slots:
    void on_cpaAddBtn_clicked();

    void on_cpaCnclBtn_clicked();

private:
    Ui::CarPartsAdder *ui;
    CarParts carPart;
    Utilities utilities;
};

#endif // CARPARTSADDER_H
