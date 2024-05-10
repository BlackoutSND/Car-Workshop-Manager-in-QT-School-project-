#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPixmap>
#include <QtSql>
#include <QMessageBox>
#include <QListWidgetItem>
#include "timepicker.h"
#include "ticketviewer.h"
#include <QDesktopServices>
#include <QUrl>
#include <windows.h>
#include "utilities.h"
#include "employeetimeslots.h"

#include <QDialog>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_emplBtn_clicked();

    void on_tktsBtn_clicked();

    void on_empModBtn_clicked();

    void on_empListDropBox_activated(int index);

    void on_empAddButton_clicked();

    void on_quitBtn_clicked();

    void on_empApplyChangesBtn_clicked();

    void on_empRemBtn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_tcAcceptBTn_clicked();

    void on_tcPickTImeSlots_clicked();

    void on_ticketAddBtn_clicked();

    void on_ticketModBtn_clicked();

    void on_ticketView_clicked();

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_ticketViewBtn_clicked();

    void on_ticketRmBtn_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_ticketSOSBtn_clicked();

    void on_empCalendar_activated(const QDate &date);

    void on_tcCancelBtn_clicked();

    void on_dataBtn_clicked();

    void on_stngsBtn_clicked();

    void on_ticket_modified();

private:
    Ui::MainWindow *ui;
    QSqlDatabase DB_Connection;
    timePicker TimePicker;
    TikcetViewer TicketViewerWindow;
    Utilities utilities;
    EmployeeTimeSlots EmployeeTimeSlotsWindow;
};
#endif // MAINWINDOW_H
