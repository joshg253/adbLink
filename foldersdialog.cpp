#include "foldersdialog.h"
#include "ui_foldersdialog.h"

foldersDialog::foldersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::foldersDialog)
{
    ui->setupUi(this);

}

foldersDialog::~foldersDialog()
{
    delete ui;
}

void foldersDialog::on_fileButton_clicked()
{
   fval=0;
}

void foldersDialog::on_folderButton_clicked()
{
   fval=1;
}



int foldersDialog::returnfval()
{
   return fval;
}

