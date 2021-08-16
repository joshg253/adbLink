#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
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


#ifdef Q_OS_LINUX
 int os_pref=0;
#elif defined(Q_OS_WIN)
  int os_pref=1;
#elif defined(Q_OS_MAC)
int os_pref=2;
#endif


QString pdir = "";
QString version2;
int rval1 = 0;

QString packagepreset1 = "org.xbmc.kodi";
QString packagepreset2 = "org.xbmc.xbmc";
QString packagepreset3 = "com.semperpax.spmc16";
QString homedir;
QString tmpdir1;
QString ostype1;

QString xpackage;
QString adbdir_pref;
QString adb_pref;
bool su_pref;

///////////////////////////////////////////////
QString RunProcess_pref(QString cstring)
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

////////////////////////////////////////////////
 QString strip2 (QString str)
{
    str = str.simplified();
    str.replace( " ", "" );
    return str;
}
//////////////////////////////////
bool isConnectedToNetwork2()
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;

    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        if ( iface.flags().testFlag(QNetworkInterface::IsUp)
             && !iface.flags().testFlag(QNetworkInterface::IsLoopBack) )
        {


            // this loop is important
            for (int j=0; j<iface.addressEntries().count(); j++)
            {

                // we have an interface that is up, and has an ip address
                // therefore the link is present

                // we will only enable this check on first positive,
                // all later results are incorrect

                if (result == false)
                    result = true;
            }
        }

    }

    return result;
}




///////////////////////////////////////////////////
preferencesDialog::preferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preferencesDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if (os_pref == 1)
            {
             adbdir_pref = "./";
            }

       if (os_pref == 2)

            {
               adbdir_pref = QCoreApplication::applicationDirPath();
               adbdir_pref = adbdir_pref+"/adbfiles/";
            }

       if (os_pref == 0)
            {
               adbdir_pref = QCoreApplication::applicationDirPath();
               adbdir_pref = adbdir_pref+"/adbfiles/";
            }

       on_isusb_clicked(ui->isusb->isChecked());




}

preferencesDialog::~preferencesDialog()
{
    delete ui;
}



///////////////////////////////////////////////////
QString preferencesDialog::xbmcpackageName() {
   return ui->packagename->text();
}
///////////////////////////////////////////////////
QString preferencesDialog::data_root() {
    return ui->data_root->text();
}
///////////////////////////////////////////////////
QString preferencesDialog::pulldir() {
   return ui->pulldir->text();
}
///////////////////////////////////////////////////
QString preferencesDialog::daddr() {
   return ui->daddr->text();
}

///////////////////////////////////////////////////
QString preferencesDialog::description() {
   return ui->description->text();
}
///////////////////////////////////////////////////
QString preferencesDialog::filepath() {
   return ui->filepath->text();
}

///////////////////////////////////////////////////
QString preferencesDialog::port() {
   return ui->port->text();
}


///////////////////////////////////////////////////
bool preferencesDialog::isusb() {
   return ui->isusb->isChecked();

}

///////////////////////////////////////////////////
QString preferencesDialog::ostype() {
 QString ost= QString::number(ui->ostypeBox->currentIndex());
 return ost;
}

///////////////////////////////////////////////////
void preferencesDialog::setPackagename(const QString &packagename)
{

    bool isset=false;

    ui->packagename->setText(packagename);



    if (packagename == packagepreset1)
    {  ui->kodiButton->setChecked(true);
        isset=true;
    }


     if (packagename == packagepreset3)
        { ui->spmcButton->setChecked(true);
         isset=true;
        }



     if (!isset)
       ui->otherButton->setChecked(true);

}

///////////////////////////////////////////////////
void preferencesDialog::setPulldir(const QString &pulldir)
{
    ui->pulldir->setText(pulldir);
}



////////////////////////////////////////////////////////
void preferencesDialog::setisusb(const bool &isusb)
{
    ui->isusb->setChecked(isusb);
}


///////////////////////////////////////////////////
void preferencesDialog::setversionLabel(const QString &versiontext)
{
    ui->versionLabel->setText("adbLink version: "+ versiontext);
    version2 = versiontext;
}

///////////////////////////////////////////////////
void preferencesDialog::setostype(const QString &ostype)
{
    ui->ostypeBox->setCurrentIndex(ostype.toInt());


}


///////////////////////////////////////////////////
void preferencesDialog::setdevicelist(const QStringList &dstringlist)
{

    if(dstringlist.count() <1)
     return;

    QString tmpstr;


    for (QStringList::const_iterator it = dstringlist.begin();
            it != dstringlist.end(); ++it)
           {
              tmpstr = *it;
              ui->listDevicesp->addItem(tmpstr);
           }
}



///////////////////////////////////////////////////
void preferencesDialog::setdaddr(const QString &daddr)
{
    ui->daddr->setText(daddr);

}

///////////////////////////////////////////////////
void preferencesDialog::setdescription(const QString &description)
{
    ui->description->setText(description);

}


///////////////////////////////////////////////////
void preferencesDialog::setfilepath(const QString &filepath)
{
    ui->filepath->setText(filepath);

}

///////////////////////////////////////////////////
void preferencesDialog::setdataroot(const QString &data_root)
{
    ui->data_root->setText(data_root);

    if (ui->ostypeBox->currentIndex()==0)
    {
    //if(ui->data_root->text().isEmpty())
    //   ui->data_root->setText("/sdcard/");
    }

}

///////////////////////////////////////////////////
void preferencesDialog::setport(const QString &port)
{
    ui->port->setText(port);

}

////////////////////////////////////
int preferencesDialog::returnval1() {
   return rval1;
}


/////////////////////////////////////////////////////
void preferencesDialog::on_pfolderButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose pull folder"),
                                                 ui->pulldir->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty() )
    {
        ui->pulldir->setText(dir);
    }
}



///////////////////////////////////////////////
void preferencesDialog::on_pushButton_clicked()
{

  int passval=0;




  if (ui->description->text().isEmpty())
   {
    QMessageBox::information(0, "","Description is required",QMessageBox::Cancel);
   }
   else
   {
    passval = passval+1;
   }


   if (ui->ostypeBox->currentIndex() == 0)
    {
       if (ui->daddr->text().isEmpty())
         {
           QMessageBox::information(0, "","Device address is required",QMessageBox::Cancel);
          }
        else
         {
           passval=passval+1;
         }
      }
     else {
            passval=passval+1;
      }


   if (ui->ostypeBox->currentIndex() == 0)
    {

       if (ui->packagename->text().isEmpty())
       {
        QMessageBox::information(0, "","Package name is required.\nFill in or use preset",QMessageBox::Cancel);
        }
      else
       {
         passval=passval+1;
        }
     }

   else {
          passval=passval+1;
    }




   if (ui->ostypeBox->currentIndex() == 0)
    {

       if (ui->filepath->text().isEmpty())
       {
        QMessageBox::information(0, "","Filepath is required.\nFill in or use preset",QMessageBox::Cancel);
        }
      else
       {
         passval=passval+1;
        }
     }

   else {
          passval=passval+1;
    }





   if (passval==4)
   {
    rval1 = 1;
    QDialog::accept();
   }


}




///////////////////////////////////////////////

void preferencesDialog::onRequestCompleted() {
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






//////////////////////////////////////////////////////
void preferencesDialog::on_kodiButton_clicked()
{
    ui->packagename->setText("org.xbmc.kodi");
    ui->filepath->setText("/files/.kodi");
}


void preferencesDialog::on_spmcButton_clicked()
{
    ui->packagename->setText("com.semperpax.spmc16");
    ui->filepath->setText("/files/.spmc");
}

void preferencesDialog::on_otherButton_clicked()
{
    ui->packagename->setText("");
    ui->filepath->setText("");
}


void preferencesDialog::on_isusb_clicked(bool checked)
{
    if (checked)
    {
        ui->port->setText("");

    }

      else
    {
        ui->port->setText("5555");

    }
}




////////////////////////////////////////////////////////////
void preferencesDialog::setadb_pref(const QString &adb_pref)
{


    QString command;
    QString cstring;
    QString mounted;
    QString fline;
    QString tmpdir_pref;


    ui->listkodirootBox->addItem("/sdcard/");

    //QMessageBox::information(0,"",adb_pref);

    cstring=adb_pref+" shell /data/local/tmp/adblink/busybox which su";

    command=RunProcess_pref(cstring);

    if (command.contains("su"))
        su_pref=true;
      else su_pref=false;

   // if (adb_pref.contains(":5555"))
    //    ui->isusb->setEnabled(false);

    cstring=adb_pref+" shell /data/local/tmp/adblink/busybox find /storage/ -type d -maxdepth 2 -perm 0771";
    command=RunProcess_pref(cstring);

    if (command.contains("error:"))
         return;


    //QMessageBox::information(0,"",command);

    QFile file21(adbdir_pref+"temp.txt");



    if(!file21.open(QFile::WriteOnly))

      {
          QMessageBox::critical(this,"","Error creating file!");

          return;
      }


      QTextStream out1(&file21);
      out1  << command << endl;

      file21.flush();
      file21.close();





      QString tmpstr2 = adbdir_pref+"temp.txt";
      QString fline2;
      QFile file32(tmpstr2);

           if (!file32.open(QIODevice::ReadOnly | QIODevice::Text))
             {QMessageBox::critical(0,"","Error reading file!");
                return; }

           QTextStream in1(&file32);




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


                  ui->listkodirootBox->addItem(fline2+"/");

                 }

                  // i =  fline2.indexOf("\t");

                    //usbid =fline2.mid(0,i);



                }


              ui->listkodirootBox->item(0)->setSelected(true);

                 file32.close();
                  QFile::remove(tmpstr2);

            }




///////////////////////////////////////////////////
void preferencesDialog::on_listkodirootBox_clicked()
{
    ui->data_root->setText(ui->listkodirootBox->currentItem()->text());
}




////////////////////////////////////////////////////////////////////
void preferencesDialog::disable_ui()
{
    ui->port->setText("");
    ui->port->setEnabled(false);
    ui->isusb->setEnabled(false);
    ui->listkodirootBox->setEnabled(false);

    ui->daddr->setText("");
    ui->packagename->setText("");
    ui->data_root->setText("");

    ui->daddr->setEnabled(false);
    ui->packagename->setEnabled(false);
    ui->data_root->setEnabled(false);

    ui->mediaBox->setDisabled(true);


}



////////////////////////////////////////////////////////////////////
void preferencesDialog::enable_ui()
{
    ui->port->setEnabled(true);  
    ui->port->setText("5555");
    ui->isusb->setEnabled(true);
    ui->listkodirootBox->setEnabled(true);

    ui->daddr->setEnabled(true);
    ui->packagename->setEnabled(true);
    ui->data_root->setEnabled(true);

     ui->mediaBox->setEnabled(true);

}

////////////////////////////////////////////////////////////////////
void preferencesDialog::on_ostypeBox_currentIndexChanged(int index)
{
    switch(index ){
        case 0:

        enable_ui();

        if (ui->kodiButton->isChecked())
           { ui->packagename->setText("org.xbmc.kodi");
             ui->filepath->setText("/files/.kodi");
           }


        if (ui->spmcButton->isChecked())
           { ui->packagename->setText("com.semperpax.spmc16");
             ui->filepath->setText("/files/.spmc");
           }


        if (ui->otherButton->isChecked())
           { ui->packagename->setText("");
             ui->filepath->setText("");
           }

        if(ui->data_root->text().isEmpty())
           ui->data_root->setText("/sdcard/");


        break;

        case 1:

        //  Windows
        // C:\Users\USERNAME\AppData\Roaming\Kodi

        ui->daddr->setText("");
        ui->packagename->setText("");
        ui->data_root->setText("");



        homedir = QDir::homePath();
        tmpdir1 = homedir+ "/AppData/Roaming/Kodi";
        ui->filepath->setText(tmpdir1);

         disable_ui();

        break;

        case 2:
         //
        // /Users/USERNAME/Library/Application Support/Kodi
        homedir = QDir::homePath();
        tmpdir1 = homedir +"/Library/Application Support/Kodi";
        ui->daddr->setText("");
        ui->packagename->setText("");
        ui->data_root->setText("");
        ui->filepath->setText(tmpdir1);

        disable_ui();

        break;

        case 3:
        //

        // /home/USERNAME/.kodi
        homedir = QDir::homePath();
        tmpdir1 = homedir +"/.kodi";
        ui->daddr->setText("");
        ui->packagename->setText("");
        ui->data_root->setText("");
        ui->filepath->setText(tmpdir1);

        disable_ui();

        break;
        }
}


////////////////////////////////////////////////////
void preferencesDialog::on_filepathButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose file path"),
                                                 ui->filepath->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty() )
    {
        ui->filepath->setText(dir);
    }
}


void preferencesDialog::on_listDevicesp_doubleClicked(const QModelIndex &index)
{
    ui->daddr->setText( index.data(Qt::DisplayRole).toString());

}




bool preferencesDialog::disableroot() {
   return ui->disableroot->isChecked();
}



void preferencesDialog::setdisableroot(const bool &disableroot)
{
    ui->disableroot->setChecked(disableroot);
}
