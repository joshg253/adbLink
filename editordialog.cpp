#include "editordialog.h"
#include "ui_editordialog.h"
#include "searchdialog.h"
#include<QClipboard>
#include <QMessageBox>





#ifdef Q_OS_LINUX
 int edos=0;
#elif defined(Q_OS_WIN)
  int edos=1;
#elif defined(Q_OS_MAC)
int edos=2;
#endif

QClipboard *edclipboard = QApplication::clipboard();


editorDialog::editorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editorDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


}

editorDialog::~editorDialog()
{
    delete ui;
}

/////////////////////////////////////////////////////////////

void editorDialog::seteditor(const QString &xmlfile)
{
    ui->editorBrowser->setText(xmlfile);

}

void editorDialog::on_copyButton_clicked()
{
    edclipboard->setText(ui->editorBrowser->toPlainText());

}

///////////////////////////////////////////////////
QString editorDialog::xmlfile() {
    return ui->editorBrowser->toPlainText();
}

///////////////////////////////////////////////////
void editorDialog::setfilename(const QString &filename)
{
    ui->editorfilename->setText(filename);
}


///////////////////////////////////////////
void editorDialog::on_searchButton_clicked()
{
    searchDialog sdialog;

    sdialog.setModal(true);

     if(sdialog.exec() == QDialog::Accepted)
       {
        QString fstring = sdialog.esearch1();
        QString rstring = sdialog.esearch2();
        QString tempstring = ui->editorBrowser->toPlainText();
        tempstring.replace(fstring,rstring);
        ui->editorBrowser->setText(tempstring);
       }

}
