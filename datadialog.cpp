#include "datadialog.h"
#include "ui_datadialog.h"
#include <QProcess>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>

#ifdef Q_OS_LINUX
 int os_data=0;
#elif defined(Q_OS_WIN)
  int os_data=1;
#elif defined(Q_OS_MAC)
int os_data=2;
#endif

int rval2;
QString adbloc;
QString adb_data;
QString adbdir_data_data;
QString tmpdir_data;

QString external_Location;

///////////////////////////////////////////////
QString RunProcess_data(QString cstring)
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


int dataDialog::returnval2() {
   return rval2;
}


QString dataDialog::externalLocation() {
     return external_Location;
}



dataDialog::dataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dataDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


    rval2=1;


    QString adbloc;

    QString tmpstr;
    QString commstr;
    QString cstr;
    QString argument;
    QString adb2;
    QString fline;



    if (os_data == 1)
       {
        tmpdir_data = "./";
         adb2 = tmpdir_data+"adb.exe";
       }


     if (os_data == 2)
       {
         tmpdir_data = QCoreApplication::applicationDirPath();
         tmpdir_data = tmpdir_data+"/adbfiles/";
         adb2 = tmpdir_data+"adb";
       }

     if (os_data == 0)
       {
         tmpdir_data = QCoreApplication::applicationDirPath();
         tmpdir_data = tmpdir_data+"/adbfiles/";
         adb2 = tmpdir_data+"adb";
       }



}







dataDialog::~dataDialog()
{
    delete ui;
}


void dataDialog::setadb_data(const QString &adb_data)
{

    QString command;
    QString cstring;
    QString mounted;

    adbloc=adb_data;

    //cstring=adbloc+" shell df | grep /storage";
    //mounted=RunProcess_data(cstring);

    cstring=adbloc+" shell /data/local/tmp/adblink/busybox find /storage/ -type d -maxdepth 2 -perm 0771";
    command=RunProcess_data(cstring);
    QStringList myStringList = command.split("\n");




    //QMessageBox::information(0,"",command);

    QFile file21(tmpdir_data+"temp.txt");



    if(!file21.open(QFile::WriteOnly))

      {
          QMessageBox::critical(this,"","Error creating file!");

          return;
      }


      QTextStream out1(&file21);
      out1  << command << endl;

      file21.flush();
      file21.close();





      QString tmpstr2 = tmpdir_data+"temp.txt";
      QString fline2;
      QFile file32(tmpstr2);

           if (!file32.open(QIODevice::ReadOnly | QIODevice::Text))
             {QMessageBox::critical(0,"","Error reading file!");
                return; }

           QTextStream in1(&file32);

            //ui->listDirectories1->addItem("/sdcard/");


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


                  ui->listDirectories1->addItem(fline2);

                 }

                  // i =  fline2.indexOf("\t");

                    //usbid =fline2.mid(0,i);



                }

            if(ui->listDirectories1->count()<=0)
              { ui->listDirectories1->addItem("No storage found");
                ui->listDirectories1->item(0)->setSelected(false);
                ui->dataButton1->setEnabled(false);
                ui->dataButton2->setEnabled(false);
                ui->dataButton3->setEnabled(false);
                ui->dataButton4->setEnabled(false);
                ui->okButton->setEnabled(false);
                return;
               }
            else
             { ui->listDirectories1->item(0)->setSelected(true);
                external_Location=ui->listDirectories1->item(0)->text();
            }




                 file32.close();
                  QFile::remove(tmpstr2);

            }




void dataDialog::setadb_dir(const QString &adb_dir)
{

    adbdir_data_data=adb_dir;





}

void dataDialog::on_dataButton1_clicked()
{
    rval2 = 1;
}

void dataDialog::on_dataButton2_clicked()
{
    rval2 = 2;
}

void dataDialog::on_dataButton3_clicked()
{
    rval2 = 3;
}

void dataDialog::on_dataButton4_clicked()
{
    rval2 = 4;
}


void dataDialog::on_listDirectories1_clicked(const QModelIndex &index)
{

 //  external_Location=ui->listDirectories1->currentItem()->text();

 external_Location = index.data(Qt::DisplayRole).toString();
}
