#include "backupdialog.h"
#include "ui_backupdialog.h"
#include <QProcess>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>



#ifdef Q_OS_LINUX
 int os_backup=0;
#elif defined(Q_OS_WIN)
  int os_backup=1;
#elif defined(Q_OS_MAC)
int os_backup=2;
#endif

QString adb_backup;
QString adbdir_backup;
QString tmpdir_backup;
QString thumbnail_backup_dir;
QString data_root_backup;



///////////////////////////////////////////////
QString RunProcess_backup(QString cstring)
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


QString backupDialog::return_data_root() {
   return data_root_backup;
}


QString backupDialog::returnthumb() {
   // return thumbnail_backup_dir;
    return data_root_backup;
}



void backupDialog::setadb_backup(const QString &adb_backup, const QString &data_root_backup)
{

    QString command;
    QString cstring;
    QString mounted;

    //ui->listDirectories_backup2->item(0)->setSelected(true);
    //ui->listDirectories_backup->item(0)->setSelected(true);

    //ui->backup_label1->setText("Kodi data: "+ data_root_backup);
    //ui->backup_label2->setText("Thumbnails: "+data_root_backup);

    cstring=adb_backup+" shell /data/local/tmp/adblink/busybox find /storage/ -type d -maxdepth 2 -perm 0771";
    command=RunProcess_backup(cstring);

 // QMessageBox::critical(0,"",data_root_backup);



    //QMessageBox::information(0,"",command);

    QFile file21(tmpdir_backup+"temp.txt");



    if(!file21.open(QFile::WriteOnly))

      {
          QMessageBox::critical(this,"","Error creating file!");

          return;
      }


      QTextStream out1(&file21);
      out1  << command << endl;

      file21.flush();
      file21.close();





      QString tmpstr2 = tmpdir_backup+"temp.txt";
      QString fline2;
      QFile file32(tmpstr2);

           if (!file32.open(QIODevice::ReadOnly | QIODevice::Text))
             {QMessageBox::critical(0,"","Error reading file!");
                return; }

           QTextStream in1(&file32);

           ui->listDirectories_backup->addItem("/sdcard/");
           ui->listDirectories_backup2->addItem("/sdcard/");

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


                  ui->listDirectories_backup->addItem(fline2+"/");
                  ui->listDirectories_backup2->addItem(fline2);

                 }
       }

                file32.close();
                QFile::remove(tmpstr2);

          //ui->listDirectories_backup->setCurrentRow(0);

        //  QList<QListWidgetItem *> items = ui->listDirectories_backup->findItems(data_root_backup, Qt::MatchExactly);
        //  if (items.size() > 0) {

        //  }


          for (int i = 0; i < ui->listDirectories_backup->count(); i++) {
            {
              //QMessageBox::critical(0,"",ui->listDirectories_backup->item(i)->text()+"   "+data_root_backup);

              if( ui->listDirectories_backup->item(i)->text() == data_root_backup)
                {
                  //QMessageBox::critical(0,"","Error reading file!");
                  ui->listDirectories_backup->setCurrentRow(i);
              }
             }








          }

}




backupDialog::backupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::backupDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QString tmpstr;
    QString commstr;
    QString cstr;
    QString argument;
    QString adb_backup;
    QString fline;



    if (os_backup == 1)
       {
        tmpdir_backup = "./";
       }


     if (os_backup == 2)
       {
         tmpdir_backup = QCoreApplication::applicationDirPath();
         tmpdir_backup = tmpdir_backup+"/adbfiles/";
       }

     if (os_backup == 0)
       {
         tmpdir_backup = QCoreApplication::applicationDirPath();
         tmpdir_backup = tmpdir_backup+"/adbfiles/";
       }



}

backupDialog::~backupDialog()
{
    delete ui;
}


void backupDialog::on_listDirectories_backup_clicked()
{
    data_root_backup = ui->listDirectories_backup->currentItem()->text();
    //ui->backup_label1->setText("Kodi data: "+ data_root_backup);

    if(data_root_backup!="/sdcard/")
    {
        ui->listDirectories_backup2->clearSelection();
        thumbnail_backup_dir = data_root_backup;
        ui->backup_label2->setText("Thumbnails: "+thumbnail_backup_dir);
    }

}



void backupDialog::on_listDirectories_backup2_clicked()
{
   thumbnail_backup_dir = ui->listDirectories_backup2->currentItem()->text();
   ui->backup_label2->setText("Thumbnails: "+thumbnail_backup_dir);

    if(data_root_backup!="/sdcard/")
    {
        ui->listDirectories_backup2->clearSelection();
        thumbnail_backup_dir = data_root_backup;
        ui->backup_label2->setText("Thumbnails: "+thumbnail_backup_dir);
    }

}

void backupDialog::on_okButton_clicked()
{
    data_root_backup = ui->listDirectories_backup->currentItem()->text();
}
