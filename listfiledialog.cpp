#include "listfiledialog.h"
#include "ui_listfiledialog.h"
// #include <QDebug>

listfileDialog::listfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::listfileDialog)
{
    ui->setupUi(this);
}

listfileDialog::~listfileDialog()
{
    delete ui;
}



///////////////////////////////////////////////////

void listfileDialog::setFilelist(const QStringList &filelist) {

// qDebug() << filelist;


for (QStringList::const_iterator it = filelist.begin();
        it != filelist.end(); ++it)
       {
        //currentitem1 = *it;
        ui->flistWidget->addItem(*it);
       }
}

////////////////////////////////////////////////////

void listfileDialog::setDialogTitle(const QString &dtitle) {

 this->setWindowTitle(dtitle);

}


///////////////////////////////////////////////

void listfileDialog::on_selectButton_clicked()
{

    QStringList mstringlist;

    if( ui->flistWidget->selectedItems().count() >= 1 )
     {
        foreach( QListWidgetItem *item, ui->flistWidget->selectedItems() )
          mstringlist << item->text();
     }

    else return;

    fitem=mstringlist.at(0);



}


//////////////////////////////////

void listfileDialog::on_flistWidget_doubleClicked(const QModelIndex &index)
{
   fitem=index.data(Qt::DisplayRole).toString();

   listfileDialog::accept();

}

///////////////////////////////////////////////////
QString listfileDialog::return_fitem() {
   return fitem;
}
