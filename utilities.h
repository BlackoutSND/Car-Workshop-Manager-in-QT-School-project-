#ifndef UTILITIES_H
#define UTILITIES_H
#include <QDialog>
#include <QMessageBox>

class Utilities {
public:

    void throughAnErrorMsgBox(QString errMsg){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","An error has occured! \n"+ errMsg);
        messageBox.setFixedSize(500,200);
    }

    //Return true if the string wont break the query. I know, better use regexps and overall fool proof the db querring, but for the time being it should work.
    bool stringAcceptabilityCheck(QString s){
        if(s.contains('"')||s.contains('\'')||s.contains(';')||s.contains(':')||s.contains('\\')){
            QMessageBox messageBox;
            messageBox.critical(0,"Warning","A prohibited symbol has been used!\n");
            messageBox.setFixedSize(500,200);
            return false;
        }
        return true;
    }
};
#endif // UTILITIES_H
