#include "sleepdialog.h"
#include "ui_sleepdialog.h"

sleepDialog::sleepDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sleepDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Set timers");

     ui->screenEdit->setInputMask("#9999999999");
     ui->sleepEdit->setInputMask("#9999999999");

}

sleepDialog::~sleepDialog()
{
    delete ui;
}




QString sleepDialog::sleepValue() {
   return ui->sleepEdit->text();
}


QString sleepDialog::screenValue() {
   return ui->screenEdit->text();
}





void sleepDialog::setcurrentscreen(const QString &cscreen)
{
   // ui->screenValue->setText(cscreen);
     ui->screenEdit->setText(cscreen);
}


void sleepDialog::setcurrentsleep(const QString &csleep)
{
  //  ui->sleepValue->setText(csleep);
    ui->sleepEdit->setText(csleep);

}



void sleepDialog::setdevicelabel(const QString &description)
{
    ui->deviceLabel->setText(description);
}



void sleepDialog::on_sleepOff_clicked()
{
    // sleep off 0
     ui->sleepEdit->setText("0");
}

void sleepDialog::on_sleepOn_clicked()
{

   // default sleep 1200000
    ui->sleepEdit->setText("1200000");


}

void sleepDialog::on_screenOff_clicked()
{
   // screensaver off  2147460000
     ui->screenEdit->setText("2147460000");
}

void sleepDialog::on_screenOn_clicked()
{
   // default screensaver 300000
    ui->screenEdit->setText("300000");

}
