#include "searchdialog.h"
#include "ui_searchdialog.h"

searchDialog::searchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

}

searchDialog::~searchDialog()
{
    delete ui;
}

void searchDialog::on_pushButton_2_clicked()
{

}

///////////////////////////////////////////////////
QString searchDialog::esearch1() {
   return ui->fstring->text();
}

///////////////////////////////////////////////////
QString searchDialog::esearch2() {
   return ui->rstring->text();
}

