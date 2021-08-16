#include "adbprefdialog.h"
#include "ui_adbprefdialog.h"
#include <QFileDialog>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork/QNetworkInterface>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QDesktopServices>
#include <QDir>

adbprefDialog::adbprefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adbprefDialog)
{
    ui->setupUi(this);
}

adbprefDialog::~adbprefDialog()
{
    delete ui;
}

void adbprefDialog::on_checkButton_clicked()
{
    QNetworkRequest request;
    // request.setUrl(QUrl("http://www.elkins.org/version.txt"));
 
    request.setUrl(QUrl("http://www.jocala.com/version.txt"));
 
    QNetworkAccessManager *m_networkManager = new QNetworkAccessManager();
    QNetworkReply *reply = m_networkManager->get(request);

    connect(reply, SIGNAL(finished()),
            this, SLOT(onRequestCompleted()));
}

void adbprefDialog::onRequestCompleted() {
   QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());


   if (reply->error() != QNetworkReply::NoError)
   {

       int err = reply->error();
       QString s2 = QString::number(err);
       QMessageBox::critical(0, "","Network error: " + s2,QMessageBox::Cancel);
       return;
   }



   QByteArray data = reply->readAll();

   QString s1(data);

       s1 = strip2(s1);

       int err = reply->error();

       QString s2 = QString::number(err);


       if (version2 != s1)
       {
           QMessageBox::StandardButton rep;
            rep = QMessageBox::question(0, "", "Version "+s1+" is ready. Download?",
                    QMessageBox::Yes|QMessageBox::No);
              if (rep == QMessageBox::Yes)
               {
                  QString link = "http://www.jocala.com";
                  QDesktopServices::openUrl(QUrl(link));

                }

       }

       else
           QMessageBox::information(0, "","No adbLink update available",QMessageBox::Cancel);

       delete reply;

}

void adbprefDialog::setversionLabel(const QString &versiontext)
{
    ui->versionLabel->setText("adbLink version: "+ versiontext);
    version2 = versiontext;
}

void adbprefDialog::setversioncheck(const bool &versioncheck)
{
    ui->versioncheck->setChecked(versioncheck);
}

void adbprefDialog::setdownloaddir(const QString &ddir)
{
    ui->dfilepath->setText(ddir);
}



void adbprefDialog::setmsgboxtype(const bool &msgboxtype)
{
    ui->msgboxtype->setChecked(msgboxtype);
}

bool adbprefDialog::versioncheck() {
   return ui->versioncheck->isChecked();
}



QString adbprefDialog::downloaddir() {
   return ui->dfilepath->text();
}



bool adbprefDialog::msgboxtype() {
   return ui->msgboxtype->isChecked();
}

void adbprefDialog::on_downloadButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose download path"),
                                                 ui->dfilepath->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty() )
    {
        ui->dfilepath->setText(dir);
    }
}


QString adbprefDialog::strip2 (QString str)
{
   str = str.simplified();
   str.replace( " ", "" );
   return str;
}
