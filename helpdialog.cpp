#include "helpdialog.h"
#include "ui_helpdialog.h"
#include <QDesktopServices>
#include <QUrl>

helpDialog::helpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


    ui->textBrowser->setSource(*new QUrl("qrc:/assets/menu.html"));

    ui->textBrowser->setWindowTitle("adbLink Help");


}

helpDialog::~helpDialog()
{
    delete ui;
}

