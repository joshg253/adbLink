#include "oslogdialog.h"
#include "ui_oslogdialog.h"
#include <QProcess>
#include <QMessageBox>
#include<QClipboard>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QStandardPaths>

#ifdef Q_OS_LINUX
 int los1=0;
#elif defined(Q_OS_WIN)
  int los1=1;
#elif defined(Q_OS_MAC)
int los1=2;
#endif

bool osgetfile = true;
QString oslogfile1 = "kodi.log";
QString oslogfile2 = "kodi.old.log";

QString currlog;
QString oldlog;
QString cliplog;

QString logText;
QString oldlogText;

QClipboard *osclipboard = QApplication::clipboard();




oslogDialog::oslogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::oslogDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QString osldir = QDir::homePath();




}

oslogDialog::~oslogDialog()
{
    delete ui;
}

void oslogDialog::ospassdata(const QString &ostype)
{


    // iOS/ATV2	/private/var/mobile/Library/Preferences/kodi.log
    // Linux	$HOME/.kodi/temp/kodi.log
   //   Mac OS X	/Users/<username>/Library/Logs/kodi.log
   //  Windows	%APPDATA%\Kodi\kodi.log


QString tmpdir1;

    logostype = ostype;


    int x = logostype.toInt();


     switch(x)
     {
     case 0:
       return;
     break;

     case 1:
       // windows

        tmpdir1 = QDir::homePath()+ "/AppData/Roaming/Kodi";
         currlog = tmpdir1+"/kodi.log";
         oldlog = tmpdir1+"/kodi.old.log";

         break;

     case 2:
       // macos
         currlog=QDir::homePath()+"/Library/Logs/kodi.log";
         oldlog = QDir::homePath()+"/Library/Logs/kodi.old.log";
      break;

     case 3:
         // linux
         currlog = QDir::homePath()+"/.kodi/kodi.log";
         oldlog = QDir::homePath()+"/.kodi/kodi.old.log";
     break;


     }


     QFile file(currlog);
     if(file.open(QFile::ReadOnly |  QFile::Text))
     {
         QTextStream in(&file);
         logText = in.readAll();
         file.close();
      }
     else
     {
         logText = "Log file not found at "+currlog;

     }

      QFile file2(oldlog);
      if(file2.open(QFile::ReadOnly | QFile::Text))
      {
         QTextStream in(&file2);
         oldlogText = in.readAll();
         file2.close();
       }
      else
      {
          oldlogText= "Log file not found at "+oldlog;

      }


cliplog=logText;
ui->oslogfileName->setText("kodi.log");
ui->oslogBrowser->setPlainText(logText);



}





void oslogDialog::on_oscopyButton_clicked()
{

    osclipboard->setText(cliplog);
    oslogDialog::accept();


}

void oslogDialog::on_osswapButton_clicked()
{


    osgetfile = !osgetfile;

    if (osgetfile)
    {  cliplog=logText;
        ui->oslogfileName->setText("kodi.log");
       ui->oslogBrowser->setPlainText(logText);
    }
    else
     {  cliplog=oldlogText;
        ui->oslogfileName->setText("kodi.old.log");
        ui->oslogBrowser->setPlainText(oldlogText);
    }



}
