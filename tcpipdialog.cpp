#include "tcpipdialog.h"
#include "ui_tcpipdialog.h"



int tcpon = 0;

tcpipDialog::tcpipDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tcpipDialog)
{
    ui->setupUi(this);
}


tcpipDialog::~tcpipDialog()
{
    delete ui;
}


/////////////////////////////////////////////////////////////

void tcpipDialog::settcplabel(const QString &labeltext)
{
   ui->tcplabel->setText(labeltext);


}





void tcpipDialog::on_tcpipOn_clicked()
{
    tcpon=1;
}


////////////////////////////////////
int tcpipDialog::returntcp() {
   return tcpon;
}
