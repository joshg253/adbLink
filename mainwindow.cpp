#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog2.h"
#include "helpdialog.h"
#include "uninstalldialog.h"
#include "editordialog.h"
#include "keyboarddialog.h"
#include "usbfiledialog.h"
#include "downloaddialog.h"
#include "listfiledialog.h"
#include "kodidialog.h"
#include "cachedialog.h"
#include "datadialog.h"
#include "backupdialog.h"
#include "forcequitdialog.h"
#include "restdialog.h"
#include "klogdialog.h"
#include "tcpipdialog.h"
#include "oslogdialog.h"
#include "adbprefdialog.h"
#include "sleepdialog.h"

#ifdef __WIN32__
  #include "windows.h"
#endif


#include <QtConcurrent/QtConcurrent>
#include <qtconcurrentrun.h>
#include <QMessageBox>
#include <QTableWidget>
#include <QResource>
#include <QProcess>
#include <QDir>
#include <QFileDialog>
#include <QThread>
#include <QFile>
#include <QProgressBar>
#include <QTimer>
#include <QLabel>
#include <QInputDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <preferencesdialog.h>
#include <QElapsedTimer>
#include <QTextStream>
#include <QStringRef>
#include <QDate>
#include <QRegularExpression>
#include <QStringList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>
#include <QIcon>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <QtNetwork/QNetworkInterface>
#include <adblogdialog.h>
#include <QFileInfo>
#include <QThread>
#include <QFileInfo>
#include<QClipboard>
#include <QShortcut>


#ifdef Q_OS_LINUX
 int os=0;
#elif defined(Q_OS_WIN)
  int os=1;
#elif defined(Q_OS_MAC)
int os=2;
#endif



//const QString vqurl = "http://www.elkins.org/version.txt";
const QString vqurl = "http://www.jocala.com/version.txt";

QRegularExpression validip( "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");

const QString version = "4.3";


bool isConnected = false;
bool serverRunning = false;
bool is_packageInstalled = false;


bool dbexists = false;
bool updatecheck = true;
bool versioncheck = true;
bool isusb = false;
bool disableroot = false;

QString port = "5555";
QString filename = "";
QString adbdir =  "";
QString adb = "";
QString aapt = "";
QString fastboot = "";
QString xmldir = "";
QString splashdir = "";



QString hdir = "";
QString daddr="";
QString sldir = "";
QString pushdir = "";
QString pulldir = "";
QString xbmcpackage ="";
QString ostype = "";
QString data_root = "";
QString buffersize = "";
QString bufferfactor = "";
QString dbstring = "";
QString description = "";
QString filepath = "";
QString busypath = "";
QString adblog;
QStringList bufferlist;


QString adbstr1 = "ADB running. ";
QString adbstr2 = "ADB stopped. ";
QString devstr1 = "  Current device connected";
QString devstr2 = "Selected device not connected.";
//QString devstr3 = "  USB Mode";
QString program = "adbLink";
QString tempdir = "/data/local/tmp/";
QString logfiledir;
QString databasedir;
QString downloaddir;

int usbcheck;
int rootpath;
int ftvupdate;
int checkversion;
int extdata;
int buffermode = 1;
int tsvalue = 4000;
int ct = 1;
int Id = 0;
int deviceboxindex = 0;

QFutureWatcher<void> watcher1;
QFutureWatcher<void> watcher2;

QSqlDatabase db;



//////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{




  if (os == 1)
     {
         adbdir = "./";
         adb = adbdir+"adb.exe";
         adb = '"'+adb+'"';

         if (!QFile::exists(adbdir+"adb.exe"))
         {
             QMessageBox::critical(0, "","adb binary missing!\n",QMessageBox::Cancel);
         }


          QString databasedir= QDir::homePath()+"/AppData/Roaming/.jocala/";
          QDir dir(databasedir);

         if (!dir.exists())
          {
             dir.mkpath(".");
           }


         dbstring = databasedir+"adblink.db";
         logfiledir=databasedir;

         if(QFileInfo(adbdir+"adblink.db").exists())
          QFile::copy(adbdir+"adblink.db", dbstring);
      }



   if ( (os == 2)  || (os == 0) )

        {

           adbdir = QCoreApplication::applicationDirPath();
           adbdir = adbdir+"/adbfiles/";
           databasedir = QDir::homePath()+"/.jocala/";



           QDir dir(databasedir);

          if (!dir.exists())
           {
              dir.mkpath(".");
            }


           dbstring = databasedir+"adblink.db";
           logfiledir=databasedir;


           if (QFile::exists(adbdir+"adb"))
           {
               adb = adbdir+"adb";
               adb = '"'+adb+'"';
               QProcess::execute ("chmod 0755 "+ adb);
           }

           else
               QMessageBox::critical(0, "","adb binary missing!\n",QMessageBox::Cancel);


           if (QFile::exists(adbdir+"fastboot"))
           {
               fastboot = adbdir+"fastboot";
               fastboot = '"'+fastboot+'"';
               QProcess::execute ("chmod 0755 "+ fastboot);
           }



   }




       xmldir = adbdir+"remotes/";
       splashdir = adbdir+"splash/";


     ui->setupUi(this);
     setFixedSize(size());



#ifdef Q_OS_MAC
  //https://bugreports.qt.io/browse/QTBUG-51120
  ui->progressBar->setTextVisible(true);
#endif //Q_OS_MAC



     ui->statusBar->addPermanentWidget(ui->server_running);
     ui->statusBar->addPermanentWidget(ui->progressBar);
     ui->progressBar->setHidden(true);


     QPixmap pix(":/assets/donatel.png");
     QIcon icon(pix);
     ui->donate->setIcon(icon);
     ui->donate->setText("");
     ui->donate->setIconSize(pix.size());



     ui->server_running->setText(adbstr2);


  rotate_logfile();


  logfile("adbLink "+version);

  QDateTime dateTime = QDateTime::currentDateTime();
  QString dtstr = dateTime.toString("MMddyyhhmmss");
  logfile(dtstr);




  QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int nIter=0; nIter<list.count(); nIter++)

     {
         if(!list[nIter].isLoopback())
             if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
           logfile("IP: "+list[nIter].toString());

     }






  if (os == 1)
          {
          logfile("Windows");
          }

     if (os == 2)

          {
        logfile("OS X");
          }



     if (os == 0)

          {
        logfile("Linux");

          }

 logfile("------------");


  hdir = QDir::homePath();


  if(QFileInfo(dbstring).exists())
     dbexists = true;
  else
      dbexists = false;



  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbstring);


  if (!db.open()) {
      QMessageBox::critical(0, qApp->tr("Cannot open database"),
     "Database error:\n"+dbstring, QMessageBox::Cancel);
     logfile("error opening database "+dbstring);
  }




  if (!dbexists)
   { createTables();
      createPrefTables();
    }


   if (!db.tables().contains(QLatin1String("preferences")) )
    examinedatabase();


   add_disableroot();


   loaddevicebox();

 connect(&watcher1, SIGNAL(finished()), SLOT(finishedCopy1()));
 connect(&watcher2, SIGNAL(finished()), SLOT(finishedCopy2()));

 ui->clearButton->setShortcut(QKeySequence("Ctrl+Y"));

 ui->newRecord->setShortcut(QKeySequence("Ctrl+B"));
 ui->editRecord->setShortcut(QKeySequence("Ctrl+C"));
 ui->delRecord->setShortcut(QKeySequence("Ctrl+D"));
 ui->connButton->setShortcut(QKeySequence("Ctrl+E"));
 ui->disButton->setShortcut(QKeySequence("Ctrl+F"));

ui->fmButton->setShortcut(QKeySequence("Ctrl+G"));
ui->adbshellButton->setShortcut(QKeySequence("Ctrl+I"));
ui->backupButton->setShortcut(QKeySequence("Ctrl+J"));
ui->restoreButton->setShortcut(QKeySequence("Ctrl+K"));


ui->sideload_Button->setShortcut(QKeySequence("Ctrl+L"));
ui->uninstall_Button->setShortcut(QKeySequence("Ctrl+M"));
ui->mvdataButton->setShortcut(QKeySequence("Ctrl+N"));
ui->editButton->setShortcut(QKeySequence("Ctrl+O"));


ui->refreshConnectedDevices->setShortcut(QKeySequence("Ctrl+P"));
ui->killServer->setShortcut(QKeySequence("Ctrl+Z"));
ui->pushRemote->setShortcut(QKeySequence("Ctrl+R"));
ui->cacheButton->setShortcut(QKeySequence("Ctrl+S"));

ui->doConsole->setShortcut(QKeySequence("Ctrl+T"));
ui->keypadButton->setShortcut(QKeySequence("Ctrl+U"));
ui->splashButton->setShortcut(QKeySequence("Ctrl+V"));
ui->screencapButton->setShortcut(QKeySequence("Ctrl+W"));


    start_server();
    logfile("starting server");
    if (serverRunning)
     { ui->server_running->setText(adbstr1);
        }

    else
    { ui->server_running->setText(adbstr2);

    }




    on_refreshConnectedDevices_clicked();



   if (getadbPref())
          get_data();

    downloaddir=getdownloadpath();


 }


MainWindow::~MainWindow()
{



    logfile("closing program");
    kill_server();
    db.close();
     delete ui;


}



/////////////////////////////////////////////////////
void MainWindow::TimerEvent()
{
  int value = ui->progressBar->value();

  if (value >= 100)
      {
         value = 0;
         ui->progressBar->reset();
     }

  ui->progressBar->setValue(value+1);



}




/////////////////////////////////////////////////

QString MainWindow::getDescription(QString mdaddr)
{



    QSqlQuery query;
    QString sqlstatement;
    QString quote = "\"";
    QString descrip="";
    mdaddr = quote+mdaddr+quote;

    sqlstatement= "SELECT description FROM device WHERE daddr=" + mdaddr;
   // query.exec(sqlstatement);
  //  logfile(sqlstatement);

    query.prepare( sqlstatement );

    if (!query.exec()){
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
    }
    else
    {
        if( query.next( ) )
          descrip = query.value(0).toString();
    }


return descrip;

}




/////////////////////////////////////////////////

bool MainWindow::record_exists(QString descrip)
{


    int record_is=0;
    QSqlQuery query;
    QString sqlstatement;
    QString quote = "\"";

    descrip = quote+descrip+quote;

sqlstatement="SELECT EXISTS(SELECT 1 FROM device WHERE description=";
sqlstatement=sqlstatement+descrip+" LIMIT 1)";

    query.prepare( sqlstatement );

    if (!query.exec()){
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
    }
    else
    {
        if( query.next( ) )
           record_is = query.value(0).toInt();
    }



if(record_is==1)
return true;
else
return false;

}



///////////////////////////////////////////////
QString MainWindow::RunProcess(QString cstring)
{
 QProcess run_command;
 run_command.setProcessChannelMode(QProcess::MergedChannels);
 run_command.start(cstring);

 run_command.waitForStarted();

 // int zzz = run_command.processId();

 while(run_command.state() != QProcess::NotRunning)
     qApp->processEvents();

 QString command=run_command.readAll();

 return command;
}


/*

//////////////////////////////////////////////
void MainWindow::adberror()
{
stderrtxt=run_command.readAllStandardError();

if (!stderrtxt.contains("error: device not found"))
   {   logfile("QPROCESS : "+stderrtxt );
}

}


*/

/*

//////////////////////////////////////////////
void MainWindow::notAndroid()

{

  return;

    ui->sideload_Button->setDisabled(true);
    ui->uninstall_Button->setDisabled(true);
    ui->mvdataButton->setDisabled(true);
    ui->adbshellButton->setDisabled(true);
    ui->screencapButton->setDisabled(true);


//    ui->connButton->setDisabled(true);
 //   ui->disButton->setDisabled(true);


    ui->fpushButton->setDisabled(true);
    ui->fpullButton->setDisabled(true);
    ui->fdellButton->setDisabled(true);
    ui->fclearButton->setDisabled(true);
    ui->comboBox->setDisabled(true);
    ui->customdir->setDisabled(true);
    ui->editButton->setDisabled(true);
}


//////////////////////////////////////////////
void MainWindow::isAndroid()

{

    ui->sideload_Button->setDisabled(false);
    ui->uninstall_Button->setDisabled(false);
    ui->mvdataButton->setDisabled(false);
    ui->adbshellButton->setDisabled(false);
    ui->screencapButton->setDisabled(false);


    ui->connButton->setDisabled(false);
    ui->disButton->setDisabled(false);


    ui->fpushButton->setDisabled(false);
    ui->fpullButton->setDisabled(false);
    ui->fdellButton->setDisabled(false);
    ui->fclearButton->setDisabled(false);
    ui->comboBox->setDisabled(false);
    ui->editButton->setDisabled(false);

     ui->customdir->setDisabled(false);

}

*/


//////////////////////////////////////////////
QString MainWindow::getadb()

{
    QString gadb="";
    QString editport="";

    if (!isusb)
        editport=":"+port;

    gadb=adb+" -s "+daddr+editport+" ";


    return gadb;

}


/////////////////////////////////////////////

bool MainWindow::fileExists(QString path) {
    QFileInfo checkFile(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

/////////////////////////////////////////
void MainWindow::delayTimer(int rdelay)
{

 QElapsedTimer rtimer;

int nMilliseconds;
 int i = 0;

 rtimer.start();

while(i == 0)
  {
    qApp->processEvents();
     nMilliseconds = rtimer.elapsed();
   if (nMilliseconds >= rdelay)
       break;
}


}


//////////////////////////////////////////////
void MainWindow::rotate_logfile()

{

 QFile file(logfiledir+"adblink.old.log");

 if( file.exists() )
     QFile::remove(logfiledir+"adblink.old.log");


QFile file2(logfiledir+"adblink.log");

if( file2.exists() )
    file2.rename(logfiledir+"adblink.old.log");


}


//////////////////////////////////////////////
void MainWindow::logfile(QString line)

{


QFile file(logfiledir+"adblink.log");
   if(!file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
      {
       QMessageBox::critical(0, "","Can't create logfile!\n",QMessageBox::Cancel);
       return;
      }

   QTextStream out(&file);
            out  << line << endl;

 }


/////////////////////////////////////////
void MainWindow::rebootDevice(QString reboot)
{

 QElapsedTimer rtimer;
 int nMilliseconds;

QProcess reboot_device;
rtimer.start();
reboot_device.setProcessChannelMode(QProcess::MergedChannels);

QString cstring = getadb() + " " + reboot;

reboot_device.start(cstring);
reboot_device.waitForStarted();
while(reboot_device.state() != QProcess::NotRunning)
  {
    qApp->processEvents();
     nMilliseconds = rtimer.elapsed();
   if (nMilliseconds >= 5000)
       break;
}


}


/////////////////////////////////////////////////////
bool MainWindow::mount_system(QString mnt)
{



    QString cstring;
    QString command;


    cstring = getadb() + " shell /data/local/tmp/adblink/which su";
    command=RunProcess(cstring);



      cstring = getadb() + " shell su -c /data/local/tmp/adblink/mount  -o "+ mnt + ",remount /system";
      command=RunProcess(cstring);



        if (command.isEmpty())
          {
           // QMessageBox::information(this,"","/system partition mounted "+mnt);
           // logfile("/system mounted "+mnt);
           // logfile(cstring);
           // logfile(command);
            return true;
          }
            else
          {
            // QMessageBox::critical(this,"","Unknown error, see log");
            // logfile("/system mount error:");
            // logfile(cstring);
           // logfile(command);
            return false;
          }

}



/////////////////////////////
bool MainWindow::is_su()
{

QString cstring = getadb() + " shell /data/local/tmp/adblink/which su";
QString command=RunProcess(cstring);



if (!command.contains("su"))
  {

    return false;
   }
    else
  {
     return true;
   }


}



////////////////////////////////////////////////
 QString MainWindow::strip (QString str)
{
    str = str.simplified();
    str.replace( " ", "" );
    return str;
}




 ////////////////////////////////////////////

 void MainWindow::default_device_values()
 {
     ostype="0";
     sldir = QDir::homePath();
     pushdir = QDir::homePath();

     pulldir = getdownloadpath();
     filepath = "/files/.kodi";
     buffermode = 1;
     buffersize = "20971520";
     bufferfactor = "1";
     versioncheck=true;
     xbmcpackage = "org.xbmc.kodi";
     data_root="/sdcard/";
 }





 ////////////////////////////////////////////

 void MainWindow::blank_entry_form()
 {

     daddr="";
     ostype="0";
     description="";
     pulldir = "";
     isusb = false;
     filepath = "/files/.kodi";
     port = "5555";
     buffermode = 1;
     buffersize = "20971520";
     bufferfactor = "1";
     xbmcpackage = "org.xbmc.kodi";
     data_root="/sdcard/";
    disableroot = false;

 }


 //////////////////////////////////////////
 void MainWindow::kill_server()
{

 QString cstring = adb + " kill-server";
 QString command=RunProcess(cstring);
 // logfile(cstring);
 logfile("server stopped");
 serverRunning = false;

}


//////////////////////////////////////
void MainWindow::start_server()
{

  //QString cstring;
  //QString command;

    QString cstring = adb + " kill-server";
    QString command=RunProcess(cstring);

      cstring = adb + " start-server";
      command=RunProcess(cstring);

     if (command.contains("daemon started successfully"))
        {
           logfile("server started");
          // logfile(cstring);
           logfile(command);
           serverRunning = true;
          }

         else
        {
         logfile("start-server failed!");
         // logfile(cstring);
         logfile(command);
         serverRunning = false;
         }
}



/////////////////////////////////////////////////////
bool MainWindow::is_package(QString package)
{

    QString editport = ":"+port;

    if (isusb)
    editport = "";




    QString cstring = getadb() + " -s "+daddr+editport+" shell pm list packages ";
    QString command=RunProcess(cstring);
    logfile ("package: "+cstring);

        if (command.contains(package))
            {
            logfile(package+ " is installed");
            // logfile(cstring);
            // logfile(command);
            is_packageInstalled = true;
            }
            else
            {
            logfile(package+ " not found");
            // logfile(cstring);
            // logfile(command);
            is_packageInstalled = false;}

        return  is_packageInstalled;
}




//////// DATABASE ///////////////////

////////////////////////////////
int MainWindow::getRows()
{

   int rowcount=0;
   QString sqlstatement = "SELECT Count(*) FROM device";

    QSqlQuery query;
    query.exec(sqlstatement);
    while (query.next()) {
          rowcount = query.value(0).toInt();
    }

    if (query.lastError().isValid())
     {
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
       }


    return rowcount;
}




////////////////////////////////

void MainWindow::createTables()
{
    logfile("creating adblink.db");



    QString sqlstatement="CREATE TABLE device (Id INTEGER PRIMARY KEY, daddr text,description text NOT NULL UNIQUE,pulldir text,xbmcpackage text,data_root text,buffermode int, buffersize text, bufferfactor text, filepath text, port text,isusb int, ostype text, disableroot int)";


    QSqlQuery query;
    query.exec(sqlstatement);

    if (query.lastError().isValid())
     {
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
       }




}




////////////////////////////////

void MainWindow::createPrefTables()
{
    logfile("creating preference table");

     QSqlQuery query;
    QString sqlstatement="CREATE TABLE preferences (Id INTEGER PRIMARY KEY, checkversion int, msgboxtype int, downloaddir text)";


    query.exec(sqlstatement);

   if (query.lastError().isValid())
    {
       logfile(sqlstatement);
       logfile("SqLite error:" + query.lastError().text());
       logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
      }

   QString vcheck="1";
   QString mcheck="0";
   QString ddir=QDir::homePath();

   sqlstatement = "INSERT INTO preferences(checkversion,msgboxtype,downloaddir) VALUES  ('"+vcheck+"', '"+mcheck+"','"+ddir+"' ) ";

         query.exec(sqlstatement);

         if (query.lastError().isValid())
          {
            logfile(sqlstatement);
            logfile("SqLite error:" + query.lastError().text());
            logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
           }



}




////////////////////////////////


////////////////////////////////

void MainWindow::insertDevice()
{

   logfile("Insert new record");

   QSqlQuery query;

    daddr.replace(QString("'"), QString("''"));
    description.replace(QString("'"), QString("''"));
    xbmcpackage.replace(QString("'"), QString("''"));
    pulldir.replace(QString("'"), QString("''"));
    xbmcpackage.replace(QString("'"), QString("''"));
    filepath.replace(QString("'"), QString("''"));
    port.replace(QString("'"), QString("''"));
    data_root.replace(QString("'"), QString("''"));
    ostype.replace(QString("'"), QString("''"));

    QString ckversion;
    QString buffermd;
    QString usbflag;
    QString rootflag;

    buffermd.setNum(buffermode);
    usbflag.setNum(isusb);
    rootflag.setNum(disableroot);

QString sql1  = "INSERT INTO device( daddr, description, pulldir, xbmcpackage, data_root, buffermode, buffersize, bufferfactor, filepath , port ,isusb,ostype,disableroot)";
QString sql2 = " VALUES  ('"+daddr+"', '"+description+"', '"+pulldir+"', '"+xbmcpackage+"','"+data_root+"',  '"+buffermd+"',  '"+buffersize+"',  '"+bufferfactor+"', '"+filepath+"', '"+port+"','"+usbflag+"','"+ostype+"','"+rootflag+"')";

QString sqlstatement = sql1+sql2;

      query.exec(sqlstatement);

      if (query.lastError().isValid())
       {
         logfile(sqlstatement);
         logfile("SqLite error:" + query.lastError().text());
         logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
        }


}



////////////////////////////////
void MainWindow::updateRoot()
{
logfile("updateRoot()");


    logfile("updating database adblink.db");

    QString sqlstatement;
    QString tempstring;

    QString idstring;
    idstring.setNum(Id);
    logfile(idstring);
    QSqlQuery query;

    QString quote = "\"";



     tempstring = data_root;
     tempstring.replace(QString("'"), QString("''"));


      sqlstatement = "UPDATE device SET data_root='"+tempstring+"'  WHERE Id="+ idstring;
      query.exec(sqlstatement);

      if (query.lastError().isValid())
       {
         logfile(sqlstatement);
         logfile("SqLite error:" + query.lastError().text());
         logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
        }




}




//////////////////////////////////////////////

void MainWindow::updateDevice()
{
logfile("updateDevice()");


    logfile("updating database adblink.db");

    QString str1;
    QString str2;
    QString str3;
    QString str4;
    QString str5;
    QString str6;
    QString rootflag;

    QString tempstring;
    QString sqlstatement;

    str1.setNum(usbcheck);
    str2.setNum(ftvupdate);
    str3.setNum(checkversion);

    str4.setNum(buffermode);
    str5.setNum(isusb);
    rootflag.setNum(disableroot);



  //  int idstring = Id.t;
    QString idstring;
    idstring.setNum(Id);
    //logfile(idstring);
    QSqlQuery query;

    QString quote = "\"";




///////////////////////////////



   tempstring = description;
   tempstring.replace(QString("'"), QString("''"));

   sqlstatement = "UPDATE device SET description='"+tempstring+"'  WHERE Id="+ idstring;
   query.exec(sqlstatement);

   if (query.lastError().isValid())
    {
      logfile(sqlstatement);
      logfile("SqLite error:" + query.lastError().text());
      logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
     }




/////////////////////////

    tempstring = daddr;
    tempstring.replace(QString("'"), QString("''"));

    sqlstatement = "UPDATE device SET daddr='"+tempstring+"'  WHERE Id="+ idstring;
    logfile(sqlstatement);
    query.exec(sqlstatement);

     if (query.lastError().isValid())
      {
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
       }

//////////

     tempstring = pulldir;
     tempstring.replace(QString("'"), QString("''"));

     sqlstatement = "UPDATE device SET pulldir='"+tempstring+"'  WHERE Id="+ idstring;
     query.exec(sqlstatement);


     if (query.lastError().isValid())
      {
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
       }

///////////

     tempstring = xbmcpackage;
     tempstring.replace(QString("'"), QString("''"));

     sqlstatement = "UPDATE device SET xbmcpackage='"+tempstring+"'  WHERE Id="+ idstring;
     query.exec(sqlstatement);

     if (query.lastError().isValid())
      {
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
       }


/////////////

      tempstring = data_root;
      tempstring.replace(QString("'"), QString("''"));

      sqlstatement = "UPDATE device SET data_root='"+tempstring+"'  WHERE Id="+ idstring;
      query.exec(sqlstatement);


      if (query.lastError().isValid())
      {
         logfile(sqlstatement);
         logfile("SqLite error:" + query.lastError().text());
         logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
        }


/////////

      tempstring = str4;
      tempstring.replace(QString("'"), QString("''"));


       sqlstatement = "UPDATE device SET buffermode='"+tempstring+"'  WHERE Id="+ idstring;
       query.exec(sqlstatement);

       if (query.lastError().isValid())
        {
          logfile(sqlstatement);
          logfile("SqLite error:" + query.lastError().text());
          logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
         }

///////////////////////

       tempstring = buffersize;
       tempstring.replace(QString("'"), QString("''"));

       sqlstatement = "UPDATE device SET buffersize='"+tempstring+"'  WHERE Id="+ idstring;
        query.exec(sqlstatement);

        if (query.lastError().isValid())
         {
           logfile(sqlstatement);
           logfile("SqLite error:" + query.lastError().text());
           logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
          }

///////////

        tempstring = bufferfactor;
        tempstring.replace(QString("'"), QString("''"));

         sqlstatement = "UPDATE device SET bufferfactor='"+tempstring+"'  WHERE Id="+ idstring;
         query.exec(sqlstatement);

         if (query.lastError().isValid())
          {
            logfile(sqlstatement);
            logfile("SqLite error:" + query.lastError().text());
            logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
           }

 //////////////////

          tempstring = filepath;
          tempstring.replace(QString("'"), QString("''"));

           sqlstatement = "UPDATE device SET filepath='"+tempstring+"'  WHERE Id="+ idstring;
           query.exec(sqlstatement);

           if (query.lastError().isValid())
            {
              logfile(sqlstatement);
              logfile("SqLite error:" + query.lastError().text());
              logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
             }


  /////////////////

           tempstring = port;
           tempstring.replace(QString("'"), QString("''"));

           sqlstatement = "UPDATE device SET port='"+tempstring+"'  WHERE Id="+ idstring;
           query.exec(sqlstatement);

           if (query.lastError().isValid())
            {
              logfile(sqlstatement);
              logfile("SqLite error:" + query.lastError().text());
              logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
             }


///////////////


           tempstring = str5;
           tempstring.replace(QString("'"), QString("''"));

           sqlstatement = "UPDATE device SET isusb='"+tempstring+"'  WHERE Id="+ idstring;
           query.exec(sqlstatement);

           if (query.lastError().isValid())
            {
              logfile(sqlstatement);
              logfile("SqLite error:" + query.lastError().text());
              logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
             }


 //////////////



           tempstring = rootflag;
           tempstring.replace(QString("'"), QString("''"));

           sqlstatement = "UPDATE device SET disableroot='"+tempstring+"'  WHERE Id="+ idstring;
           query.exec(sqlstatement);

           if (query.lastError().isValid())
            {
              logfile(sqlstatement);
              logfile("SqLite error:" + query.lastError().text());
              logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
             }


 //////////////










           tempstring = ostype;
           tempstring.replace(QString("'"), QString("''"));

           sqlstatement = "UPDATE device SET ostype='"+tempstring+"'  WHERE Id="+ idstring;
           query.exec(sqlstatement);

           if (query.lastError().isValid())
            {
              logfile(sqlstatement);
              logfile("SqLite error:" + query.lastError().text());
              logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
             }

/////////////

}





///////////////////////////////////////////////

void MainWindow::getRecord(QString descrip)

{


    QString sqlstatement;

    QString quote = "\"";

    descrip = quote+descrip+quote;
    QSqlQuery query;


   sqlstatement= "SELECT Id FROM device WHERE description=" + descrip;
   query.exec(sqlstatement);
   logfile(sqlstatement);


           while (query.next()) {
                  Id = query.value(0).toInt();
                }


             if (query.lastError().isValid())
              {
                logfile(sqlstatement);
                logfile("SqLite error:" + query.lastError().text());
                logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
               }



             sqlstatement= "SELECT daddr FROM device WHERE description=" + descrip;
             query.exec(sqlstatement);
             logfile(sqlstatement);


                     while (query.next()) {
                            daddr = query.value(0).toString();
                          }


                       if (query.lastError().isValid())
                        {
                          logfile(sqlstatement);
                          logfile("SqLite error:" + query.lastError().text());
                          logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
                         }






           sqlstatement="SELECT pulldir FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  pulldir = query.value(0).toString();
            }

        //    logfile(sqlstatement);
        //    logfile(pulldir);

            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }



            sqlstatement="SELECT xbmcpackage FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  xbmcpackage = query.value(0).toString();
            }

      //      logfile(sqlstatement);
      //      logfile(xbmcpackage);

            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }


            sqlstatement="SELECT data_root FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  data_root = query.value(0).toString();
            }


            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }




            sqlstatement="SELECT buffermode FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  buffermode = query.value(0).toInt();
            }

        //    logfile(sqlstatement);

            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }



            sqlstatement="SELECT buffersize FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  buffersize = query.value(0).toString();
            }


        //      logfile(sqlstatement);
         //     logfile(buffersize);

            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }


            sqlstatement="SELECT bufferfactor FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  bufferfactor = query.value(0).toString();
            }

        //    logfile(sqlstatement);
         //   logfile(bufferfactor);

            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }


            sqlstatement="SELECT description FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  description = query.value(0).toString();
            }

         //   logfile(sqlstatement);
         //   logfile(description);

            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }


            sqlstatement="SELECT filepath FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  filepath = query.value(0).toString();
            }

        //    logfile(sqlstatement);
        //    logfile(filepath);

            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }



            sqlstatement="SELECT port FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  port = query.value(0).toString();
            }

        //    logfile(sqlstatement);
        //    logfile(filepath);

            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }



            sqlstatement="SELECT isusb FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  isusb = query.value(0).toInt();
            }




            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }



            sqlstatement="SELECT disableroot FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  disableroot = query.value(0).toInt();
            }




            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }







            sqlstatement="SELECT ostype FROM device  WHERE description=" + descrip;
            query.exec(sqlstatement);
            while (query.next()) {
                  ostype = query.value(0).toString();

            //    if (ostype=="0")
            //        isAndroid();
             //     else
             //       notAndroid();

                //qDebug() << ostype;

            }

        //    logfile(sqlstatement);

            if (query.lastError().isValid())
             {
               logfile(sqlstatement);
               logfile("SqLite error:" + query.lastError().text());
               logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
              }





     if (sldir.isEmpty())
         sldir = hdir;

     if (pushdir.isEmpty())
      pushdir = hdir;

     if (pulldir.isEmpty())
      pulldir = getdownloadpath();

     if (xbmcpackage.isEmpty())
         xbmcpackage = "org.xbmc.kodi";

    // if (data_root.isEmpty())
    //     data_root = "/sdcard/";

     if (checkversion==0)
         versioncheck=false;
     else
         versioncheck=true;



}



//////////////////////////////////////////////

void MainWindow::deleteRecord(QString descrip)

{


  QString sqlstatement;
  logfile("getting database record");

  QString quote = "\"";

  descrip = quote+descrip+quote;
  QSqlQuery query;


   sqlstatement= "DELETE FROM device WHERE description=" + descrip;
   query.exec(sqlstatement);
   logfile(sqlstatement);


}


/////////////////////////////////////

void MainWindow::addcolumn1()
{
QString sqlstatement="SELECT rootpath FROM device ";
 QSqlQuery query;
 query.exec(sqlstatement);

 if (query.lastError().number() == 1)
  {
     sqlstatement = "ALTER TABLE device ADD COLUMN rootpath int";
     query.exec(sqlstatement);


     if (query.lastError().isValid())
      {
        logfile("rootpath column NOT added to device table");
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
        QMessageBox::critical(0,"","Can't alter databse. See log.");
     }
   else logfile("rootpath column added to device table");

 }

}


//////////////////////////////////

bool MainWindow::isConnectedToNetwork()
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




/////////////////////////////////////

void MainWindow::delay(int secs)
{
    QTime dieTime= QTime::currentTime().addSecs(secs);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


/////////////////////////////////////////////////////////////////////////
bool MainWindow::find_daddr(QString deviceid)
{
   bool usbdaddr = false;

   for(int row = 0; row < ui->listDevices->count(); row++)
   {
       if (ui->listDevices->item(row)->text()==deviceid)
           usbdaddr=true;
   }

    return usbdaddr;
}



//////////////////////////////////////////////////////////////////////
void MainWindow::examinedatabase() {

    QString sqlstatement;
    QStringList fieldlist;
    QStringList preflist;
    QSqlQuery query;



    // backup database
   // dbstring = databasedir+"adblink.db";


    QString bkdbstring = databasedir+"backup.db";

     if (!QDir(databasedir).exists())
          QDir().mkdir(databasedir);

         if(QFileInfo(dbstring).exists()){
            QFile::copy(dbstring, bkdbstring);
         }



       createPrefTables();



if (db.tables().contains("netshare") )
          {
       sqlstatement="DROP TABLE netshare";
       query.exec(sqlstatement);
       if(!query.isActive())
            {   logfile("SQLERROR: "+query.lastError().text());
                return; }
   }


query.exec("PRAGMA table_info(device)");
  while (query.next()) {
     fieldlist << query.value(1).toString();
   }



if(fieldlist.contains("sshpassword"))
{

 if (!db.tables().contains("temp") )
   {
    sqlstatement="CREATE TABLE temp (Id INTEGER PRIMARY KEY, daddr text,description text NOT NULL UNIQUE,pulldir text,xbmcpackage text,data_root text,buffermode int, buffersize text, bufferfactor text, filepath text, port text,isusb int, ostype text)";
    query.exec(sqlstatement);
    if(!query.isActive())
         {   logfile("SQLERROR: "+query.lastError().text());
             return; }



if(!fieldlist.contains("flag1"))
 {  sqlstatement="INSERT INTO temp(daddr ,description ,pulldir,xbmcpackage,data_root,buffermode, buffersize, bufferfactor, filepath, port,isusb) SELECT daddr,description ,pulldir,xbmcpackage,sshpassword,buffermode, buffersize, bufferfactor, filepath, port,isusb FROM device";

    query.exec(sqlstatement);
    if(!query.isActive())
         {   logfile("SQLERROR: "+query.lastError().text());
             return; }

  QString tmpstring="0";

    sqlstatement = "UPDATE temp SET ostype='"+tmpstring+"'";

    query.exec(sqlstatement);
    if(!query.isActive())
         {   logfile("SQLERROR: "+query.lastError().text());
             return; }
}

else

  { sqlstatement="INSERT INTO temp(daddr ,description ,pulldir,xbmcpackage,data_root,buffermode, buffersize, bufferfactor, filepath, port,isusb,ostype) SELECT daddr,description ,pulldir,xbmcpackage,sshpassword,buffermode, buffersize, bufferfactor, filepath, port,isusb,flag1 FROM device";
   query.exec(sqlstatement);
   if(!query.isActive())
        {   logfile("SQLERROR: "+query.lastError().text());
            return; }

 }



 sqlstatement="DROP TABLE device";
 query.exec(sqlstatement);
 if(!query.isActive())
      {   logfile("SQLERROR: "+query.lastError().text());
          return; }

 sqlstatement="ALTER TABLE temp RENAME TO device";
 query.exec(sqlstatement);
 if(!query.isActive())
      {   logfile("SQLERROR: "+query.lastError().text());
          return; }


}}


}




//////////////////////////////////////////////////////////////////////
void MainWindow::loaddevicebox() {

    QString sqlstatement;
    QStringList mstringlist;
    QSqlQuery query;

       ui->deviceBox->clear();
       sqlstatement= "SELECT description FROM device";
       query.exec(sqlstatement);
           while (query.next()) {
              ui->deviceBox->addItem(query.value(0).toString());
           }

         //  query.exec("PRAGMA table_info(device)");
          // while (query.next()) {
         //      mstringlist << query.value(1).toString();
         //  }

     //      if ( db.tables().contains("device") )
       //     qDebug() << "true";
      //     else
     //    qDebug() << "false";

}



//////////////////////////////////////////////////////////////////////
void MainWindow::get_data() {
   QNetworkRequest request;
   request.setUrl(QUrl(vqurl));

   QNetworkAccessManager *nam = new QNetworkAccessManager();
   QNetworkReply *reply = nam->get(request);

   connect(reply, SIGNAL(finished()),
           this, SLOT(onReqCompleted()));



}




/////////////////////////////////
void MainWindow::onReqCompleted() {
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

       s1 = strip(s1);

       int err = reply->error();

       QString s2 = QString::number(err);

       if (version != s1)
       {
           QMessageBox::StandardButton reply;
            reply = QMessageBox::question(0, "", program+" version "+s1+" is ready. Download?",
                    QMessageBox::Yes|QMessageBox::No);
              if (reply == QMessageBox::Yes)
               {
                  QString link = "http://www.jocala.com";
                  QDesktopServices::openUrl(QUrl(link));
                }

       }

     //  delete reply;

}









//////////////////////////////////////////////////////////////////////
void MainWindow::on_actionQuit_triggered()
{

    QCoreApplication::quit();

}

////////////////////////////////////////////////////////////////////////////
bool MainWindow::installAPK(QString filename)
{

    QString cstring;
    QString command;


    QElapsedTimer rtimer;
    int nMilliseconds;
    rtimer.start();





    logfile("Installing "+filename);
    cstring = getadb() + " install -r " + '"'+ filename+'"';
    command=RunLongProcess(cstring,"Install APK");
    logfile(command);



    nMilliseconds = rtimer.elapsed();
    logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );


   // if (!command.contains("bytes in") || command.contains("Failure"))
  //  {
    //    QMessageBox::critical(this,"",filename+" install failed.\nSee log.");
     //   return false;
  //  }
  //  else return true;

    if (!command.contains("uccess") || command.contains("Failure"))
    {
        QMessageBox::critical(this,"",filename+" install failed.\nSee log.");
        return false;
    }
    else return true;




}

////////////////////////////////////////////////////////////////////////////
void MainWindow::on_sideload_Button_clicked()
{

    if (!check_devices() )
        return;

     bool installer=false;

    QStringList filenames = QFileDialog::getOpenFileNames(this,tr("APK files"),QDir::currentPath(),tr("APK files (*.apk);;All files (*.*)") );
    if( !filenames.isEmpty() )
    {

        QMessageBox::StandardButton reply;
          reply = QMessageBox::question(this, "Install", "Install APKs?",
                                        QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::Yes)
          {
               for (int i =0;i<filenames.count();i++)
              installer = installAPK(filenames.at(i));

          }



          if (installer)
          {

           

              QMessageBox::information(this,"","APK(s) installed.\nSee log for details.");


          }


    }


}


///////////////////////////////////////////////////////////////////////////
void MainWindow::on_uninstall_Button_clicked()
{


    if (!check_devices() )
        return;



QString package = "";
QString cstring;
bool keepbox = false;




    QElapsedTimer rtimer;
    int nMilliseconds;
    rtimer.start();




    logfile("open uninstall dialog");


     if(isusb)
         port="";

    uninstallDialog dialog(daddr,port);


    // dialog.setModal(true);
    if(dialog.exec() == QDialog::Accepted)
    {

    package = dialog.packageName();
    keepbox = dialog.keepBox();

    }

    else return;


    if (package.isEmpty())
       {
        QMessageBox::critical(this,"","No file selected");
        


        return;
        }



            if ( !is_package(package))
               { QMessageBox::critical(
                     this,
                     "",
                     package +" not installed");
                  return;

            logfile("Error: "+ package +" not installed");
            }


            QMessageBox::StandardButton reply;
                  reply = QMessageBox::question(this, "Uninstall", "Uninstall "+package+"?",
                     QMessageBox::Yes|QMessageBox::No);
                  if (reply == QMessageBox::Yes) {



                      if (!keepbox)
                      cstring = getadb()+" shell pm uninstall " + package;
                      else
                      cstring = getadb()+" shell pm uninstall -k " + package;

                      logfile("uninstall: "+cstring);

                      QString command=RunLongProcess(cstring,"Uninstall APK");

;

                      nMilliseconds = rtimer.elapsed();
                      logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );


                      if (!command.contains("Success"))
                           
                           QMessageBox::critical(this,"","Uninstall failed");
                        else
                          QMessageBox::information(this,"","Uninstalled");
                         


                  }
}


/////////////////////////////////////////////////////////////////////////
void MainWindow::on_connButton_clicked()
{



    getRecord(ui->deviceBox->currentText());

       if (  (ostype == "1") ||  (ostype == "2")  || (ostype == "3") )
        {

         
           QMessageBox::critical(this,"","Button is for Android devices only");

           return;

       }




    QString cstring;
    QString command;

   if (!ui->adhocip->text().isEmpty())

     {
           daddr=ui->adhocip->text();
           port="5555";
           cstring = adb + " connect "+daddr+":"+port;
           command=RunProcess(cstring);

          if (command.contains("connected to"))
             {   isConnected=true;
                 default_device_values();
                 on_refreshConnectedDevices_clicked();
                 logfile("Connected to "+daddr+":"+port);
                 return;
              }
    }


    getRecord(ui->deviceBox->currentText());


    if (isusb )
      {
          
           QMessageBox::critical(this,"","Inactive for USB connections");
           return;
      }

    if (daddr.isEmpty())
    {
       
        QMessageBox::critical(this,"","Device address required");
        return;
    }


    QElapsedTimer rtimer;
    int nMilliseconds;
    rtimer.start();



    //test pre-refresh

   // on_refreshConnectedDevices_clicked();

    cstring = adb + " connect "+daddr+":"+port;

      command=RunProcess(cstring);

    if (command.contains("connected to"))
    {   isConnected=true;

           on_refreshConnectedDevices_clicked();


           logfile("Connected to "+daddr+":"+port);

           if (searchlistDevices(ui->deviceBox->currentText()))
           {

                QString mstring =  ui->deviceBox->currentText();

               for(int i = 0; i < ui->listDevices->count(); ++i)
               {
                   QString str = ui->listDevices->item(i)->text();
                  if (str==mstring)
                      ui->listDevices->item(i)->setSelected(true);
               }

           }

    }
       else
       {
        isConnected=false;
        logfile("Unable to connect to: "+daddr+":"+port);
        QMessageBox::critical(this,"","Can't connect to "+daddr);

       }




    if(isConnected)
       {



         ui->server_running->setText(adbstr1);
         serverRunning = true;


       isConnected=searchlistDevices(ui->deviceBox->currentText());

      }



     nMilliseconds = rtimer.elapsed();
     logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );

}





////////////////////////////////////////////////////////////////
void MainWindow::on_disButton_clicked()
{


    getRecord(ui->deviceBox->currentText());

       if (  (ostype == "1") ||  (ostype == "2")  || (ostype == "3") )
        {

           
             QMessageBox::critical(this,"","Button is for Android devices only");
            return;

       }



    if (!ui->adhocip->text().isEmpty())

      {
            daddr=ui->adhocip->text();
            port="5555";
            QString cstring = adb + " disconnect "+daddr+":"+port ;
            QString command=RunProcess(cstring);
            logfile("disconnect: "+daddr);
            on_refreshConnectedDevices_clicked();
            return;
        }




    if (isusb )
      {
         
         QMessageBox::critical(this,"","Inactive for USB connections");
          return;
      }


    if (daddr.isEmpty())
    {
       QMessageBox::critical(this,"","Device address required.");
       
        return;
    }



    QElapsedTimer rtimer;
    int nMilliseconds;
    rtimer.start();



         QString cstring = adb + " disconnect "+daddr+":"+port ;
         QString command=RunProcess(cstring);

         logfile("disconnect: "+daddr);



         on_refreshConnectedDevices_clicked();
         nMilliseconds = rtimer.elapsed();
         logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );


}


/////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionAbout_triggered()
{


    Dialog2 dialog2;
    dialog2.setModal(true);
    dialog2.setvLabel(version);
    dialog2.exec();

}


//////////////////////////////////////////////
void MainWindow::on_actionHelp_triggered()
{
    logfile("opening help");
    helpDialog helpdialog;
    helpdialog.setModal(true);
    helpdialog.exec();
}



/////////////////////////////////////////////
void MainWindow::backupAndroid()
{

    if (!check_devices() )
        return;



 QString lookup=daddr;
 QString cstring;
 QString command;
 QString mcpath;
 QString thpath;
 QString envpath;
 QString rootpath;
 QString thumbpath;


    is_package(xbmcpackage);

   if (!is_packageInstalled)
      {
       
       QMessageBox::critical(this,"", xbmcpackage+" not installed");
       return;
       }


   backupDialog dialog;

    dialog.setadb_backup(getadb(),data_root);

    dialog.setModal(true);

if(dialog.exec() == QDialog::Accepted)
{

     QString n_data_root = dialog.return_data_root();

     if(!n_data_root.startsWith("/"))
        n_data_root.prepend("/");

     if(!n_data_root.endsWith("/"))
        n_data_root.append("/") ;


     thumbpath = dialog.returnthumb();

     rootpath=n_data_root+"Android/data/"+xbmcpackage;
     thumbpath=rootpath+filepath+"/userdata/";



   QElapsedTimer rtimer;
   int nMilliseconds;
   rtimer.start();



 QString hdir = QDir::homePath();


 mcpath = rootpath+filepath;

 cstring = getadb() + " shell ls "+mcpath;
 command=RunProcess(cstring);

 if (command.contains("No such file or directory"))
  {
    QMessageBox::critical(this,"","Media center files not found on "+n_data_root);
    logfile("backup result: "+command+"/n"+cstring);
    return;
 }


 cstring = getadb() + " shell ls "+thumbpath+"/Thumbnails";
 command=RunProcess(cstring);
 logfile("backup: "+command);
 if (command.contains("No such file or directory"))
  {
   logfile("Thumbnails not found");
   QMessageBox::critical(this,"","Thumbnails not found.  "+thumbpath );
   return;
 }


 QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Backup Destination"),
                                               hdir,
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);

if (!dir.isEmpty() )
 {
  QMessageBox::StandardButton reply;
   reply = QMessageBox::question(this, "Backup", "backup to "+dir,
                                 QMessageBox::Yes|QMessageBox::No);
   if (reply == QMessageBox::Yes)
   {
    logfile("backup function started");
       QString udir = dir+"/userdata";



       mcpath=mcpath+"/";
       dir = dir + "/";

       if (os == 1)
       {
           dir.replace("/","\\");
           udir.replace("/","\\");
       }



       cstring = getadb() + " pull "+mcpath+".  "+'"'+dir+'"';
       //command=RunLongProcess(cstring,"Backing up");
       command=RunLongProcess(cstring,"Backup");
       logfile("backup: "+cstring);




       QString dbfile=dir+"userdata/Database/Addons16.db";
       QString thfile=dir+"userdata/Thumbnails";
       QString udfile=dir+"userdata";

       if (!QDir(thfile).exists())
       {

           cstring = getadb() +  " pull "+thumbpath+" "+'"'+dir+'"'+"/userdata";
           //command=RunLongProcess(cstring,"Kodi backup");
             command=RunLongProcess(cstring,"Backup");

           logfile("backup: "+cstring);
           QFile::rename(dir+"/userdata/advancedsettings.xml", dir+"/userdata/advancedsettings.backup");

       }


        if  (QDir(udfile).exists())
        
         QMessageBox::information(this,"","Backup complete");
        else
          QMessageBox::critical(this,"","Backup Failed. See Log.");




   } //end of do backup
 } // end of !dir

nMilliseconds = rtimer.elapsed();
logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );

logfile("Backup complete");

}  // end-dialog-accepted.

}  // end function




////////////////////////////////////////

void MainWindow::androidRemote()
{


    if (!check_devices() )
        return;

    is_package(xbmcpackage);

   if (!is_packageInstalled)
      { QMessageBox::critical(
            this,
            "",
            xbmcpackage+" not installed");
         return;
   }




QElapsedTimer rtimer;
int nMilliseconds;
rtimer.start();


QString cstring;
QString command;
QString hidden;
QString xpath = "";
QString mcpath=data_root+ "Android/data/"+xbmcpackage+filepath;


cstring = getadb() + " shell ls "+mcpath;

command=RunProcess(cstring);

if (command.contains("No such file or directory"))
{
   QMessageBox::critical(
               this,
              "",
               "Destination path missing\nRun Kodi to create filesystem "+mcpath);
               return;
}

mcpath = mcpath+"/userdata/keymaps/";
cstring = getadb() + " shell ls "+mcpath;

command=RunProcess(cstring);

if (command.contains("No such file or directory"))
 {
    cstring = getadb() + " mkdir -p "+ mcpath;
    command=RunProcess(cstring);
}




 QString fileName = QFileDialog::getOpenFileName(this,
 "Choose remote xml file", xmldir, tr("Files (*.xml)"));

 if (!fileName.isEmpty() )
 {


  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Push", fileName+" selected. Continue?",QMessageBox::Yes|QMessageBox::No);

   if (reply == QMessageBox::No)
         return;


           cstring = getadb() + " push "+'"'+fileName+'"'+ " "+mcpath+"/keyboard.xml";
           command=RunProcess(cstring);




           // logfile(cstring);
           logfile("push remote:"+command);

           nMilliseconds = rtimer.elapsed();
           logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );



           if (command.contains("bytes"))


           {


              QMessageBox::information(this,"","Remote xml installed.");
              

           }
               else

           {

               QMessageBox::critical(
                           this,
                           "",
                        "Remote xml installation failed.");}



}




}


////////////////////////////////////////

void MainWindow::otherRemote()
{


    QElapsedTimer rtimer;
    int nMilliseconds;
    rtimer.start();



    QString mcpath=filepath;


    mcpath = mcpath+"/userdata/keymaps/";

    QDir dir(mcpath);
    if (!dir.exists()) {
        dir.mkpath(mcpath);
    }

     QString fileName = QFileDialog::getOpenFileName(this,
     "Choose remote xml file", xmldir, tr("Files (*.xml)"));

     if (!fileName.isEmpty() )
     {


         QMessageBox::StandardButton reply;
           reply = QMessageBox::question(this, "Push", fileName+" selected. Continue?",
               QMessageBox::Yes|QMessageBox::No);
           if (reply == QMessageBox::Yes) {

               QFileInfo fileInfo(fileName);
               QString filename(fileInfo.fileName());




              QFile::copy(fileName, mcpath+"/"+"keyboard.xml");

               nMilliseconds = rtimer.elapsed();
               logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );


               if (QFileInfo::exists(mcpath+"/"+"keyboard.xml"))
               {
                   
                    QMessageBox::information(this,"","Remote xml installed.");
               }

             else {
               QMessageBox::critical(this,"","Copy failed. See log." );
               logfile("Copy of "+fileName);
               logfile("to folder "+mcpath);
               logfile("failed for "+filename);

               }



               }


    }
}



///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushRemote_clicked()
{


    getRecord(ui->deviceBox->currentText());

    if (  (ostype == "1") ||  (ostype == "2")  || (ostype == "3") )
     {
          otherRemote();
       }

       else

  if (check_devices() )
      {
       androidRemote();
      }


}

////////////////////////////////////////////////////

bool MainWindow::searchlistDevices(QString mstring)
{

  QList<QListWidgetItem *> items = ui->listDevices->findItems(mstring, Qt::MatchExactly);
    if (items.size() > 0)
         return true;
    else
       return false;

}

////////////////////////////////////////////////
void MainWindow::on_adbshellButton_clicked()
{

    if (!check_devices() )
        return;



    logfile("detaching console process");
    logfile(daddr+":"+port);

    QString cstring = "";



    if (os == 1)

          {

          if (isusb)
              { cstring = "cmd /k  adb -s "+daddr+" shell";
                // logfile(cstring);
                 QProcess::startDetached(cstring);
               }
           else
             {   cstring = "cmd /k  adb -s "+daddr+":"+port+ " shell";
                 // logfile(cstring);
                 QProcess::startDetached(cstring);
              }
        }


     else

       {

        QString commstr = adbdir+"console.sh";
        QFile::remove(commstr);
        QFile file(commstr);



                 if(!file.open(QFile::WriteOnly))

             {
                logfile("error creating console.sh!");
                 QMessageBox::critical(this,"","Error creating command file!");
                 return;
             }



       QTextStream out(&file);

       out  << "#!/bin/sh" << endl;
       out  <<  getadb()+ " shell " << endl;
       cstring = getadb()+ " shell";
       // logfile(cstring);

             file.flush();
             file.close();

       cstring = "chmod 0755 " + commstr ;
       QString command=RunProcess(cstring);

      //  cstring = "gnome-terminal -e "+adbdir+"console.sh";
      //  cstring = "x-terminal-emulator -e "+adbdir+"console.sh";

        QString shelldir = '"'+adbdir+"console.sh"+'"';

     //  if (os == 0)
        // cstring = "x-terminal-emulator -e "+adbdir+"console.sh";
    //     cstring = "x-terminal-emulator -e "+adbdir+"console.sh";


       if (os == 0)
            {

            if (getmsgboxtype())
                cstring = "konsole --workdir="+adbdir+ " -e "+adbdir+"console.sh";
                else
                cstring = "gnome-terminal --working-directory="+adbdir+ " -x "+adbdir+"console.sh";

             }


       if (os == 2)
            {

            if (getmsgboxtype())
                cstring = "open -a iTerm.app "+shelldir;
                else
                cstring = "open -a Terminal.app "+shelldir;;

             }




    //   if (os == 2)
     //  //cstring = "open -a Terminal.app "+adbdir+"console.sh";
     //  cstring = "open -a Terminal.app "+shelldir;


       QProcess::startDetached(cstring);

    }


}


////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionReboot_triggered()
{

     QString cstring;

    isConnected=check_Connection();

      if (!isConnected)
            {
              
               QMessageBox::critical(this,"",devstr2);
              return;
            }




   QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "Reboot Device", "Reboot Device?",
         QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes) {
         isConnected=false;
        logfile("rebooting device");
        rebootDevice(" reboot");
     }

}



//////////////////////////////////////////

void MainWindow::on_actionReboot_bootloader_triggered()
{
    QString cstring;

     isConnected=check_Connection();

     if (!isConnected)
           {
            
            QMessageBox::critical(this,"",devstr2);
            return;
           }




   QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "", "Reboot to bootloader?",
         QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes) {
         isConnected=false;


         logfile("rebooting device recovery");
         rebootDevice(" reboot bootloader");
        }

}



///////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionRecovery_triggered()
{
    QString cstring;

     isConnected=check_Connection();

     if (!isConnected)
           { 
            QMessageBox::critical(this,"",devstr2);
            return;
           }




   QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "", "Reboot to recovery?",
         QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes) {
         isConnected=false;


         logfile("rebooting device recovery");
         rebootDevice(" reboot recovery");
        }

}





/////////////////////////////////////////
void MainWindow::on_donate_clicked()
{
    QString link = "https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JA5E5UP3ZSWBN";
   QDesktopServices::openUrl(QUrl(link));

}



/////////////////////////////////////////////////
void MainWindow::restoreAndroid()

{


    if (!check_devices() )
        return;


    is_package(xbmcpackage);

    if (!is_packageInstalled)
       { 

        QMessageBox::critical(this,"",xbmcpackage+" not installed");
        return;
        }

   QString cstring;
   QString command;
   QString rootpath;
   QString thumbpath;
   QString envpath;
   QString mcpath;



   cstring = cstring = getadb() +  " shell ps | grep "+xbmcpackage;
   command=RunProcess(cstring);


   if (command.contains(xbmcpackage))
      {


       QMessageBox::StandardButton reply;
          reply = QMessageBox::question(this, "Stop Kodi", "Cannot restore while Kodi is running.\n Stop "+xbmcpackage+" on device?"  ,
                                        QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::Yes)
          {


          QString cstring = getadb() + " shell am force-stop "+xbmcpackage;
          QString command=RunProcess(cstring);
          logfile(command);
       }


          else {




      // QMessageBox::critical(this,"","Cannot restore while\n"+xbmcpackage+" is running!");
        logfile(xbmcpackage+" running. Restore failed");
        return;
  }




   }




   restDialog dialog;

    dialog.setModal(true);

    dialog.setadb_restore(getadb());

   if(!dialog.exec() == QDialog::Accepted)
      return;



       data_root = dialog.restore_data_root();

       // QMessageBox::critical(0,"",data_root );



        if(!data_root.startsWith("/"))
          data_root.prepend("/");

       if(!data_root.endsWith("/"))
          data_root.append("/") ;




        rootpath=data_root+"Android/data/"+xbmcpackage;

          //QMessageBox::critical(this,"",rootpath);
          //return;


              envpath=rootpath+"/files/";
              mcpath=rootpath+filepath;



   QElapsedTimer rtimer;
   int nMilliseconds;
   rtimer.start();


// extra rms


 QString dir = QFileDialog::getExistingDirectory(this, tr("Choose backup folder"),
  hdir,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);


 if (!QDir(dir+"/userdata").exists() )

   {
     QMessageBox::critical(0,"","Invalid backup. userdata folder not found." );
     return;
   }



 if (!QDir(dir+"/addons").exists() )

   {
     QMessageBox::critical(0,"","Invalid backup. addons folder not found." );
     return;
   }





 if (dir.isEmpty() )
  return;






 QMessageBox::StandardButton reply;
   reply = QMessageBox::question(this, "Restore", "Restore from "+dir+" to "+data_root+" ?"+"\nThis will overwrite the existing setup!",
                                 QMessageBox::Yes|QMessageBox::No);
   if (reply == QMessageBox::No)
   return;



       logfile("Restoring Android Kodi");






     // nuke existing

//       cstring = getadb() + " shell rm -r /sdcard/Android/data/"+xbmcpackage;
//       command=RunLongProcess(cstring,"Preparing target");
//       logfile(command);

         cstring = getadb() + " shell rm -r "+rootpath;
         command=RunLongProcess(cstring,"Preparing target");
         logfile(command);

       cstring = getadb() + " shell ls "+mcpath;
       command=RunProcess(cstring);



       if (command.contains("No such file or directory"))
        {
           cstring = getadb() + " shell mkdir -p "+mcpath;
           command=RunProcess(cstring);
           logfile(command);
           QString errorp = command;
           cstring = getadb() + " shell ls "+mcpath;
           command=RunProcess(cstring);

           if (command.contains("No such file or directory"))
            {
               QMessageBox::critical(this,"","Error creating restore point");
               logfile("Restore error:"+ errorp);
               return;
             }

        } // nuke existing




       cstring = getadb() + " shell rm /sdcard/xbmc_env.properties";
       command=RunProcess(cstring);

       cstring = getadb() + " shell rm -r "+mcpath + "/*";
       command=RunProcess(cstring);

       dir=dir+"/.";

       cstring = getadb() + " push "+'"'+dir+'"'+ " "+mcpath;
       command=RunLongProcess(cstring,"Restore");

        nMilliseconds = rtimer.elapsed();
        logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );


   if (command.contains("bytes"))

       {  QMessageBox::information(this,"","Restore complete");

           if (data_root != "/sdcard/")
                {
                  cstring = getadb() + " shell echo xbmc.data="+envpath+" > /sdcard/xbmc_env.properties";
                  command=RunProcess(cstring);
                  logfile("create /sdcard/xbmc_env.properties");
                  logfile(command);
                }
          }

      else

            {
               QMessageBox::critical(this,"","Restore Failed. See log.");
               logfile(cstring);
               logfile(command);
            }



}



/////////////////////////////////////////////

void MainWindow::on_editRecord_clicked()
{
         dataentry(0);
         loaddevicebox();

}

////////////////////////////////////////

void MainWindow::on_doConsole_clicked()
{


    // set PATH=%PATH%;C:\xampp\php

    logfile("detaching console process");

     QString cstring = "";
     QString command ="";


     if (os == 1)

            {

            QString commstr = adbdir+"cpath.bat";
            QFile file(commstr);

                if(!file.open(QFile::WriteOnly |
                              QFile::Text))
                {
                    logfile("error creating cpath.bat!");
                    QMessageBox::critical(this,"","Error creating bat file!");
                    return;
                }



                QTextStream out(&file);
                out  << "set PATH=%PATH%;"+QDir::currentPath()+";"<< endl;



                file.flush();
                file.close();

                cstring = "cmd /k " +QDir::currentPath()+"/cpath.bat";
                QProcess::startDetached(cstring);


         }






     QString pathdir = QCoreApplication::applicationDirPath() +"/adbfiles";

       if (os == 2 || os == 0)
         {

        //adbdir=QDir::toNativeSeparators(adbdir);

           adbdir=QDir::fromNativeSeparators(adbdir);


           QString commstr = adbdir+"cpath.sh";


           QFile file(commstr);

               if(!file.open(QFile::WriteOnly |
                             QFile::Text))
               {
                   logfile("error creating cpath!");
                   QMessageBox::critical(this,"","Error creating sh file!");
                   return;
               }



               QTextStream out(&file);
                out  << "#!/bin/sh" << endl;
                out  << "export PATH="+pathdir+":$PATH" << endl;
                out  << "/bin/sh" << endl;


               file.flush();
               file.close();


               cstring = "chmod 0755 " + commstr ;
               QString command=RunProcess(cstring);



       }

/*
       if (os == 2)
        {
       //QString cstring = "open -a Terminal.app "+adbdir+"cpath.sh";
       QString cstring = "open -a iTerm.app "+adbdir+"cpath.sh";
       // logfile(cstring);
       QProcess::startDetached(cstring);
        }

*/

       if (os == 2)
            {

            if (getmsgboxtype())
                cstring = "open -a iTerm.app "+adbdir+"cpath.sh";
                else
                cstring = "open -a Terminal.app "+adbdir+"cpath.sh";

                QProcess::startDetached(cstring);

             }


       if (os == 0)
            {

            if (getmsgboxtype())
                cstring = "konsole --workdir="+adbdir+ " -e "+adbdir+"cpath.sh";
                else
                cstring = "gnome-terminal --working-directory="+adbdir+ " -x "+adbdir+"cpath.sh";

                QProcess::startDetached(cstring);

             }




}


////////////////////////////////////////

void MainWindow::on_killServer_clicked()
{
    QElapsedTimer rtimer;
    int nMilliseconds;
    rtimer.start();


    QString cstring = adb + "  disconnect "+daddr+":"+port ;
    QString command=RunProcess(cstring);

    isConnected=false;


    kill_server();

    if (serverRunning)
      ui->server_running->setText(adbstr1);
    else
     ui->server_running->setText(adbstr2);

  ui->listDevices->clear();
  ui->listDevicesStatus->clear();

    nMilliseconds = rtimer.elapsed();
    logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );


}



///////////////////////////////////////////////////
void MainWindow::on_refreshConnectedDevices_clicked()
{


    QString isdevice;
    QString cstring = "";
    QString command = "";
    QSqlQuery query;
    QString sqlstatement;
    QString descrip;
    QString mdaddr;
    QStringList mstringlist;
    bool isusb;
    QStringList zzz;



    ui->listDevices->clear();
    ui->listDevicesStatus->clear();
    ui->listDevices->setSelectionMode(QAbstractItemView::SingleSelection);



     cstring = adb + " devices";


     for( int z = 0; z < 10; z = z + 1 )
       {     command="";
             command=RunProcess(cstring);
        }

     // QThread::sleep(2);

      mstringlist=command.split(QRegExp("[\t\n\r]"),QString::SkipEmptyParts);

if (mstringlist.count() >0)
 {


     mstringlist.removeFirst();


    //  if (!mstringlist.contains("*"))
     //     if(!mstringlist.contains("daemon"))

for( int a = 0; a < mstringlist.size(); a = a + 2 )
 {


          if (mstringlist.at(a).contains(":"))
             {
               QStringList pieces = mstringlist.at(a).split( ":" ,QString::SkipEmptyParts);
               mdaddr=pieces.at(0);
               isusb=false;
              }

          else

              {
                 isusb=true;
                 mdaddr=mstringlist.at(a);
              }


          if (!mdaddr.isEmpty())
             descrip=getDescription(mdaddr);

          if (descrip.isEmpty())
              descrip=mstringlist.at(a);


          isdevice=mstringlist.at(a+1);

          if (isdevice.contains("device"))
             {
                   if (isusb)
                       isdevice="USB connection";
                   else
                       isdevice="IP connection";
            }


          if (!descrip.contains("*") || !descrip.contains("daemon"))
           {


             for(int i = 0; i < ui->listDevices->count(); ++i)
             {
                 QListWidgetItem* item = ui->listDevices->item(i);
                 if (item->text().contains(descrip))
                 return;  // bail if exists
             }


              ui->listDevices->addItem(descrip);
              ui->listDevicesStatus->addItem(isdevice);

          }

           if (isdevice.contains("unauthorized"))
               QMessageBox::critical(0,"","Device unauthorized\n\nPress 'Refresh ADB' or see help topic 'Device unauthorized' ");

          if (isdevice.contains("offline"))
              QMessageBox::critical(0,"","Device offline\n\nPress 'Refresh ADB' or see help topic 'Device Offline' ");


}


       ui->listDevices->setFocus();

}

 return;

}





//////////////////////////////////////////
void MainWindow::on_newRecord_clicked()
{


  logfile("go to dataentry -- new record");

  blank_entry_form();
  dataentry(1);
  loaddevicebox();

}


//////////////////////////////////////////////////
void MainWindow::dataentry(int flag)
{

// flag = 0 edit record
// flag = 1 new record

  ui->deviceBox->currentIndex();

logfile("opening preferences dialog");

    QString cstring;
    QString command;
    QString oldpass;
    QString olddaddr;
    QString olddescription;
    QString pkg;
    QString device_name;
    QStringList mstringlist;
    QStringList dstringlist;


    cstring = adb + " devices";
    command=RunProcess(cstring);
    QThread::sleep(2);

     mstringlist=command.split(QRegExp("[\t\n\r]"),QString::SkipEmptyParts);


    if (command.contains("List of devices attached"))
     {
      mstringlist.removeFirst();

            for( int a = 0; a < mstringlist.size(); a = a + 2 )
                 {
                    QStringList pieces = mstringlist.at(a).split( ":" ,QString::SkipEmptyParts);

                     if (!mstringlist.at(a).contains("daemon"))
                      dstringlist << pieces.at(0);
                  }

          }



    olddaddr = daddr;


    updatecheck = true;
    olddescription=description;

   // disableroot=false;

    preferencesDialog dialog;

    dialog.setPackagename(xbmcpackage);
    dialog.setPulldir(pulldir);
    dialog.setdevicelist(dstringlist);
    dialog.setfilepath(filepath);
    dialog.setdataroot(data_root);
    dialog.setversionLabel(version);
    dialog.setostype(ostype);
    dialog.setdescription(description);
    dialog.setadb_pref(getadb());
    dialog.setdisableroot(disableroot);



    if (isusb)
        dialog.setport("");
      else
        dialog.setport(port);

    dialog.setdaddr(daddr);


    dialog.setisusb(isusb);
    dialog.setModal(true);


    if(dialog.exec() == QDialog::Accepted)
    {



    int x = dialog.returnval1();


    data_root = dialog.data_root();
    xbmcpackage = dialog.xbmcpackageName();
    pulldir = dialog.pulldir();
    description = dialog.description();
    data_root = dialog.data_root();
    filepath = dialog.filepath();
    port = dialog.port();
    daddr = dialog.daddr();
    isusb = dialog.isusb();
    ostype = dialog.ostype();
    disableroot = dialog.disableroot();

  if(daddr.contains(validip))
  {
      isusb=false;
  }


  else


  {
      if (!isusb  && ostype == "0")
        {  QMessageBox::StandardButton reply;
           reply = QMessageBox::question(0, "", "USB connection?",
                   QMessageBox::Yes|QMessageBox::No);
             if (reply == QMessageBox::No)
                isusb=false;
               else
                 isusb=true;
      }
  }




    if (x == 1 && flag == 1)
    {logfile("go insert a record");
      insertDevice();
      on_refreshConnectedDevices_clicked();
    }

    if (x == 1 && flag == 0)
           {
             logfile("Saving Device Record");
              updateDevice();
              on_refreshConnectedDevices_clicked();

           }  // end x1



  }




 }


////////////////////////////////////////
void MainWindow::on_delRecord_clicked()
{

    description = ui->deviceBox->currentText();

    if (!description.isEmpty())
     {

    QString descrip = description;

    QMessageBox::StandardButton reply;
     reply = QMessageBox::question(0, "", " Delete "+descrip+"?",
             QMessageBox::Yes|QMessageBox::No);
       if (reply == QMessageBox::No)
        {
          return;
         }

       deleteRecord(descrip);
       loaddevicebox();
       on_refreshConnectedDevices_clicked();

       logfile(descrip+ " is deleted");
  }
}




//////////////////////////////////////////////////////////////////////
void MainWindow::cacheButton_other()
{

    bool oldxml = false;


    QString line1;
    QString line2;
    QString line3;
    QString line4;
    QString line5;
    QString line6;
    QString line7;



QString mcpath=filepath;

mcpath = mcpath+"/userdata/";

if (!QFileInfo::exists(mcpath))
{
     QMessageBox::information(this,"","userdata folder not found" );
     return;
}

logfile("opening cache dialog");

   cacheDialog dialog;

    dialog.setxmlcheck(false);
    dialog.setcbuffermode(buffermode);
    dialog.setcbuffersize(buffersize);
    dialog.setcbufferfactor(bufferfactor);

    dialog.setModal(true);


    if(dialog.exec() == QDialog::Accepted)
    {

     int x = dialog.returncval1();


    buffermode = dialog.cbuffermode();
    buffersize = dialog.cbuffersize();
    bufferfactor = dialog.cbufferfactor();
    oldxml = dialog.xmlcheck();

    QString str1;
    str1.setNum(buffermode-1);

    if (oldxml)
      {
          line1 ="<advancedsettings>";
          line2 ="  <network>";
          line3 ="    <buffermode>"+str1+"</buffermode>";
          line4 ="    <cachemembuffersize>"+buffersize+"</cachemembuffersize>";
          line5 ="    <readbufferfactor>"+bufferfactor+"</readbufferfactor>";
          line6 ="  </network>";
          line7 ="</advancedsettings>";
      }

    else

      {
         line1 ="<advancedsettings>";
         line2 ="  <cache>";
         line3 ="    <buffermode>"+str1+"</buffermode>";
         line4 ="    <memorysize>"+buffersize+"</memorysize>";
         line5 ="    <readfactor>"+bufferfactor+"</readfactor>";
         line6 ="  </cache>";
         line7 ="</advancedsettings>";
      }





    /*
    <cachemembuffersize> and <readbufferfactor>
     - In v17 <cachemembuffersize> is renamed to  <memorysize>
     and <readbufferfactor> is renamed to <readfactor>.
    In addition, all three buffer related settings in <network>
    are moved out of <network> and
    into a new <cache> parent tag.

    */



   if (x == 2)
          {


         QString str1;
         str1.setNum(buffermode-1);

              QString filename1 = "advancedsettings.xml";
              QString filename2 = mcpath+"/"+filename1;

              if (QFileInfo::exists(filename2))   // does file exist

              {
                    logfile("advancedsettings.xml exists");
                          QMessageBox::StandardButton reply;
                           reply = QMessageBox::question(this, "XML", "advancedsettings.xml exists. Overwrite?\n(will backup original)",
                                                         QMessageBox::Yes|QMessageBox::No);
                           if (reply == QMessageBox::No) {
                               logfile("abort xml write");
                               return;
                              } else
                              {
                              logfile("continue xml write");
                              logfile("backup advancedsettings.xml");
                              QFile::rename(filename2, mcpath+"/"+"advancedsettings.old.xml");
                              }
                          }  // end if exists


         QFile file(filename2);

          if(!file.open(QFile::WriteOnly))
                {
                  logfile("error creating advancedsettings.xml.");
                  QMessageBox::critical(this,"","Unknown error creating xml file!");
                   return;
                 }

                  QTextStream out(&file);

                  out  << line1 << endl;
                  out  << line2 << endl;
                  out  << line3 << endl;
                  out  << line4 << endl;
                  out  << line5 << endl;
                  out  << line6 << endl;
                  out  << line7 << endl;

                  file.flush();
                  file.close();


       }

  }  //end accepted


} // end  other cache

//////////////////////////////////////////////////////////////////////
void MainWindow::cacheButton_android()

{

    if (!check_devices() )
        return;


    bool oldxml = false;


    QString line1;
    QString line2;
    QString line3;
    QString line4;
    QString line5;
    QString line6;
    QString line7;




logfile("opening cache dialog");

if (data_root.isEmpty())
    data_root="/sdcard/";

QString cstring;
QString command;
QString hidden;
QString xpath = "";
QString mcpath=data_root+ "/Android/data/"+xbmcpackage+filepath;
xpath = mcpath+"/userdata/";

cstring = getadb() + " shell ls "+mcpath;

command=RunProcess(cstring);

if (command.contains("No such file or directory"))
{
   QMessageBox::critical(
               this,
              "",
               "Cache folder missing. Run Kodi to create it.");
               return;
}

   cacheDialog dialog;

    dialog.setxmlcheck(false);
    dialog.setcbuffermode(buffermode);
    dialog.setcbuffersize(buffersize);
    dialog.setcbufferfactor(bufferfactor);

    dialog.setModal(true);


    if(dialog.exec() == QDialog::Accepted)
    {

 int x = dialog.returncval1();


    buffermode = dialog.cbuffermode();
    buffersize = dialog.cbuffersize();
    bufferfactor = dialog.cbufferfactor();
    oldxml = dialog.xmlcheck();

    QString str1;
    str1.setNum(buffermode-1);




    /*
    <cachemembuffersize> and <readbufferfactor>
     - In v17 <cachemembuffersize> is renamed to  <memorysize>
     and <readbufferfactor> is renamed to <readfactor>.
    In addition, all three buffer related settings in <network>
    are moved out of <network> and
    into a new <cache> parent tag.

    */

             if (oldxml)
               {
                   line1 ="<advancedsettings>";
                   line2 ="  <network>";
                   line3 ="    <buffermode>"+str1+"</buffermode>";
                   line4 ="    <cachemembuffersize>"+buffersize+"</cachemembuffersize>";
                   line5 ="    <readbufferfactor>"+bufferfactor+"</readbufferfactor>";
                   line6 ="  </network>";
                   line7 ="</advancedsettings>";
               }

             else

               {
                  line1 ="<advancedsettings>";
                  line2 ="  <cache>";
                  line3 ="    <buffermode>"+str1+"</buffermode>";
                  line4 ="    <memorysize>"+buffersize+"</memorysize>";
                  line5 ="    <readfactor>"+bufferfactor+"</readfactor>";
                  line6 ="  </cache>";
                  line7 ="</advancedsettings>";
               }



    if (x == 2)
          {


   logfile("write advancedsettings.xml");



         cstring = getadb() + " shell ls "+mcpath;


              QString filename1 = "advancedsettings.xml";
              QString filename2 = adbdir+filename1;



             cstring = getadb() + " shell ls "+mcpath;
             command=RunProcess(cstring);
              if (command.contains("No such file or directory"))
                      {
                         QMessageBox::critical(this,"","Destination path missing");
                          return;
                     }



                     cstring = getadb() + " shell ls "+xpath+filename1;
                     command=RunProcess(cstring);

                     // logfile(cstring);
                     logfile(command);

                      if (!command.contains("No such file or directory"))
                         {
                          logfile("advancedsettings.xml exists");

                          QMessageBox::StandardButton reply;
                           reply = QMessageBox::question(this, "XML", "advancedsettings.xml exists. Overwrite?\n(will backup original)",
                                                         QMessageBox::Yes|QMessageBox::No);
                           if (reply == QMessageBox::No) {
                               logfile("abort xml write");
                               return;
                              } else {
                              logfile("continue xml write");
                              logfile("backup advancedsettings.xml");
                              cstring = getadb() + " shell cp "+xpath+filename1+" "+xpath+filename1+".old";
                              command=RunProcess(cstring);
                              }
                          }  // end if exists


          QFile file(filename2);


          if(!file.open(QFile::WriteOnly))
                {
                  logfile("error creating advancedsettings.xml.");
                  QMessageBox::critical(this,"","Unknown error creating xml file!");
                   return;
                 }

                  QTextStream out(&file);

                  out  << line1 << endl;
                  out  << line2 << endl;
                  out  << line3 << endl;
                  out  << line4 << endl;
                  out  << line5 << endl;
                  out  << line6 << endl;
                  out  << line7 << endl;

                  file.flush();
                  file.close();

                  cstring = getadb() + " push "+filename2+ " "+xpath+filename1;
                  command=RunProcess(cstring);

                   if (!command.contains("bytes"))
                      {
                        logfile(command);
                        logfile("error pushing xml script to device!");
                        QMessageBox::critical(this,"","Error pushing xml from PC to device!");
                        return;
                      }
                    else {
                       QMessageBox::information(this,"","advancedsettings.xml written");
                   }



           }


  }


} // end  android cache



///////////////////////////////////////////
void MainWindow::on_cacheButton_clicked()

{
    getRecord(ui->deviceBox->currentText());

    if (  (ostype == "1") ||  (ostype == "2")  || (ostype == "3") )
     {
           cacheButton_other();
       }

       else

  if (check_devices() )
      {
        cacheButton_android();
      }

}



/////////////////////////////////////////////////////////////////////////////////////
void MainWindow::splashButton_android()
{

    isConnected=check_Connection();


     if (!isConnected)
           {
              QMessageBox::critical(this,"",devstr2);
             
              return;
           }

     if(!is_busybox())
     {
         QMessageBox::critical(0,"","Busybox installation failed.");
        return;
     }

     busybox_permissions();

    is_package(xbmcpackage);

   if (!is_packageInstalled)
      { QMessageBox::critical(
            this,
            "",
            xbmcpackage+" not installed");
         return;
   }


  QString cstring;
  QString command;
  QString hidden;
  QString xpath = "";
  QString mcpath=data_root+ "Android/data/"+xbmcpackage+filepath;

  mcpath=mcpath+"/media";

  cstring = getadb() + " shell ls "+mcpath;
  command=RunProcess(cstring);


  cstring = getadb() + " shell ls "+mcpath;
  command=RunProcess(cstring);

  if (command.contains("No such file or directory"))
   {
      cstring = getadb() + " mkdir -p "+ mcpath;
      command=RunProcess(cstring);
  }





QElapsedTimer rtimer;
int nMilliseconds;
rtimer.start();



 QString fileName = QFileDialog::getOpenFileName(this,
 "Choose splash screen file", splashdir, tr("Files (*.png)"));

 if (!fileName.isEmpty() )
 {


     QMessageBox::StandardButton reply;
       reply = QMessageBox::question(this, "Push", fileName+" selected. Continue?",
           QMessageBox::Yes|QMessageBox::No);
       if (reply == QMessageBox::Yes) {


           cstring = getadb() + " push "+'"'+fileName+'"'+ " "+mcpath+"/splash.png";

           command=RunLongProcess(cstring,"Splash Screen");

           // logfile(cstring);
           logfile(command);

           nMilliseconds = rtimer.elapsed();
           logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );



           if (command.contains("bytes"))


           {



               QMessageBox::information(
                           this,
                          "",
                          "Splash screen installed." );
           }
               else

           {

               QMessageBox::critical(
                           this,
                           "",
                        "Splash screen installation failed.");}


   }

}

}



///////////////////////////////////////////////////////////////////////////////////////
void MainWindow::splashButton_other()
{

QElapsedTimer rtimer;
int nMilliseconds;
rtimer.start();



QString mcpath=filepath;

 mcpath=mcpath+"/media/";


QDir dir(mcpath);
if (!dir.exists()) {
    dir.mkpath(mcpath);
}



 QString fileName = QFileDialog::getOpenFileName(this,
"Choose splash screen file", splashdir, tr("Files (*.png)"));

 if (!fileName.isEmpty() )
 {


     QMessageBox::StandardButton reply;
       reply = QMessageBox::question(this, "Push", fileName+" selected. Continue?",
           QMessageBox::Yes|QMessageBox::No);
       if (reply == QMessageBox::Yes) {

           QFileInfo fileInfo(fileName);
           QString filename(fileInfo.fileName());









          QFile::copy(fileName, mcpath+"/"+"splash.png");

           nMilliseconds = rtimer.elapsed();
           logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );


           if (QFileInfo::exists(mcpath+"/"+"splash.png"))
           {
                QMessageBox::information(this,"","Splash screen installed." );
               }

         else
           {
           QMessageBox::information(this,"","Copy failed. See log." );
           logfile("Copy of "+fileName);
           logfile("to folder "+mcpath);
           logfile("failed for "+filename);

              }
           }


}

}


/////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_splashButton_clicked()
{

    getRecord(ui->deviceBox->currentText());

    if (  (ostype == "1") ||  (ostype == "2")  || (ostype == "3") )
     {
           splashButton_other();
       }

       else

  if (check_devices() )
      {
       splashButton_android();
      }

}

//////////////////////////////////////////////////
void MainWindow::on_actionView_Kodi_Log_triggered()
{

    getRecord(ui->deviceBox->currentText());

       if (ostype != "0")
        {
            otherLog();
       }

       else

  if (check_devices() )
      {
        androidLog();
      }




}

//////////////////////////////////////////////////
void MainWindow::androidLog()
{

    if (!check_devices() )
        return;




    if (!xbmcpackage.contains("kodi")  && !xbmcpackage.contains("spmc") )
      {
       qDebug() << xbmcpackage;
        
        QMessageBox::critical(this,"","Use File Manager to view logs");
        return;
       }


    QString cstring = getadb()+" shell "+busypath+"find "+data_root+"Android/data/"+xbmcpackage+" -name *.log";
    QString command=RunProcess(cstring);
    logfile(command);

    if (!command.contains("kodi.log") && !command.contains("spmc.log")          )
     {
        QMessageBox::critical(0,"","Log not found.");
        return;
      }


    logfile("opening kodi log");
    klogDialog klogdialog;
    klogdialog.passdata(getadb(),data_root,filepath,xbmcpackage);
    klogdialog.setModal(true);
    klogdialog.exec();

}


//////////////////////////////////////////////////
void MainWindow::otherLog()
{


   // iOS/ATV2	/private/var/mobile/Library/Preferences/kodi.log
   // Linux	$HOME/.kodi/temp/kodi.log
  //   Mac OS X	/Users/<username>/Library/Logs/kodi.log
  //  Windows	%APPDATA%\Kodi\kodi.log


    logfile("opening kodi log");
    oslogDialog oslogdialog;
    oslogdialog.ospassdata(ostype);
    oslogdialog.setModal(true);
    oslogdialog.exec();

}


////////////////////////////////////////////////////
void MainWindow::on_actionView_adbLink_Log_triggered()
{
    adblogDialog dialog;
    dialog.setModal(true);
    dialog.exec();

}

/////////////////////////////////////////////
void MainWindow::on_screencapButton_clicked()
{


    if (!check_devices() )
        return;

     QElapsedTimer rtimer;
     int nMilliseconds;
     rtimer.start();

         QDateTime dateTime = QDateTime::currentDateTime();
         QString dtstr = dateTime.toString("MMddyyhhmmss");

         QString cstring = getadb() + " shell screencap -p " +tempdir+dtstr+".png";


     QString command=RunProcess(cstring);

     if (!command.isEmpty())

     {
         // logfile(cstring);
         logfile(command);

         QMessageBox::critical(
                     this,
                    "",
                     "Screenshot failed");
     }

     else
     {

         // logfile(cstring);
         logfile(command);

         QString cstring = getadb() + " pull "+tempdir+dtstr+".png "  +'"'+pulldir+'"';

         command=RunProcess(cstring);


         logfile(command);


         if (!command.contains("bytes"))
          {

             logfile(command);
             QMessageBox::critical(
                         this,
                        "",
                         "Screenshot failed");
         }
         else
         {
             // logfile(cstring);
             logfile(command);

             QMessageBox::information(
                            this,
                           "",
                            "Screenshot "+dtstr+ " copied to "+pulldir);
         }

         cstring = getadb() + " shell rm "+tempdir+dtstr+".png " ;
         command=RunProcess(cstring);
         // logfile(cstring);


     }

     nMilliseconds = rtimer.elapsed();
     logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );



}

///////////////////////////////////////////////
bool MainWindow::is_busybox()
{

QString cstring;
QString command;
busypath="/data/local/tmp/adblink/";

cstring = getadb() + " shell ls /data/local/tmp/adbfire";
command=RunProcess(cstring);


if (!command.contains("No such file or directory"))
   {
    cstring = getadb() + " shell rm -r /data/local/tmp/adbfire";
    command=RunProcess(cstring);
   }


cstring = getadb() + " shell ls /data/local/tmp/adblink";
command=RunProcess(cstring);


if (command.contains("No such file or directory"))
   {
    cstring = getadb() + " shell mkdir -p /data/local/tmp/adblink";
    command=RunProcess(cstring);
   }

  cstring = getadb() + " shell ls /data/local/tmp/adblink/busybox";
  command=RunProcess(cstring);


  if (command.contains("No such file or directory"))
     {

        QString busybox;

        /*
        cstring = getadb() + " shell getprop ro.product.cpu.abi";
           command=RunProcess(cstring);

            if (command.contains("x86"))
                busybox = adbdir+"busybox.zzz";
            else
                busybox = adbdir+"busybox.arm";
          */

           busybox = adbdir+"busybox";

      cstring = getadb() + " push "+busybox+ " /data/local/tmp/adblink/busybox";

      // cstring = getadb() + " push "+busybox+ " /data/local/tmp/adblink/";

      QString command=RunProcess(cstring);
      if (!command.contains("bytes"))
        {
           logfile("busybox install failed ");
           logfile(command);
           busypath="";
           QMessageBox::critical(0,"","busybox install failed. See log.");
           return false;
          }
     else
      {


          logfile(command);

          cstring = getadb() + " shell chmod 755 /data/local/tmp/adblink/busybox";
          command=RunProcess(cstring);

          // logfile(cstring);
          logfile(command);

           cstring = getadb() + " shell /data/local/tmp/adblink/busybox --install -s /data/local/tmp/adblink";
           command=RunProcess(cstring);

           // logfile(cstring);
           logfile(command);

      }


  }

  return true;

}


////////////////////////////////////////////
void MainWindow::on_backupButton_clicked()

{

    getRecord(ui->deviceBox->currentText());

       if (  (ostype == "1") ||  (ostype == "2")  || (ostype == "3") )
        {
           backupOther();
       }

       else



  if (check_devices() )
      {
         backupAndroid();
      }

}


/////////////////////////////////////////////
void MainWindow::on_restoreButton_clicked()

{



         getRecord(ui->deviceBox->currentText());

        if (  (ostype == "1") ||  (ostype == "2")  || (ostype == "3") )

            restoreOther();

       else

  if (check_devices() )
      {
        restoreAndroid();
      }



}

///////////////////////////////////////////
void MainWindow::backupOther()
{


    QString filecheck = filepath+"/userdata";


   if(!QDir(filecheck).exists())
    {
       QMessageBox::information(this,"",filecheck+" not found. Can't proceed.");
       logfile("BACKUP: "+ filecheck+" not found. Can't proceed.");
       return;
    }

    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Backup Folder"),
                                                  hdir,
                                                  QFileDialog::ShowDirsOnly
                                        | QFileDialog::DontResolveSymlinks);

   if (!dir.isEmpty() )
    {
     QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Backup", "backup to "+dir+"?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No)
      {
          return;
      }
   }


logfile("Local backup");



ui->progressBar->setHidden(false);
ui->progressBar->setValue(0);

QTimer *timer = new QTimer(this);
connect(timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
timer->start(tsvalue);


   ui->listRunningJobs->addItem("Local backup");


QFuture <void> future1 = QtConcurrent::run(this,&MainWindow::copyFolder,filepath,dir);
 watcher1.setFuture(future1);




}



///////////////////////////////////////////
void MainWindow::restoreOther()
{


    QMessageBox::critical(this,"","Restore is for Android devices only.");
    return;




    if (ui->deviceBox->currentText().isEmpty())
     {
        QMessageBox::critical(this,"","No Non-Android record selected");
        return;
    }

    if (filepath.isEmpty())
     {
        QMessageBox::critical(this,"","No path to Media Center");
        return;
    }


    bool goodfolder = QDir(filepath+"/userdata").exists();

    if (!goodfolder)
     {
       QMessageBox::critical(this,"","Filepath missing userdata folder");
        return;
      }





    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Backup Folder"),
                                                  hdir,
                                                  QFileDialog::ShowDirsOnly
                                        | QFileDialog::DontResolveSymlinks);


     if (dir.isEmpty())
         return;


     // check for valid backup



     if (!QDir(dir+"/userdata").exists() )

       {
         QMessageBox::critical(0,"","Invalid backup. userdata folder not found." );
         return;
       }



     if (!QDir(dir+"/addons").exists() )

       {
         QMessageBox::critical(0,"","Invalid backup. addons folder not found." );
         return;
       }





    if (!dir.isEmpty() )
    {
     QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Restore", "restore from "+dir+"?\nThe current Kodi data folder will be backed up first.",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No)
      {
          return;
      }
   }


   bool oldbackup = QDir(filepath+".backup").exists();

   if (oldbackup )
    {
     QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Restore", "Delete old backup?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No)
      {
        
         QMessageBox::critical(this,"","Local restore canceled");
          return;
      }
      else
       {
          QDir oldbackup(filepath+".backup");
          oldbackup.removeRecursively();
       }
   }



   logfile("Restoring");

   logfile("Local restore");



   ui->progressBar->setHidden(false);
   ui->progressBar->setValue(0);

   QTimer *timer = new QTimer(this);
   connect(timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
   timer->start(tsvalue);


      ui->listRunningJobs->addItem("Local restore");


 //  QFuture <void> future1 = QtConcurrent::run(this,&MainWindow::copyFolder,filepath,dir);
 //   watcher1.setFuture(future1);



   QFuture <void> future2 = QtConcurrent::run(this,&MainWindow::kodiFolder,dir,filepath);
   watcher2.setFuture(future2);


}





/////////////////////////////////////////////////////////////////////

void MainWindow::copyFolder(QString sourceFolder, QString destFolder)
{

QDir sourceDir(sourceFolder);
if(!sourceDir.exists())
return;

QDir destDir(destFolder);
if(!destDir.exists())
{
destDir.mkdir(destFolder);
}

QStringList files = sourceDir.entryList(QDir::Files);

for(int i = 0; i< files.count(); i++)
{
QString srcName = sourceFolder + "/" + files[i];
QString destName = destFolder + "/" + files[i];
QFile::copy(srcName, destName);
}

files.clear();

files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

for(int i = 0; i< files.count(); i++)
{
QString srcName = sourceFolder + "/" + files[i];
QString destName = destFolder + "/" + files[i];
copyFolder(srcName, destName);
}



}



/////////////////////////////////////////////////////////////////////

void MainWindow::kodiFolder(QString sourceFolder, QString destFolder)
{


QDir sourceDir(sourceFolder);
QDir destDir(destFolder);

if(!sourceDir.exists())
return;

QDir backupfolder(destFolder+".backup");

  if (backupfolder.exists())
  {
     logfile("Kodi folder backup already exists. Delete or rename it.");
     return;
  }



 bool check = destDir.rename(destFolder, destFolder+".backup");

 if (!check)
 {
     logfile("Problem backing up local Kodi folder");
     return;
 }



if(!destDir.exists())
{
destDir.mkdir(destFolder);
}

QStringList files = sourceDir.entryList(QDir::Files);
for(int i = 0; i< files.count(); i++)
{
QString srcName = sourceFolder + "/" + files[i];
QString destName = destFolder + "/" + files[i];
QFile::copy(srcName, destName);
}
files.clear();
files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
for(int i = 0; i< files.count(); i++)
{
QString srcName = sourceFolder + "/" + files[i];
QString destName = destFolder + "/" + files[i];
copyFolder(srcName, destName);
}



}



/////////////////////////////////////////////
void MainWindow::finishedCopy1()
{


  logfile("Backup finished.");

    QString mstring =  "Local backup";

   for(int i = 0; i < ui->listRunningJobs->count(); ++i)
   {
       QString str = ui->listRunningJobs->item(i)->text();
      if (str==mstring)
          delete ui->listRunningJobs->item(i);
   }


if (ui->listRunningJobs->count() > 0 )
       {
        ui->progressBar->setHidden(false);
        ui->progressBar->setValue(0);
         }
   else
      {
        ui->progressBar->setHidden(true);
        ui->progressBar->setValue(0);
    }





 




QMessageBox::information(this,"","Local backup finished");


}



/////////////////////////////////////////////
void MainWindow::finishedCopy2()
{


  logfile("Restore finished.");

     // QString mstring =  "Local Kodi restore";
     QString mstring =  "Local restore";
   for(int i = 0; i < ui->listRunningJobs->count(); ++i)
   {
       QString str = ui->listRunningJobs->item(i)->text();
      if (str==mstring)
          delete ui->listRunningJobs->item(i);
   }



   if (ui->listRunningJobs->count() > 0 )
          {
           ui->progressBar->setHidden(false);
           ui->progressBar->setValue(0);
            }
      else
         {
           ui->progressBar->setHidden(true);
           ui->progressBar->setValue(0);
          }



  
       QMessageBox::information(this,"","Local restore finished");

}


////////////////////////////////////////////////////
void MainWindow::on_actionEdit_XML_files_triggered()
{

    getRecord(ui->deviceBox->currentText());

    if (  (ostype == "1") ||  (ostype == "2")  || (ostype == "3") )
     {
           editOther();
       }

       else

  if (check_devices() )
      {
        editAndroid();
      }


}

//////////////////////////////////////////////////////////
void MainWindow::on_actionPaste_path_triggered()
{
    QClipboard *pathClipboard = QApplication::clipboard();
    pathClipboard->setText("PATH=$PATH:/data/local/tmp/adblink\n");
}


//////////////////////////////////////////////////////////
void MainWindow::usbbuttons(bool isusb)
{

 return;

    if(!isusb)
           {
              ui->connButton->setDisabled(false);
              ui->disButton->setDisabled(false);
              }
          else
              {
                ui->connButton->setDisabled(true);
                ui->disButton->setDisabled(true);
              }

}


//////////////////////////////////////////////////////

void MainWindow::on_actionStop_Application_triggered()
{
   isConnected=check_Connection();

      if (!isConnected)
            { 
               QMessageBox::critical(this,"",devstr2);
             return;
            }


    forcequitDialog dialog(false);

    if(dialog.exec() == QDialog::Accepted)
    {
    QString cstring = getadb() + " shell am force-stop "+dialog.packagename();
    QString command=RunProcess(cstring);
    logfile(command);
    }


}

/////////////////////////////////////////////////////////

void MainWindow::on_actionStart_Application_triggered()
{

    isConnected=check_Connection();

      if (!isConnected)
            { 
               QMessageBox::critical(this,"",devstr2);
              return;
            }



    forcequitDialog dialog(true);

    if(dialog.exec() == QDialog::Accepted)
    {

        QString cstring = getadb() + " shell am start -n "+dialog.packagename();
        QString command=RunProcess(cstring);
        logfile(command);

    }

}


////////////////////////////////////////////////

void MainWindow::on_keypadButton_clicked()
{

    if (!check_devices() )
        return;

     QString cstring = getadb() + " shell input keyevent ";
     keyboardDialog dialog;
     dialog.setdaddressLabel(cstring);
     dialog.exec();

}



///////////////////////////////////////////////////////

void MainWindow::on_fmButton_clicked()
{

    if (!check_devices() )
        return;

     busybox_permissions();

    getRecord(description);


    fmdialog = new usbfileDialog(this);
    fmdialog->setModal(false);
    fmdialog->setkodiPath(data_root+"Android/data/"+xbmcpackage+filepath);
    fmdialog->setData(description);
    fmdialog->setADB(getadb());
    fmdialog->setPath1("/sdcard/");
    fmdialog->setPath2("/sdcard/");
    fmdialog->setPulldir(pulldir);
    fmdialog->setdisableroot(disableroot);


   // fmdialog->setPulldir(getdownloadpath());

    fmdialog->setAdbdir(adbdir);

    fmdialog->show();



}





///////////////////////////////////////////
void MainWindow::editOther()
{


    QString mcpath=filepath;

     mcpath=mcpath+"/userdata/";


    if (!QFileInfo::exists(mcpath))
    {
         QMessageBox::information(this,"","userdata folder not found" );
         return;
    }


     QString fileName = QFileDialog::getOpenFileName(this,
    "Choose XML file", mcpath, tr("Files (*.xml)"));



     QFile file1(fileName);

     if(!file1.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     if (!fileName.isEmpty() )
     {




                 QFileInfo fileInfo(fileName);
                 QString filename(fileInfo.fileName());
                 QString xmlfile = file1.readAll();
                 editorDialog dialog;
                 dialog.seteditor(xmlfile);
                 dialog.setfilename(filename);

                dialog.setModal(true);

                  if(dialog.exec() == QDialog::Accepted)
                    {


                      QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, "Save","Save "+ fileName+"?",
                            QMessageBox::Yes|QMessageBox::No);
                        if (reply == QMessageBox::No)
                          return;


                      xmlfile = dialog.xmlfile();





                       //QMessageBox::information(this,"",xmlfile);


                       QFile::copy(fileName, fileName+".bak");

                       QFile caFile(fileName);
                           caFile.open(QIODevice::WriteOnly | QIODevice::Text);


                           QTextStream outStream(&caFile);
                           outStream << xmlfile;
                           caFile.close();

              }

   }


}




////////////////////////////////////////////////////////
void MainWindow::editAndroid()
{

    if (!check_devices() )
        return;

    QString tempfile1;
    QString tempfile2;
     QString xpath = "";
     QString fileName;
     QString cstring;
    QString command;
    QString mcpath=data_root+ "Android/data/"+xbmcpackage+filepath;


       xpath = mcpath+"/userdata/";



      cstring = getadb() + " shell "+busypath+"busybox find " +xpath+ " -maxdepth 1 -name *.xml ";
      command=RunProcess(cstring);

      //qDebug() << command;

      QStringList filelist=command.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

     if (command.isEmpty() || command.contains("No such file or directory"))
        { QMessageBox::critical(this,"","No files found");

         // logfile(cstring);
         logfile(command);
         logfile("no files found!");
         return;
         }

         //qDebug() << filelist;


         listfileDialog fdialog;
         fdialog.setFilelist(filelist);
         fdialog.setDialogTitle("XML Files");

if(fdialog.exec() == QDialog::Accepted)
 {

    fileName = fdialog.return_fitem();



     if (fileName.isEmpty())
        {
         QMessageBox::critical(this,"","No file selected");
         logfile("no file selected");
         return;
         }



     QString filename(fileName.mid(fileName.lastIndexOf("/")+1,fileName.length()));

             cstring = getadb() + " pull "+'"'+fileName+'"'+" "+'"'+adbdir+'"'+"/"+filename;
             command=RunProcess(cstring);


             if (!command.contains("bytes"))
              {
                 logfile("edit failed");
                 logfile(command);
                 QMessageBox::critical(
                             this,
                            "",
                             "Edit failed "+command);
                 return;
             }



             QFile file1(adbdir+filename);

             if(!file1.open(QIODevice::ReadOnly | QIODevice::Text))
                 return;

             QString xmlfile = file1.readAll();
             editorDialog dialog;
             dialog.seteditor(xmlfile);
             dialog.setfilename(filename);

             tempfile2 = adbdir+"/"+filename;
             tempfile1 = adbdir+"/"+filename+".bak";


              dialog.setModal(true);

              if(dialog.exec() == QDialog::Accepted)
                {


                  QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Save","Save "+ fileName+"?",
                        QMessageBox::Yes|QMessageBox::No);
                    if (reply == QMessageBox::No)
                      return;


                  xmlfile = dialog.xmlfile();


                   //QMessageBox::information(this,"",xmlfile);



                   QFile::copy(adbdir+"/"+filename, adbdir+filename+".bak");
                   QFile caFile( adbdir+"/"+filename);
                   caFile.open(QIODevice::WriteOnly | QIODevice::Text);
                   QTextStream outStream(&caFile);
                   outStream << xmlfile;
                   caFile.close();

                   cstring = getadb() + " push "+'"'+tempfile1+'"'+ " "+xpath;
                   command=RunProcess(cstring);
                   logfile(command);

                   if (!command.contains("bytes"))
                    {  QMessageBox::critical(this,"","Backup of "+filename+ "failed. Edit abandoned." );
                      logfile("Backup of "+filename+ "failed");
                       return;
                   }



                   cstring = getadb() + " push "+'"'+tempfile2+'"'+ " "+xpath;
                   command=RunProcess(cstring);
                   logfile(command);

                   if (!command.contains("bytes"))
                    {  QMessageBox::critical(this,"","Problem replacing "+filename+ ". Edit abandoned." );
                      logfile("Problem replacing "+filename+ ". Edit abandoned." );
                       return;
                   }


              }



              QFile file2 (tempfile1);
              file2.remove();

              QFile file3 (tempfile2);
              file3.remove();

}


}


void MainWindow::on_editButton_clicked()
{
    on_actionEdit_XML_files_triggered();
}


///////////////////////////////////////////////
QString MainWindow::RunLongProcess(QString cstring, QString jobname)
{

ui->progressBar->setHidden(false);
ui->progressBar->setValue(0);

QString s = description+" "+jobname;
RunProcessList << s;
ui->listRunningJobs->addItem(s);
ui->listRunningJobs->setFocus();

QTimer *timer = new QTimer(this);
connect(timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
timer->start(tsvalue);

QProcess run_long_command;
run_long_command.setProcessChannelMode(QProcess::MergedChannels);
run_long_command.start(cstring);
run_long_command.waitForStarted();


// int pnumber = run_long_command.processId();
// QString s = QString::number(pnumber);

 //s = description+" "+jobname+":"+s;
 //RunProcessList << s;
// ui->listRunningJobs->addItem(s);


 while(run_long_command.state() != QProcess::NotRunning)
   qApp->processEvents();

 QString command=run_long_command.readAll();

 RunProcessList.removeAll(s);

   ui->listRunningJobs->clear();

   for (QStringList::const_iterator it = RunProcessList.begin();
            it != RunProcessList.end(); ++it)
           {
            QString current = *it;
              ui->listRunningJobs->addItem(current);
           }


  if (RunProcessList.count() > 0 )
    {  //activityIcon(true);
      ui->progressBar->setHidden(false);
      ui->progressBar->setValue(0);
     }
  else
    {  //activityIcon(false);
       ui->progressBar->setHidden(true);
       ui->progressBar->setValue(0);
  }

 return command;
}


// qint64 id = run_long_command.pid();
// qDebug() << QString::number(id);
// QDateTime::fromTime_t(10).toUTC().toString("hh:mm:ss")
//qDebug() << RunProcessList;
//connect(run_long_command, SIGNAL(finished(int)), this, SLOT( endprocess() ) );


 /*

///////////////////////////////////////////////
void MainWindow::activityIcon(bool value)
{


    if (!value)
   {
        ui->mactivity->setScaledContents( true );
        ui->mactivity->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
        QPixmap pix1(":/assets/adblink.png");
        ui->mactivity->setStyleSheet("image: url(:/assets/adblink.png);");
        ui->mactivity->setPixmap(pix1);}
    else

    {
        ui->mactivity->setScaledContents( true );
        ui->mactivity->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
        QPixmap pix1(":/assets/colorlink.png");
        ui->mactivity->setStyleSheet("image: url(:/assets/colorlink.png);");
        ui->mactivity->setPixmap(pix1);
    }



}

*/

/////////////////////////////////////////////
void MainWindow::finishedLocalBackup()
{

  logfile("Backup finished.");

  QString mstring =  "Local Kodi backup";

 for(int i = 0; i < ui->listRunningJobs->count(); ++i)
 {
     QString str = ui->listRunningJobs->item(i)->text();
    if (str==mstring)
        delete ui->listRunningJobs->item(i);
 }



}



////////////////////////////////////////////////////////
void MainWindow::data_external(QString external_Location)
{


    QString cstring;
    QString command;
    //bool movecomplete=false;

    QString mcfiles=xbmcpackage+"/files/";
    QString root = external_Location;


    external_Location=external_Location + "Android/data/";

    cstring = getadb() +" shell ls "+root;
    command=RunProcess(cstring);

    if (command.contains("No such file or directory"))
      {



        //cstring = getadb() + " shell mkdir -p /data/local/tmp/adblink";
        //command=RunProcess(cstring);
        //cstring = getadb() + " push "+busybox+ " /data/local/tmp/adblink/busybox";



           QMessageBox::critical(this,"","Can't find "+root);
           logfile(command);
           return;
       }



   cstring = getadb() +" shell ls /sdcard/Android/data/"+xbmcpackage+filepath;
   command=RunProcess(cstring);

    if (command.contains("No such file or directory"))
      {
         QMessageBox::critical(this,"","Kodi data not found on /sdcard/  "+cstring);
         logfile(command);
          return;
    }



    int i =  filepath.indexOf(".");
    QString hidden = filepath.mid(i);

   cstring = getadb() +" shell ls /sdcard/xbmc_env.properties";
   command=RunProcess(cstring);

    if (!command.contains("No such file or directory"))
       {
        QMessageBox::critical(this,"","xbmc_env.properties already exists");
        return;
    }

    cstring = getadb() +" shell ls "+external_Location+"/mcfiles/"+hidden;
    command=RunProcess(cstring);

    if (!command.contains("No such file or directory"))
      {
          QMessageBox::critical(this,"","Data found at external location. Already moved?");
           logfile(command);
           return;
       }

    QMessageBox::StandardButton reply1;
       reply1 = QMessageBox::question(this, "", "Move data to "+root+"?",
                                    QMessageBox::Yes|QMessageBox::No);
       if (reply1 == QMessageBox::No)
         {
           return;}

logfile("Moving kodi data to external storage");

   QElapsedTimer rtimer;
   int nMilliseconds;
   rtimer.start();



   cstring = getadb() +" shell ls "+root+"/Android/data";
   command=RunProcess(cstring);

   if (command.contains("No such file or directory"))
     {

       cstring = getadb() + " shell mkdir -p "+root+"/Android/data";
       command=RunProcess(cstring);

      }





   cstring = getadb() +" shell cp -R /sdcard/Android/data/"+xbmcpackage + " /"+ external_Location+"/";

    //command=RunLongProcess(cstring,"Moving Kodi data to "+root);
    command=RunLongProcess(cstring,"Moving data to "+root);


   if (command.isEmpty())
    {

       cstring = getadb() + " shell echo xbmc.data="+external_Location+mcfiles+ " > /sdcard/xbmc_env.properties";
       command=RunProcess(cstring);

       QMessageBox::StandardButton reply2;
          reply2 = QMessageBox::question(this, "", "Delete old Kodi data?",
                                       QMessageBox::Yes|QMessageBox::No);
          if (reply2 == QMessageBox::Yes)
            {
              cstring = getadb() +" shell rm -r /sdcard/Android/data/"+xbmcpackage;
              command=RunLongProcess(cstring,"Removing old data");
          }

        //movecomplete=true;
       logfile("Data moved to external storage");
       data_root=root;
       updateDevice();
      
       QMessageBox::information(this,"","Kodi data moved to "+root);
   }

   else
  {
       QMessageBox::critical(this,"","Move error: "+ command);
       logfile("Move error: "+ command);
   }

   nMilliseconds = rtimer.elapsed();
  logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );



}

/////////////////////////////////////////////////////////
void MainWindow::data_internal(QString external_Location)
{



    QString cstring;
    QString command;
    QString mcfiles= xbmcpackage +"/files/";
    QString root = external_Location;


    external_Location=external_Location + "Android/data/";


    int i =  filepath.indexOf(".");
    QString hidden = filepath.mid(i);


    cstring = getadb() +" shell ls "+root;
    command=RunProcess(cstring);

    if (command.contains("No such file or directory"))
      {
         
           QMessageBox::critical(this,"","Root path missing");
            logfile(command);
                return;
       }



    cstring = getadb() +" shell ls "+external_Location+mcfiles+hidden;
    command=RunProcess(cstring);


    if (command.contains("No such file or directory"))
      {
           QMessageBox::critical(this,"","Kodi data not found "+cstring+" "+external_Location);
           logfile(command);
           return;
       }




    cstring = getadb() +" shell ls /sdcard/Android/data/"+xbmcpackage+filepath;
    command=RunProcess(cstring);

    if (!command.contains("No such file or directory"))
      {
          
            QMessageBox::critical(this,"","Kodi data has already been restored");
            logfile(command);
            return;
       }


    QMessageBox::StandardButton reply1;
       reply1 = QMessageBox::question(this, "", "Move Kodi data to internal storage?",
                                    QMessageBox::Yes|QMessageBox::No);
       if (reply1 == QMessageBox::No)
         {
           return;}

logfile("Moving kodi data to internal storage");

       QElapsedTimer rtimer;
       int nMilliseconds;
       rtimer.start();

       cstring = getadb() +" shell rm -r /sdcard/Android/data/"+xbmcpackage;
       command=RunProcess(cstring);


       logfile(command);


       cstring = getadb() +" shell cp -R "+external_Location+"/"+xbmcpackage + " /sdcard/Android/data/";
       //command=RunLongProcess(cstring,"Move Kodi data to /sdcard/");
       command=RunLongProcess(cstring,"Move data to /sdcard/");

       logfile(command);

       cstring = getadb() +" shell ls /sdcard/Android/data/"+xbmcpackage+filepath;
       command=RunProcess(cstring);

       if (!command.contains("No such file or directory"))
         {


           cstring = getadb() + " shell rm /sdcard/xbmc_env.properties";
           command=RunProcess(cstring);

           logfile(command);


           QMessageBox::StandardButton reply2;
              reply2 = QMessageBox::question(this, "", "Delete external Kodi data?",
                                           QMessageBox::Yes|QMessageBox::No);
              if (reply2 == QMessageBox::Yes)
                {
                  cstring = getadb() +" shell rm -r "+external_Location+xbmcpackage;
                  command=RunLongProcess(cstring,"Deleting external data");

              }





           logfile("Data moved to internal storage");
           data_root="/sdcard/";
           updateDevice();
          
          QMessageBox::information(this,"","Data moved to /sdcard/");
       }


       else
       {
         
            QMessageBox::critical(this,"","Error moving data to /sdcard/. See log");
       }



     nMilliseconds = rtimer.elapsed();
     logfile("process time duration: "+ QString::number(nMilliseconds/1000)+ " seconds" );





}



//////////////////////////////////////////////
void MainWindow::on_mvdataButton_clicked()
{

    if (!check_devices() )
        return;

    QString cstring = getadb() + " shell ps | grep "+xbmcpackage;
    QString command=RunProcess(cstring);


    if (command.contains(xbmcpackage))
       {
      
         QMessageBox::critical(this,"","Cannot move data while\n"+xbmcpackage+" is running!");
        return;
        }



    dataDialog dialog;

    dialog.setadb_data(getadb());

    dialog.setadb_dir(adbdir);

     dialog.setModal(true);

    if(dialog.exec() == QDialog::Accepted)
    {

        QString external_Location = dialog.externalLocation();

        if(!external_Location.startsWith("/"))
           external_Location.prepend("/");

        if(!external_Location.endsWith("/"))
           external_Location.append("/") ;

        int x = dialog.returnval2();


      switch(x){
        case 1:
        data_external(external_Location);
        break;

        case 2:
        data_internal(external_Location);
        break;

        default:
         data_external(external_Location);
        break;   }


    }

}



//////////////////////////////////////////////

void MainWindow::updateDeviceRecord()
{

    QString tempstring;
    QString sqlstatement;
    QSqlQuery query;
   // QString quote = "\"";

   // qDebug() << data_root;
    //qDebug() << description;

     data_root="VVVV";

     tempstring = data_root;

     //tempstring.replace(QString("'"), QString("''"));


      sqlstatement = "UPDATE device SET data_root='"+tempstring+"'  WHERE description="+'"'+ui->deviceBox->currentText()+'"';


     logfile(sqlstatement);

      query.exec(sqlstatement);

      if (query.lastError().isValid())
       {
         logfile(sqlstatement);
         logfile("SqLite error:" + query.lastError().text());
         logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
        }


}


////////////////////////////////////////////////////////

void MainWindow::on_actionMount_system_RO_triggered()
{


   isConnected=check_Connection();

    if (!isConnected)
          { 
             QMessageBox::critical(this,"",devstr2);
            return;
          }

    if (!is_su())
       {  
         QMessageBox::critical(this,"","Root required!");
        return;
       }


    if (mount_system("ro"))
     {

        logfile("/system partition is read-only");
        
        QMessageBox::information(this,"","/system partition is read-only");
     }

     else

    {
        logfile("filesystem not remounted r/o!");
        
         QMessageBox::critical(this,"","filesystem not remounted r/o!");
    }


}

////////////////////////////////////////////////////////

void MainWindow::on_actionMount_system_RW_triggered()
{

    isConnected=check_Connection();

    if (!isConnected)
          { 
            QMessageBox::critical(this,"",devstr2);
            return;
          }


    if (!is_su())
    {  
        QMessageBox::critical(this,"","Root required!");
    return;
    }



    if (mount_system("rw"))

       {

        logfile("/system partition is read-only");
        
        QMessageBox::information(this,"","/system partition is read-write");
    }
     else

    {
        logfile("filesystem not remounted r/o!");
          
            QMessageBox::critical(this,"","filesystem not mounted read-write");
    }


}

///////////////////////////////////////////////////////

void MainWindow::on_actionWireless_ADBD_triggered()
{
    isConnected=check_Connection();

    if (!isConnected)
          { 
              QMessageBox::critical(this,"",devstr2);
        return;
          }


 //   if (!isusb)
 //         { 
  //            QMessageBox::critical(this,"","This function requires a USB connection");
     //      return;
  //        }

   /*

setprop service.adb.tcp.port 5555
stop adbd
start adbd

setprop service.adb.tcp.port -1
stop adbd
start adbd

*/



   // QString tcpstatus = getadb() +   " shell getprop service.adb.tcp.port";
  //   QString tcpstatus = getadb() +   " shell getprop persist.adb.tcp.port 5555";
// QString tcpstatus = getadb() +   " shell getprop persist.adb.tcp.port -1";
    // QString tcpstatus = getadb() +   " usb";

   tcpipDialog dialog;

     //  QString tcpstatus = getadb() +   " shell getprop service.adb.tcp.port";


   QString tcpstatus = getadb() +   " shell getprop persist.adb.tcp.port 5555";
       QString command=RunProcess(tcpstatus);

      // qDebug() << command;

       if (command.contains("5555"))
         dialog.settcplabel("ADB/WIFI is enabled");
       else
        dialog.settcplabel("ADB/WIFI is disabled");

       if (port=="")
           port="5555";

       QString cstring = getadb() +  " tcpip "+port;
       command=RunProcess(cstring);

        if(dialog.exec() == QDialog::Accepted)
         {
            QString command=RunProcess(cstring);
            logfile(command);
          }
}


////////////////////////////////////////////////////////

void MainWindow::on_actionKodi_data_usage_triggered()
{

 isConnected=check_Connection();


          backupDialog dialog;

         QString n_data_root;

          dialog.setadb_backup(getadb(),data_root);

           dialog.setModal(true);

           dialog.setWindowTitle("Kodi Data Size");

       if(dialog.exec() == QDialog::Accepted)
       {



           n_data_root = dialog.return_data_root();

            if(!n_data_root.startsWith("/"))
               n_data_root.prepend("/");

            if(!n_data_root.endsWith("/"))
               n_data_root.append("/") ;


            QString cstring = getadb() + " shell du -sh " + n_data_root +"Android/data/"+xbmcpackage;
            QString command=RunLongProcess(cstring,"Data size");

             QString kodidata;

            int z = command.indexOf("G");

            if (z==-1)
                z = command.indexOf("M");

            if (z==-1)
                z = command.indexOf("K");


            if (z != -1)
             kodidata = command.mid(0,z+1);
            else
            kodidata = "No Kodi data found";


            cstring = getadb() + " shell df " + n_data_root;
            QString mystring=RunProcess(cstring);
            QStringList list=mystring.split(QRegExp("\\s"),QString::SkipEmptyParts);



            QMessageBox::information(0,"","Partition:  " + list[5]+"\n"+
                                          "Partition size:  " + list[6]+"\n"+
                                          "Kodi data size:  " + kodidata+"\n"+
                                          "Total space used:  " + list[7]+"\n"+
                                          "Free space:  " + list[8]
                    );


          }


}

////////////////////////////////////////////////////////////////////////

void MainWindow::on_deviceBox_currentIndexChanged(const QString &arg1)
{
    getRecord(arg1);

    if (ostype == "0")
        usbbuttons(isusb);
}


//////////////////////////////////////////////////////////////////

void MainWindow::on_listDevices_clicked(const QModelIndex &index)
{

    QString item=index.data(Qt::DisplayRole).toString();

    device_clicked(item);



}

//////////////////////////////////////////////

void MainWindow::on_listDevices_doubleClicked(const QModelIndex &index)
{
  on_fmButton_clicked();
}

////////////////////////////////

void MainWindow::on_clearButton_clicked()
{
    ui->adhocip->setText("");
}

//////////////////////////////////////////////////////

bool MainWindow::check_Connection()
{

bool result=false;

result=searchlistDevices(ui->deviceBox->currentText());

if (result)
    return true;

foreach( QListWidgetItem *item, ui->listDevices->selectedItems() )
 {

    if (item->text().contains(":"))
    {
        QStringList pieces = item->text().split( ":" ,QString::SkipEmptyParts);
        daddr=pieces.at(0);
        port=pieces.at(1);
        port="5555";
        isusb=false;
    }

   else

    {
     daddr=item->text();
     port="";
     isusb=true;
    }


    description=daddr;
    default_device_values();
    return true;

 }

return false;

}



//////////////////////////////////////////////////

void MainWindow::device_clicked(QString item)
{

     int hasrecord=ui->deviceBox->findText(item,Qt::MatchExactly );


     if (hasrecord>=0)
     {  ui->deviceBox->setCurrentText(item);
        getRecord(item);
     }
    else
     {

       default_device_values();
       description=item;

    if (item.contains(":"))
       {
        isusb=false;
        QStringList pieces = item.split( ":" ,QString::SkipEmptyParts);
        daddr=pieces.at(0);
        port = "5555";
         }
    else
         {
            daddr=item;
            isusb=true;
            port="";
         }


     }


}

void MainWindow::on_actionPreferences_triggered()
{


    adbprefDialog dialog;


    if (getadbPref())
    dialog.setversioncheck(true);
    else
     dialog.setversioncheck(false);

    if (getmsgboxtype())
    dialog.setmsgboxtype(true);
    else
     dialog.setmsgboxtype(false);


    dialog.setdownloaddir(getdownloadpath());

    dialog.setversionLabel(version);


    dialog.setModal(true);


    if(dialog.exec() == QDialog::Accepted)
    {

        bool vcheck = dialog.versioncheck();
        bool mcheck = dialog.msgboxtype();
        QString ddir = dialog.downloaddir();

        if (vcheck)
         updateckversion(1);
        else
        updateckversion(0);

      if (mcheck)
        updateMsgboxtype(1);
      else
     updateMsgboxtype(0);

     updatedownloaddir(ddir);


    }


}


////////////////////////////////

bool MainWindow::getadbPref()

{


    QString sqlstatement;

    QSqlQuery query;

     QString tmpstr ;

     int ckver;

     bool vercheck;

             sqlstatement= "SELECT  checkversion FROM preferences";
             query.exec(sqlstatement);



                     while (query.next()) {
                           tmpstr = query.value(0).toString();
                           //qDebug() << tmpstr;
                     }


                       if (query.lastError().isValid())
                        {
                          logfile(sqlstatement);
                          logfile("SqLite error:" + query.lastError().text());
                          logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
                         }

      ckver=tmpstr.toInt();

     if (ckver==0)
         vercheck=false;
     else
         vercheck=true;


     return vercheck;

}


//////////////////////////////////////

bool MainWindow::getmsgboxtype()

{


    QString sqlstatement;

    QSqlQuery query;

     QString tmpstr ;

     int mver;

     bool mcheck;

             sqlstatement= "SELECT  msgboxtype FROM preferences";
             query.exec(sqlstatement);



                     while (query.next()) {
                           tmpstr = query.value(0).toString();
                           //qDebug() << tmpstr;
                     }


                       if (query.lastError().isValid())
                        {
                          logfile(sqlstatement);
                          logfile("SqLite error:" + query.lastError().text());
                          logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
                         }

      mver=tmpstr.toInt();

     if (mver==0)
         mcheck=false;
     else
         mcheck=true;


     return mcheck;

}



//////////////////////////////////////////

void MainWindow::updateckversion(int value)
{

    QString sqlstatement;
    QSqlQuery query;
    QString str;

  str.setNum(value);

    sqlstatement = "UPDATE preferences SET checkversion='"+str+"' ";
    logfile(sqlstatement);
    query.exec(sqlstatement);

     if (query.lastError().isValid())
      {
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
       }




}



//////////////////////////////////////////

void MainWindow::updateMsgboxtype(int value)
{

    QString sqlstatement;
    QSqlQuery query;
    QString str;

  str.setNum(value);

    sqlstatement = "UPDATE preferences SET msgboxtype='"+str+"'";
    logfile(sqlstatement);
    query.exec(sqlstatement);

     if (query.lastError().isValid())
      {
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
       }




}



//////////////////////////////////////////

void MainWindow::updatedownloaddir(QString str)
{

    QString sqlstatement;
    QSqlQuery query;


    sqlstatement = "UPDATE preferences SET downloaddir='"+str+"'";
    logfile(sqlstatement);
    query.exec(sqlstatement);

     if (query.lastError().isValid())
      {
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
       }




}





//////////////////////////////////////

QString MainWindow::getdownloadpath()

{

    QSqlQuery query;
    QString sqlstatement;
   QString downloaddir;

    sqlstatement= "SELECT downloaddir FROM preferences WHERE Id=1";

    query.prepare( sqlstatement );

    if (!query.exec()){
        logfile(sqlstatement);
        logfile("SqLite error:" + query.lastError().text());
        logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
    }
    else
    {
        if( query.next( ) )
         downloaddir = query.value(0).toString();
    }


//qDebug() << downloaddir;

return downloaddir;


}


void MainWindow::on_actionDown_SPMC_triggered()
{


    QString link = "http://spmc.semperpax.com/";
    QDesktopServices::openUrl(QUrl(link));


}





////////////////////////////////////////////////

void MainWindow::on_actionDownload_Kodi_triggered()
{


    QString link = "https://kodi.tv/download";
    QDesktopServices::openUrl(QUrl(link));



}




///////////////////////////////////////////////

QString MainWindow::getDaddr(QString descrip)

{

    QString quote = "\"";
    QString mdaddr;
    QString sqlstatement;
    descrip = quote+descrip+quote;
    QSqlQuery query;



             sqlstatement= "SELECT daddr FROM device WHERE description=" + descrip;
             query.exec(sqlstatement);
             logfile(sqlstatement);


                     while (query.next()) {
                            mdaddr = query.value(0).toString();
                          }


                       if (query.lastError().isValid())
                        {
                          logfile(sqlstatement);
                          logfile("SqLite error:" + query.lastError().text());
                          logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
                         }


return mdaddr;


}

////////////////////////////////////////////////

void MainWindow::on_adhocip_returnPressed()
{
    on_connButton_clicked();
}




///////////////////////////////////////////////////////

bool MainWindow::check_devices()
{



    if (ui->listDevices->count() <= 0)
     {
         
         QMessageBox::critical(this,"","No connected devices");
        return false;
    }

    int selectedcount = ui->listDevices->selectedItems().count();

           if (selectedcount > 0)
              foreach( QListWidgetItem *item, ui->listDevices->selectedItems() )
                  {
                     QString mitem = item->text();
                     device_clicked(mitem);
                  }
           else

           {
               QListWidgetItem* item = ui->listDevices->item(0);
               QString mitem = item->text();
               device_clicked(mitem);
           }


   if(!is_busybox())
   {
       QMessageBox::critical(0,"","Busybox installation failed. See log.");
      return false;
   }


 return true;

}


void MainWindow::on_actionGet_package_name_triggered()
{
   QString command;
   QString cstring;
   QStringList mstringlist;

    QString filename = QFileDialog::getOpenFileName(this,tr("APK files"),QDir::currentPath(),tr("APK files (*.apk);;All files (*.*)") );
    if( !filename.isEmpty() )
    {

        cstring = aapt + " dump badging  " + '"'+ filename+'"';
        command=RunProcess(cstring);
        mstringlist=command.split(QRegExp("[\t\n\r]"),QString::SkipEmptyParts);


       for (QStringList::iterator it = mstringlist.begin();
             it != mstringlist.end(); ++it)
                {
                   QString item=*it;
                    if (item.contains("package"))
                     {
                        QRegExp rx("(\\')");
                        QStringList query = item.split(rx);
                        QString packagename = query.at(1);
                         QMessageBox::information(0, "",packagename);
                       }
                }

    }



}



//////////////////////////////////////////////
void MainWindow::open_pref_database()

{


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbstring);


    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            "Database error:\n"+dbstring
                     , QMessageBox::Cancel);
     logfile("error opening database "+dbstring);

    }



}



////////////////////////////////////////////////

void MainWindow::add_disableroot()
{


QString sqlstatement;
QStringList fieldlist;
QSqlQuery query;



query.exec("PRAGMA table_info(device)");
  while (query.next()) {
     fieldlist << query.value(1).toString();
   }

if(!fieldlist.contains("disableroot"))
{

sqlstatement = "ALTER TABLE device ADD COLUMN disableroot int";

    query.exec(sqlstatement);

         if (query.lastError().isValid())
          {
            logfile(sqlstatement);
            logfile("SqLite error:" + query.lastError().text());
            logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
           }

         sqlstatement = "UPDATE device SET disableroot=0";


         query.exec(sqlstatement);

              if (query.lastError().isValid())
               {
                 logfile(sqlstatement);
                 logfile("SqLite error:" + query.lastError().text());
                 logfile("SqLite error code:"+ QString::number( query.lastError().number() ));
                }


}

}

////////////////////////////////////////////////////////

void MainWindow::on_actionReiinstall_Busybox_triggered()
{


    QString busybox;
    QString cstring;
    QString command;


    QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "", "Re-install Busybox?",
                                   QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::No)
     {

         return;
     }


/*
     cstring = getadb() + " shell getprop ro.product.cpu.abi";
     command=RunProcess(cstring);

      if (command.contains("x86"))
          busybox = adbdir+"busybox.zzz";
      else
          busybox = adbdir+"busybox.arm";
*/

      busybox = adbdir+"busybox";


    busypath="/data/local/tmp/adblink/";
    cstring = getadb() + " shell rm -r /data/local/tmp/adblink";
    command=RunProcess(cstring);

    cstring = getadb() + " shell mkdir -p /data/local/tmp/adblink";
    command=RunProcess(cstring);
    cstring = getadb() + " push "+busybox+ " /data/local/tmp/adblink/busybox";





          command=RunProcess(cstring);

          if (!command.contains("bytes"))
            {
               logfile("busybox install failed ");
               logfile(command);
               busypath="";
               QMessageBox::critical(0,"","busybox install failed. See log.");
               return;
              }
         else
          {
              logfile(command);
              cstring = getadb() + " shell chmod 755 /data/local/tmp/adblink/busybox";
              command=RunProcess(cstring);

              // logfile(cstring);
              logfile(command);

               cstring = getadb() + " shell /data/local/tmp/adblink/busybox --install -s /data/local/tmp/adblink";
               command=RunProcess(cstring);

               // logfile(cstring);
               logfile(command);
          }

      busybox_permissions();

       
       QMessageBox::information(this,"","Busybox re-installed.");

      return;


}





///////////////////////////////////////////////
void MainWindow::busybox_permissions()
{

QString cstring;
QString command;

cstring = getadb() + " shell chmod 755 /data/local/tmp/adblink/busybox";
command=RunProcess(cstring);

}



void MainWindow::on_actionArchitecture_triggered()
{


    if (!check_devices() )
        return;


    QString cstring;
    QString command;

    cstring = getadb() + " shell getprop ro.product.cpu.abi";
    command=RunProcess(cstring);
    QMessageBox::information(0,"",command);


}


void MainWindow::on_actionSleep_adjust_triggered()
{


  // sleep
  //  adb shell settings put secure sleep_timeout 123456789
  // screensaver
  //  adb shell settings put system screen_off_timeout 123456789


    // adb shell settings get secure sleep_timeout
   //   adb shell settings get system screen_off_timeout


   if (!check_devices() )
        return;

       QString cstring;
       QString command;




       //  dialog.setdownloaddir(getdownloadpath());

   //    dialog.setversionLabel(version);



       sleepDialog dialog;

       cstring = getadb() + " shell settings get secure sleep_timeout ";
       command=RunProcess(cstring);
        command = command.simplified();
       command.replace( " ", "" );
       dialog.setcurrentsleep("Current: "+command);

       cstring = getadb() + " shell settings get system screen_off_timeout ";
       command=RunProcess(cstring);
       command = command.simplified();
       command.replace( " ", "" );
       dialog.setcurrentscreen("Current: "+command);

       dialog.setdevicelabel("Device: "+description);

        if(dialog.exec() == QDialog::Accepted)
        {
            QString screenval = dialog.screenValue();
            QString sleepval = dialog.sleepValue();



           // qDebug() << screenval;
           // qDebug() << sleepval;

           // return;


            cstring = getadb() + " shell settings put secure sleep_timeout "+sleepval;
            command=RunProcess(cstring);

            cstring = getadb() + " shell settings put system screen_off_timeout "+screenval;
            command=RunProcess(cstring);


            cstring = getadb() + " shell settings get secure sleep_timeout ";
            QString sleep1=RunProcess(cstring);
            sleep1 = sleep1.simplified();
            sleep1.replace( " ", "" );


            cstring = getadb() + " shell settings get system screen_off_timeout ";
            QString screen1=RunProcess(cstring);
            screen1 = screen1.simplified();
            screen1.replace( " ", "" );



             QMessageBox::information(this,"","Timer values in milliseconds\nSleep: " + sleep1 + "\n" + "Screensaver: " + screen1 +"\n");

        }




}
