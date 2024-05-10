#ifndef PRINT_H
#define PRINT_H

#include <QDialog>
#include <QDate>
#include <QPdfDocument>
#include <QApplication>
#include <QPdfWriter>
#include <QPainter>
#include <QTableWidget>
#include <QHeaderView>
#include <utility>
#include <QPrinter>

namespace Ui {
class Print;
}

class Print : public QDialog
{
    Q_OBJECT

public:
    explicit Print(QWidget *parent = nullptr);
    ~Print();
    void setDates(const std::vector<std::pair<QDate,std::vector<int>>> dates){this->dates = dates;};
    void setEstPrice(const double& estPrice){this->estPrice = estPrice;}
    void setTotPrice(const double& totPrice){this->totPrice = totPrice;}
    void print();
private slots:
    void on_printBtn_clicked();

    void on_CanceBtn_clicked();

private:
    Ui::Print *ui;
    std::vector<std::pair<QDate,std::vector<int>>> dates;
    double estPrice;
    double totPrice;
};

#endif // PRINT_H
