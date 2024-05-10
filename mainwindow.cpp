#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "employee.h"
#include "ticket.h"
#include "car.h"
#include "carParts.h"




std::map<int,Employee> emps;
std::unordered_map<int,Ticket> tickets;
int cutEmpIdSelected=-1;
bool empModFlag = false;
bool empAddFlag = false;


int returnTcktIdFromString(QString s, std::unordered_map<int,Ticket> tickets){
    int id;
    int switchState = 0;
    QString st;
    for (int i = 0; i < s.length(); i++) {
        if(s[i]==':'){
            switchState=1;
            continue;
        }
        else if (s[i]==' ')
            break;

        switch (switchState) {
        case 0:
            continue;
            break;
        case 1:
            st+=s[i];
            break;
        default:
            break;
        }
    }
    id = st.toInt();
    return id;
}

void setEmployeeDataInUI(const Employee& emp, Ui::MainWindow* ui) {
    ui->empNameLine->setText(emp.getName());
    ui->empSurnLine->setText(emp.getSurname());
    ui->empIDLine->setText(QString::number(emp.getId()));
    ui->empWageLine->setText(QString::number(emp.getWage()));


    ui->empCalendar->setDateTextFormat( QDate() , QTextCharFormat() );
    QTextCharFormat format;
    format.setBackground(Qt::darkGreen);
    auto empDates = emp.getBusyDates();
        for (auto i = empDates.begin(); i < empDates.end(); ++i) {
        //qDebug()<<i->first.toString("dd.MM.yyyy");
        ui->empCalendar->setDateTextFormat(i->first,format);
    }
}

// void throughAnErrorMsgBox(QString errMsg){
//     QMessageBox messageBox;
//     messageBox.critical(0,"Error","An error has occured! \n"+ errMsg);
//     messageBox.setFixedSize(500,200); kjdfgbijhfihsdfihzwerio
// }

void dbCOnnectionTestAndSetup( QSqlDatabase DB_Connection){
    DB_Connection = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(":/resources/db/carWorkshop.db");
    DB_Connection.setDatabaseName("carWorkshop.db");

    if (!DB_Connection.open()) {
        qDebug() << "Error opening database:" << DB_Connection.lastError().text()<<" >:(";
        throw; //Should add db connection error pop up window
    }
    qDebug() << "The DataBase has been opened Successfully!  >:)";
    //Sleep(5000000);
    //db.close();
}

void empDataDbRequest( Ui::MainWindow* ui, QSqlDatabase DB_Connection){
    emps.clear();
    ui->empListDropBox->clear();

    DB_Connection.transaction();

    QSqlQuery queryLoadData(DB_Connection);
    queryLoadData.prepare("SELECT * FROM employees");
    if (!queryLoadData.exec()) {
        qDebug() << "Error executing query:" << queryLoadData.lastError().text();
        throw; //Should add db connection error pop up window
    }

    QString s;
    QString st;
    while (queryLoadData.next()) {
        Employee tempEmp;
        tempEmp.setId(queryLoadData.value(0).toInt());
        tempEmp.setName( queryLoadData.value(1).toString());
        tempEmp.setSurname(queryLoadData.value(2).toString());
        tempEmp.setWage(queryLoadData.value(3).toDouble());
        st="";
        s = queryLoadData.value(4).toString();
        std::pair<QDate,std::vector<int>> tempDateSlots;
        for (int i = 0; i < s.length(); i++) {
            if(s[i]==';'){
                tempEmp.addBusyDate(tempDateSlots);
                st="";
                continue;
            }
            else if(s[i]==':'){
                tempDateSlots.first = QDate::fromString(st,"dd.MM.yyyy");
                st="";
                continue;
            }
            else if(s[i]=='['){
                continue;
            }
            else if(s[i]==']'){
                tempDateSlots.second.push_back(st.toInt());
                st="";
                continue;
            }
            else
                st+=s[i];

        }

        emps.insert({tempEmp.getId(),tempEmp});
        //qDebug() << queryLoadData.value(0).toInt() <<" "<< queryLoadData.value(1).toString()<<"\n";
    }
    cutEmpIdSelected = emps.begin()->first;

    DB_Connection.close();

    for(auto i = emps.begin();i!=emps.end();i++){
        ui->empListDropBox->addItem("ID:" + QString::number(i->second.getId())+ " Name: " + i->second.getName() + " " + i->second.getSurname());
    }

    Employee tempEmp = emps.find(cutEmpIdSelected)->second;
    setEmployeeDataInUI(tempEmp,ui);
}

void ticketDataDbRequest( Ui::MainWindow* ui, QSqlDatabase DB_Connection){
    tickets.clear();
    ui->listWidget->clear();

    DB_Connection.transaction();

    QSqlQuery queryLoadData(DB_Connection);
    queryLoadData.prepare("SELECT * FROM tickets");
    if (!queryLoadData.exec()) {
        qDebug() << "Error executing query:" << queryLoadData.lastError().text();
        throw; //Should add db connection error pop up window
    }

    while (queryLoadData.next()) {
        Ticket tempTic;
        tempTic.setId(queryLoadData.value(0).toInt());
        QString s = queryLoadData.value(1).toString();
        QString st;
        Car tempCar;
        int carAndPartsSwitch=0;
        for (int i = 0; i <s.length() ; ++i) {
            if(s[i]==';'){
                switch (carAndPartsSwitch) {
                case 0:
                    tempCar.setBrand(st);
                    st="";
                    break;
                case 1:
                    tempCar.setModel(st);
                    st="";
                    break;
                case 2:
                    tempCar.setRegId(st);
                    st="";
                    break;
                default:
                    break;
                }
                carAndPartsSwitch++;
            }
            else
                st+=s[i];
        }
        tempTic.setCarDetailes(tempCar);
        tempTic.setProblemDetails( queryLoadData.value(2).toString());
        st="";
        s = queryLoadData.value(3).toString();
        for (int i= 0; i< s.length(); ++i) {
            if(s[i]==';'){
                tempTic.addAssignedEmpIds(st.toInt());
                st="";
            }
            else
                st+=s[i];
        }

        st="";
        s = queryLoadData.value(4).toString();
        std::pair<QDate,std::vector<int>> tempDateSlots;
        for (int i = 0; i < s.length(); i++) {
            if(s[i]==';'){
                tempTic.addSchedRepairTime(tempDateSlots);
                st="";
                continue;
            }
            else if(s[i]==':'){
                tempDateSlots.first = QDate::fromString(st,"dd.MM.yyyy");
                st="";
                continue;
            }
            else if(s[i]=='['){
                continue;
            }
            else if(s[i]==']'){
                tempDateSlots.second.push_back(st.toInt());
                st="";
                continue;
            }
            else
                st+=s[i];

        }

        CarParts tempCP;
        carAndPartsSwitch = 0;
        st="";
        s = queryLoadData.value(5).toString();
        for (int i = 0; i < s.length(); ++i) {
            if(s[i]==':'||s[i]==';'){
                switch (carAndPartsSwitch) {
                case 0:
                    tempCP.setName(st);
                    st="";
                    carAndPartsSwitch=1;
                    break;
                case 1:
                    tempCP.setAmount(st.toDouble());
                    st="";
                    carAndPartsSwitch=2;
                    break;
                case 2:
                    tempCP.setUnitPrice(st.toDouble());
                    st="";
                    carAndPartsSwitch=0;
                    tempTic.addCarPart(tempCP);
                    continue;
                default:
                    break;
                }
            }
            else{
                st+=s[i];
            }
        }

        st="";
        s = queryLoadData.value(6).toString();                  //probably will remove it 'cause seems to be useless. :(
        for (int i = 0; i < s.length(); i++) {
            if(s[i]==';'){
                tempTic.addSchedRepairTime(tempDateSlots);
                st="";
                continue;
            }
            else if(s[i]==':'){
                tempDateSlots.first = QDate::fromString(st,"dd.MM.yyyy");
                st="";
                continue;
            }
            else if(s[i]=='['){
                continue;
            }
            else if(s[i]==']'){
                tempDateSlots.second.push_back(st.toInt());
                st="";
                continue;
            }
            else
                st+=s[i];

        }
        tempTic.setDeducedProblem(queryLoadData.value(7).toString());

        tempTic.setExpectedCost(queryLoadData.value(8).toDouble());

        if(queryLoadData.value(9).toInt()==1){
            tempTic.setCostIsAccepted(true);
        }
        else
            tempTic.setCostIsAccepted(false);

        tempTic.setPaidByClient(queryLoadData.value(10).toDouble());

        tempTic.setStatus(queryLoadData.value(11).toString());



        tickets.insert({tempTic.getId(),tempTic});
        //qDebug() << queryLoadData.value(0).toInt() <<" "<< queryLoadData.value(1).toString()<<"\n";
    }

    DB_Connection.close();

    for(auto i = tickets.begin();i!=tickets.end();i++){
        //qDebug() <<"ID:" + QString::number(i->first) + " [Car: "+ i->second.getCarDetailes().getBrand()+" " + i->second.getCarDetailes().getModel()+"} "+ i->second.getProblemDetails();
        ui->listWidget->addItem("ID:" + QString::number(i->first) + " [Car: "+ i->second.getCarDetailes().getBrand()+" " + i->second.getCarDetailes().getModel()+"]");
    }

    Employee tempEmp = emps.find(cutEmpIdSelected)->second;
    setEmployeeDataInUI(tempEmp,ui);

}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QObject::connect(&this->TicketViewerWindow, SIGNAL (ticket_modified()), this, SLOT(on_ticket_modified()));
    this->setWindowTitle("AutovaȾDe PRO - The Ultimate Car Workshop Management Application");

    //QPixmap logo(":/resources/imgs/tstLogo.png");
    QPixmap empLogo(":/resources/imgs/EmployeesLogo.png");
    QPixmap logo(":/resources/imgs/newLogo.png");
    QPixmap tckLogo(":/resources/imgs/ticketsLogo.png");

    ui->appLogoLabel->setPixmap(logo);
    ui->empPageHeader->setPixmap(empLogo);
    ui->ticketWinHeader->setPixmap(tckLogo);

    Employee emp = Employee("john","Doe",000,0.01);
    setEmployeeDataInUI(emp,ui);

    dbCOnnectionTestAndSetup(DB_Connection);
    empDataDbRequest(ui,DB_Connection);
    ticketDataDbRequest(ui,DB_Connection);


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_emplBtn_clicked()
{
    ui->Tabs->setCurrentIndex(0);
    empDataDbRequest(ui,this->DB_Connection);


    //Employee tempEmp = emps.find(cutEmpIdSelected)->second;
    //setEmployeeDataInUI(tempEmp,ui);

}


void MainWindow::on_tktsBtn_clicked()
{
    ui->Tabs->setCurrentIndex(1);
    ticketDataDbRequest(ui, DB_Connection);
    ui->ticketModBtn->setDisabled(true);
    ui->ticketRmBtn->setDisabled(true);
    ui->ticketViewBtn->setDisabled(true);
}


void MainWindow::on_empModBtn_clicked()
{
    if(!empModFlag&& !empAddFlag){
        empModFlag=true;
        ui->empNameLine->setReadOnly(false);
        ui->empSurnLine->setReadOnly(false);
        ui->empWageLine->setReadOnly(false);

        ui->empRemBtn->setDisabled(false);
        ui->empModBtn->setText("Cancel");
        ui->empAddButton->setDisabled(true);
        ui->empApplyChangesBtn->setDisabled(false);
        ui->empListDropBox->setDisabled(true);

    }
    else{
        empModFlag=false;
        ui->empNameLine->setReadOnly(true);
        ui->empSurnLine->setReadOnly(true);
        ui->empWageLine->setReadOnly(true);

        ui->empRemBtn->setDisabled(true);
        ui->empModBtn->setText("Modify");
        ui->empAddButton->setDisabled(false);
        ui->empApplyChangesBtn->setDisabled(true);
        ui->empListDropBox->setDisabled(false);

    }
}



void MainWindow::on_empListDropBox_activated(int index)
{
    //empDataDbRequest(this->ui,this->DB_Connection);
    QString s = ui->empListDropBox->currentText();
    QString newID;
    for (int i =3; i < s.length();i++){

        if(s.at(i)==' ')
            break;
        newID+=s.at(i);
    }
    cutEmpIdSelected = newID.toInt();
    Employee tempEmp = emps.find(cutEmpIdSelected)->second;
    setEmployeeDataInUI(tempEmp,ui);
}


void MainWindow::on_empAddButton_clicked()
{
    if( !empModFlag&& !empAddFlag) {
        empAddFlag=true;
        ui->empAddButton->setText("Cancel");
        ui->empModBtn->setDisabled(true);

        ui->empApplyChangesBtn->setDisabled(false);
        ui->empListDropBox->setDisabled(true);

        ui->empNameLine->setText("");
        ui->empSurnLine->setText("");
        ui->empIDLine->setText("");
        ui->empWageLine->setText("");
        ui->empListDropBox->addItem("");

        ui->empNameLine->setReadOnly(false);
        ui->empSurnLine->setReadOnly(false);
        ui->empWageLine->setReadOnly(false);
        ui->empIDLine->setReadOnly(false);

    }
    else{
        empAddFlag=false;
        ui->empAddButton->setText("Add");
        ui->empModBtn->setDisabled(false);
        ui->empApplyChangesBtn->setDisabled(true);
        ui->empListDropBox->setDisabled(false);


        ui->empNameLine->setReadOnly(true);
        ui->empSurnLine->setReadOnly(true);
        ui->empWageLine->setReadOnly(true);
        ui->empIDLine->setReadOnly(true);

        Employee tempEmp = emps.find(cutEmpIdSelected)->second;
        setEmployeeDataInUI(tempEmp,ui);
    }




}


void MainWindow::on_quitBtn_clicked()
{
    QApplication::quit();
}


void MainWindow::on_empApplyChangesBtn_clicked()
{
    if(empModFlag){
        if(ui->empWageLine->text().toDouble()<0.01)
        {
            this->utilities.throughAnErrorMsgBox("An employee's wage: can not be less than 0.01$!");
            return;
        }
        else if(ui->empNameLine->text()=="" && ui->empSurnLine->text() == "")
        {


            this->utilities.throughAnErrorMsgBox("An employee should have a name and a surname!");
            return;
        }
        DB_Connection.open();
        QSqlDatabase::database().transaction();

        QSqlQuery QueryUpdateEmpData(DB_Connection);
        QueryUpdateEmpData.prepare("UPDATE employees SET name = :name, surname = :surname, wage = :wage WHERE id = :id;");
        QueryUpdateEmpData.bindValue(":id", ui->empIDLine->text());
        QueryUpdateEmpData.bindValue(":name", ui->empNameLine->text());
        QueryUpdateEmpData.bindValue(":surname", ui->empSurnLine->text());
        QueryUpdateEmpData.bindValue(":wage", ui->empWageLine->text());
        QueryUpdateEmpData.exec();

        QSqlDatabase::database().commit();
        DB_Connection.close();

    }
    else if(empAddFlag){
        if(emps.find(ui->empIDLine->text().toInt())!=emps.end())
        {
            this->utilities.throughAnErrorMsgBox("An employee with ID:"+ui->empIDLine->text()+ " already exist!");
            return;
        }
        else if(ui->empWageLine->text().toDouble()<0.01)
        {
            this->utilities.throughAnErrorMsgBox("An employee's wage: can not be less than 0.01$!");
            return;
        }
        else if(ui->empNameLine->text()=="" && ui->empSurnLine->text() == "")
        {


            this->utilities.throughAnErrorMsgBox("An employee should have a name and a surname!");
            return;
        }

        DB_Connection.open();
        QSqlDatabase::database().transaction();

        QSqlQuery QueryInsertEmpData(DB_Connection);
        QueryInsertEmpData.prepare("INSERT INTO employees (id, name, surname, wage) VALUES (:id, :name, :surname, :wage);");

        QueryInsertEmpData.bindValue(":id", ui->empIDLine->text());
        QueryInsertEmpData.bindValue(":name", ui->empNameLine->text());
        QueryInsertEmpData.bindValue(":surname", ui->empSurnLine->text());
        QueryInsertEmpData.bindValue(":wage", ui->empWageLine->text());
        QueryInsertEmpData.exec();

        QSqlDatabase::database().commit();
        DB_Connection.close();



    }

    empDataDbRequest(ui,DB_Connection);
    ui->empAddButton->setText("Add");
    ui->empModBtn->setText("Modify");
    ui->empApplyChangesBtn->setDisabled(true);
    ui->empAddButton->setDisabled(false);
    ui->empModBtn->setDisabled(false);
    ui->empListDropBox->setDisabled(false);
    ui->empRemBtn->setDisabled(true);
    empModFlag = false;
    empAddFlag = false;

    //here we send the querry to the DB :)

}




void MainWindow::on_empRemBtn_clicked()
{

    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to REMOVE employee Id: " + ui->empIDLine->text()+"?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        DB_Connection.open();
        QSqlDatabase::database().transaction();

        QSqlQuery QueryDeleteEmpData(DB_Connection);
        QueryDeleteEmpData.prepare("DELETE FROM employees WHERE id = :id;");
        QueryDeleteEmpData.bindValue(":id", ui->empIDLine->text());
        QueryDeleteEmpData.exec();

        QSqlDatabase::database().commit();
        DB_Connection.close();
    }


    empDataDbRequest(ui,DB_Connection);
    ui->empAddButton->setText("Add");
    ui->empModBtn->setText("Modify");
    ui->empApplyChangesBtn->setDisabled(true);
    ui->empAddButton->setDisabled(false);
    ui->empModBtn->setDisabled(false);
    ui->empListDropBox->setDisabled(false);
    ui->empRemBtn->setDisabled(true);
    empModFlag = false;
    empAddFlag = false;
}





void MainWindow::on_tcAcceptBTn_clicked()
{
    if(!this->TimePicker.isHidden())
    {
        this->utilities.throughAnErrorMsgBox("The time slots has not been specified!");
        return;
    }
    else if(ui->tcProbDescrInput->toPlainText()==""||ui->tcCarBrandInput->text()==""||ui->tcCarModelInput->text()==""||ui->tcCarRegInput->text()==""||ui->tcProbDescrInput->toPlainText()==""){
        this->utilities.throughAnErrorMsgBox("Not all required fields has been filled");
        return;
    }

    QString tSlots = this->TimePicker.retTimeSlotsString();
    this->TimePicker.accepted();
    //qDebug() <<" >: "<< tSlots <<" >:( Is it correct?";


    DB_Connection.open();
    QSqlDatabase::database().transaction();

    QSqlQuery QueryAddNewTicket(DB_Connection);
    QueryAddNewTicket.prepare("INSERT INTO tickets ( car_details, problem_details, scheduled_repair_time, status) VALUES (:car_details, :problem_details, :scheduled_repair_time, :status);");
    QString carDets = ui->tcCarBrandInput->text()+";"+ui->tcCarModelInput->text()+";"+ui->tcCarRegInput->text()+";";
    //QueryAddNewTicket.bindValue(":id", ticketIdCounter);
    //ticketIdCounter++;
    QueryAddNewTicket.bindValue(":car_details", carDets);
    //qDebug() <<" >: "<< carDets <<" >:( Is it correct?";
    QString carProbs = ui->tcProbDescrInput->toPlainText();
    QueryAddNewTicket.bindValue(":problem_details", carProbs);
    //qDebug() <<" >: "<< carProbs <<" >:( Is it correct?";

    QueryAddNewTicket.bindValue(":status", "created");
    //qDebug() <<" >: "<< carProbs <<" >:( Is it correct?";

   // s = ui->tcSchedDTInput->date().toString()+" +"+QString::number(ui->tcSchedLenInput->value());
    QueryAddNewTicket.bindValue(":scheduled_repair_time", tSlots);
    if (!QueryAddNewTicket.exec()) {
        qDebug() << "Error executing query:" << QueryAddNewTicket.lastError().text();
    }
    QSqlDatabase::database().commit();
    DB_Connection.close();

    ui->Tabs->setCurrentIndex(1);
    ticketDataDbRequest(ui, DB_Connection);
}



void MainWindow::on_tcPickTImeSlots_clicked()
{

    this->TimePicker.setModal(true);
    this->TimePicker.exec();
}



void MainWindow::on_ticketAddBtn_clicked()
{
    ui->Tabs->setCurrentIndex(2);
}


void MainWindow::on_ticketModBtn_clicked()
{
    int tckID = returnTcktIdFromString(ui->listWidget->currentItem()->text(), tickets);
    qDebug()<<QString::number(tckID);
    this->TicketViewerWindow.setModal(true);
    this->TicketViewerWindow.setTicket(tickets.find(tckID)->second);
    this->TicketViewerWindow.setEmployee(emps);
    this->TicketViewerWindow.setToBeEdited(true);
    this->TicketViewerWindow.updateDataVisual();
    this->TicketViewerWindow.exec();
}


void MainWindow::on_ticketView_clicked()
{

}


void MainWindow::on_listWidget_itemActivated(QListWidgetItem *item)
{
    QMessageBox messageBox;
    messageBox.setText(tickets.find(returnTcktIdFromString(item->text(),tickets))->second.getProblemDetails());
    messageBox.setFixedSize(500,200);
    messageBox.exec();

}

void MainWindow::on_pushButton_clicked(){};
void MainWindow::on_pushButton_5_clicked(){};


void MainWindow::on_ticketViewBtn_clicked()
{
    // QString st;
    // QString s = ui->listWidget->currentItem()->text();
    // int switchState = 0;
    // for (int i = 0; i < s.length(); i++) {
    //     if(s[i]==':'){
    //         switchState=1;
    //         continue;
    //     }
    //     else if (s[i]==' ')
    //         break;

    //     switch (switchState) {
    //     case 0:
    //         continue;
    //         break;
    //     case 1:
    //         st+=s[i];
    //         break;
    //     default:
    //         break;
    //     }
    // }
    QString curText = ui->listWidget->currentItem()->text();
    int tckID = returnTcktIdFromString(curText, tickets);
    qDebug()<<QString::number(tckID);
    this->TicketViewerWindow.setModal(true);
    this->TicketViewerWindow.setTicket(tickets.find(tckID)->second);
    this->TicketViewerWindow.setEmployee(emps);
    this->TicketViewerWindow.setToBeEdited(false);
    this->TicketViewerWindow.updateDataVisual();
    this->TicketViewerWindow.exec();
}


void MainWindow::on_ticketRmBtn_clicked()
{

    //QString st;
    // int switchState = 0;
    // for (int i = 0; i < s.length(); i++) {
    //     if(s[i]==':'){
    //         switchState=1;
    //         continue;
    //     }
    //     else if (s[i]==' ')
    //         break;

    //     switch (switchState) {
    //     case 0:
    //         continue;
    //         break;
    //     case 1:
    //         st+=s[i];
    //         break;
    //     default:
    //         break;
    //     }
    // }
    int ticId = returnTcktIdFromString(ui->listWidget->currentItem()->text(),tickets);
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to REMOVE ticket Id:"+ QString::number(ticId) +"?" );
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        DB_Connection.open();
        QSqlDatabase::database().transaction();

        QSqlQuery QueryDeleteEmpData(DB_Connection);
        QueryDeleteEmpData.prepare("DELETE FROM tickets WHERE id = :id;");
        QueryDeleteEmpData.bindValue(":id", ticId);
        QueryDeleteEmpData.exec();

        QSqlDatabase::database().commit();
        DB_Connection.close();
    }
    ticketDataDbRequest(ui,DB_Connection);
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->ticketModBtn->setDisabled(false);
    ui->ticketRmBtn->setDisabled(false);
    ui->ticketViewBtn->setDisabled(false);
}


void MainWindow::on_ticketSOSBtn_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Help");
    msgBox.setText("Help for Ticket Management System");
    msgBox.setInformativeText("Click on an element to select it.\n"
                              "Double click to view the description.\n"
                              "To see more information about a ticket, select it and press 'View'.\n"
                              "To edit a ticket, select it and press 'Edit'.\n"
                              "For immediate assistance or support, please click 'More help'.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Close);
    QPushButton *suicidePreventionButton = msgBox.addButton("More help", QMessageBox::ActionRole);

    // Connect the button to open a link
    QObject::connect(suicidePreventionButton, &QPushButton::clicked, [&]() {
        QDesktopServices::openUrl(QUrl("https://suicidepreventionlifeline.org/"));
    });

    // Show the QMessageBox
    msgBox.exec();
}


void MainWindow::on_empCalendar_activated(const QDate &date)
{
    QColor cellColor = ui->empCalendar->dateTextFormat(date).background().color();

    if(cellColor == Qt::darkGreen ){
        //here I open a window with time slots;
        std::vector<std::pair<QDate, std::vector<int>>> empDates = emps[cutEmpIdSelected].getBusyDates();
        std::vector<int> dSlots = std::find_if(empDates.begin(),empDates.end(),
                                                    [&]( std::pair<QDate, std::vector<int>>& dates) {return dates.first == date;})->second;
        this->EmployeeTimeSlotsWindow.setDSlots(dSlots);
        this->EmployeeTimeSlotsWindow.redrawSlotsInTable();
        this->EmployeeTimeSlotsWindow.setModal(true);
        this->EmployeeTimeSlotsWindow.show();
    }

}


void MainWindow::on_tcCancelBtn_clicked()
{
    ui->Tabs->setCurrentIndex(1);
    ticketDataDbRequest(ui, DB_Connection);
    ui->ticketModBtn->setDisabled(true);
    ui->ticketRmBtn->setDisabled(true);
    ui->ticketViewBtn->setDisabled(true);
}


void MainWindow::on_dataBtn_clicked()
{
    this->utilities.throughAnErrorMsgBox("This app section is under the maintenence. \n Please try again later.");
}


void MainWindow::on_stngsBtn_clicked()
{
    this->utilities.throughAnErrorMsgBox("This app section is under the maintenence. \n Please try again later.");
}

void MainWindow::on_ticket_modified(){
    ticketDataDbRequest(this->ui,this->DB_Connection);
}
