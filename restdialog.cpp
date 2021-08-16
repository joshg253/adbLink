#include "restdialog.h"
#include "ui_restdialog.h"
#include <QProcess>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>



#ifdef Q_OS_LINUX
 int os_restore=0;
#elif defined(Q_OS_WIN)
  int os_restore=1;
#elif defined(Q_OS_MAC)
int os_restore=2;
#endif

QString data_root_restore;
QString tmpdir_restore;
QString adb_restore;
QString partition_restore;

///////////////////////////////////////////////
QString RunProcess_restore(QString cstring)
{
 QProcess run_command;
 run_command.setProcessChannelMode(QProcess::MergedChannels);
 run_command.start(cstring);

 run_command.waitForStarted();

 while(run_command.state() != QProcess::NotRunning)
     qApp->processEvents();

 QString command=run_command.readAll();

 return command;
}


restDialog::restDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::restDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if (os_restore == 1)
       {
        tmpdir_restore = "./";
         //adb_restore = tmpdir_restore+"adb.exe";
       }

       else

       {
         tmpdir_restore = QCoreApplication::applicationDirPath()+"/adbfiles/";
         //adb_restore = tmpdir_restore+"adb";
       }





}

restDialog::~restDialog()
{
    delete ui;
}



void restDialog::on_listDirectories_restore_clicked()
{
   data_root_restore = ui->listDirectories_restore->currentItem()->text();
}

 void restDialog::setadb_restore(const QString &adb_restore)
    {

        QString command;
        QString cstring;
        QString mounted;
        QString fline;


        cstring=adb_restore+" shell /data/local/tmp/adblink/busybox find /storage/ -type d -maxdepth 2 -perm 0771";
        command=RunProcess_restore(cstring);

        //QMessageBox::information(0,"",cstring);

        QFile file21(tmpdir_restore+"temp.txt");



        if(!file21.open(QFile::WriteOnly))

          {
              QMessageBox::critical(this,"","Error creating file!");

              return;
          }


          QTextStream out1(&file21);
          out1  << command << endl;

          file21.flush();
          file21.close();





          QString tmpstr2 = tmpdir_restore+"temp.txt";
          QString fline2;
          QFile file32(tmpstr2);

               if (!file32.open(QIODevice::ReadOnly | QIODevice::Text))
                 {QMessageBox::critical(0,"","Error reading file!");
                    return; }

               QTextStream in1(&file32);

              ui->listDirectories_restore->addItem("/sdcard/");


                while (!in1.atEnd())
                 {



             fline2 = in1.readLine();

                  if( (!fline2.contains("Android"))
                          && (!fline2.contains("."))
                          && (!fline2.contains("Permission denied"))
                          && (!fline2.contains("emulated"))
                          && (!fline2.isEmpty())

                          ) // endif
                     {


                      ui->listDirectories_restore->addItem(fline2+"/");

                     }


                    }



                     file32.close();
                      QFile::remove(tmpstr2);              
                      ui->listDirectories_restore->setCurrentRow(0);
                      data_root_restore = ui->listDirectories_restore->currentItem()->text();

                     }



QString restDialog::restore_data_root() {
   return data_root_restore;
}

QString restDialog::on_listDirectories_restore_doubleClicked()
{
    data_root_restore = ui->listDirectories_restore->currentItem()->text();
    return data_root_restore;
    restDialog::accept();
}
