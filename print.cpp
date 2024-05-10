#include "print.h"
#include "ui_print.h"


Print::Print(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Print)
{
    ui->setupUi(this);
}

Print::~Print()
{
    delete ui;
}

void Print::print(){
    QTextDocument document;

    QString text = " ૮₍ ˶˃ ⤙ ˂˶ ₎ა WE AE A SERIOUS BUISNESS... YOu know?! ( ｡ •̀ ᴖ •́ ｡ )";
    // Set the document content with separate paragraphs (using either HTML or plain text)
    if(ui->apPriceCheck->isChecked()){
        text += "The resulting estimation of work costs is: " + QString::number(this->estPrice) + "$.\n";
    }
    else if(ui->finCostCheck){
        text += "The final price is: " + QString::number(this->totPrice) + "$.\n";
    }
    text += "UWU";

    document.setHtml(text); // Use setPlainText for plain text formatting

}

void Print::on_printBtn_clicked()
{
    QTextDocument document;

    QString text = " ૮₍ ˶˃ ⤙ ˂˶ ₎ა WE AE A SERIOUS BUISNESS... YOu know?! ( ｡ •̀ ᴖ •́ ｡ )";
    // Set the document content with separate paragraphs (using either HTML or plain text)
    if(ui->apPriceCheck->isChecked()){
        text += "\n The resulting estimation of work costs is: " + QString::number(this->estPrice) + "$.";
    }
    if(ui->finCostCheck){
        text += "\n The final price is: " + QString::number(this->totPrice) + "$.\n";
    }
    text += "\n UWU";

    document.setHtml(text);
    QString filePath = "Receipt.pdf";
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Print the document using QTextDocument::print
    document.print(&printer);
    this->accept();
    this->close();
}


void Print::on_CanceBtn_clicked()
{
    this->reject();
    this->close();
}

