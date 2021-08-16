#include "pushfiledialog.h"
#include "ui_pushfiledialog.h"
#include <QFileSystemModel>
#include <QDir>
#include <QDebug>

pushfileDialog::pushfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pushfileDialog)
{
    ui->setupUi(this);

   // QFileSystemModel *dirModel = new QFileSystemModel;

      dirModel = new QFileSystemModel(this);

    dirModel->setRootPath(QDir::homePath());
    ui->treeView->setModel(dirModel);
    ui->treeView->setColumnWidth(0,200);
    ui->treeView->expandToDepth(1);
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
}

pushfileDialog::~pushfileDialog()
{
    delete ui;
}

void pushfileDialog::on_okButton_clicked()
{

   QModelIndexList list = ui->treeView->selectionModel()->selectedIndexes();
   foreach (QModelIndex index, list)
   {

       QString mPath = dirModel->fileInfo(index).absoluteFilePath();
       mstringlist = mstringlist << mPath;

   }


}

///////////////////////////////////////////////////
QStringList pushfileDialog::rstringlist() {
   return mstringlist;
}
