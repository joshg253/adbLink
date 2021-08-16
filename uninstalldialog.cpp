#include "uninstalldialog.h"
#include "ui_uninstalldialog.h"
#include <QProcess>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>
// #include <QDebug>

#ifdef Q_OS_LINUX
 int ost=0;
#elif defined(Q_OS_WIN)
  int ost=1;
#elif defined(Q_OS_MAC)
int ost=2;
#endif


QString tmpdir;
QString tmpstr;
QString commstr;
QString cstr;
QString argument;
QString adb2;
QString fline;

QProcess packages;


QString uninstallDialog::packageName() {

   if( ui->unlistWidget->selectedItems().count() == 1 )
        return ui->unlistWidget->currentItem()->text();
     else return "";

}

bool uninstallDialog::keepBox() {
   return ui->keepBox->isChecked();
}




//uninstallDialog::uninstallDialog(const QString &port, const QString &daddr,QWidget *parent) :
//   QDialog(parent),m_port(port),m_daddr(daddr),
//    ui(new Ui::uninstallDialog) {

    uninstallDialog::uninstallDialog(const QString &daddr, const QString &port,QWidget *parent) :
       QDialog(parent),m_daddr(daddr),m_port(port),
        ui(new Ui::uninstallDialog) {




  ui->setupUi(this);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);




    if (ost == 1)
       {
        tmpdir = "./";
         adb2 = tmpdir+"adb.exe";
       }


     if (ost == 2)
       {
         tmpdir = QCoreApplication::applicationDirPath();
         tmpdir = tmpdir+"/adbfiles/";
        adb2 = tmpdir+"adb";
       }

     if (ost == 0)
       {
         tmpdir = QCoreApplication::applicationDirPath();
         tmpdir = tmpdir+"/adbfiles/";
         adb2 = tmpdir+"adb";
       }



  ui->setupUi(this);

  this->setWindowTitle(daddr);


  QObject::connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(on_applyButton_clicked()));

  QObject::connect(ui->apkclearButton, SIGNAL(clicked()), this, SLOT(on_apkclearButton_clicked()));


   tmpstr = tmpdir+"tempfl";
   QString c1;


   loadList();
   makeFile();
   loadBox();

}

uninstallDialog::~uninstallDialog()
{
    delete ui;
}


void uninstallDialog::on_applyButton_clicked()
{

    ui->unlistWidget->clear();

  if (ui->lineEdit->text() != "" )
      {
         if (m_port.isEmpty())
           argument = " -s "+m_daddr+ " shell pm list packages | grep "+ui->lineEdit->text();
          else
           argument = " -s "+m_daddr+":"+m_port+" shell pm list packages | grep "+ui->lineEdit->text();

         cstr = adb2 + argument;
  }

    else loadList();

    makeFile();
    loadBox();

}


void uninstallDialog::loadList()
{
    if (m_port.isEmpty())
    argument = " -s "+m_daddr+ " shell pm list packages";
    else
    argument = " -s "+m_daddr+":"+m_port+" shell pm list packages";
    cstr = adb2 + argument;
}

void uninstallDialog::makeFile()
{
packages.start(cstr);
packages.waitForFinished(-1);
commstr=packages.readAll();

QFile file2(tmpstr);

  if(!file2.open(QFile::WriteOnly |
      QFile::Text))
 {
    QMessageBox::critical(this,"","Error creating file!");
     return;
  }

QTextStream out(&file2);
out  << commstr << endl;

file2.flush();
file2.close();

}

void uninstallDialog::loadBox()
{
    QFile file3(tmpstr);
      if (!file3.open(QIODevice::ReadOnly | QIODevice::Text))
        {QMessageBox::critical(this,"","Error reading file!");
           return; }

      QTextStream in(&file3);
       while (!in.atEnd())
        {


         fline = in.readLine();

           if (!fline.isEmpty())
            {

           fline.remove(0,8);


                    ui->unlistWidget->addItem(fline);

          }

       }

         file3.close();

      QFile::remove(tmpstr);
}

//void uninstallDialog::on_unlistWidget_itemClicked(QListWidgetItem *item)
//{
//   QMessageBox::critical(this,"","Error reading file!");
//}



void uninstallDialog::on_apkclearButton_clicked()
{
    ui->lineEdit->setText("");

}
