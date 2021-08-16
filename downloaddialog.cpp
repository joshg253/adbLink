#include "downloaddialog.h"
#include "ui_downloaddialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

downloadDialog::downloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::downloadDialog)
{
    ui->setupUi(this);
}

downloadDialog::~downloadDialog()
{
    delete ui;
}



bool downloadDialog::getkodiversion() {
   return ui->armv7Button->isChecked();
}


bool downloadDialog::getinstall() {
   return ui->installBox->isChecked();
}


QStringList downloadDialog::getselecteddevices() {

    QStringList mstringlist;

if( ui->listDevices->selectedItems().count() >= 1 )
  {
    foreach( QListWidgetItem *item, ui->listDevices->selectedItems() )
        mstringlist << item->text();
   }

   return mstringlist;

}

void downloadDialog::setdevices(const QStringList &mdevices)
{
    for (QStringList::const_iterator it = mdevices.begin();
            it != mdevices.end(); ++it)
           {
              ui->listDevices->addItem(*it);
            }
}



void downloadDialog::on_okButton_clicked()
{

}
