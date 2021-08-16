#include "cachedialog.h"
#include "ui_cachedialog.h"
#include <QtConcurrent/QtConcurrent>
#include <qtconcurrentrun.h>

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include<QClipboard>


QClipboard *cacheclipboard = QApplication::clipboard();

int cval1;


QString cachecontent;

bool cacheDialog::xmlcheck() {
   return ui->xmlcheck->isChecked();
}


bool cacheDialog::cacheclip() {
   return cacheclipboard;
}

int cacheDialog::cbuffermode() {
   return ui->buffermode->currentIndex();
}

QString cacheDialog::cbufferfactor() {
   return ui->bufferfactor->text();
}

QString cacheDialog::cbuffersize() {
   return ui->buffersize->text();
}



void cacheDialog::setcbuffersize(const QString &cbuffersize)
{
    ui->buffersize->setText(cbuffersize);
}

void cacheDialog::setcbuffermode(const int &cbuffermode)
{

    ui->buffermode->setCurrentIndex(cbuffermode);
}

void cacheDialog::setcbufferfactor(const QString &cbufferfactor)
{
    ui->bufferfactor->setText(cbufferfactor);
}



void cacheDialog::setxmlcheck(const bool &xmlcheck)
{
    ui->xmlcheck->setChecked(xmlcheck);
}

int cacheDialog::returncval1() {
   return cval1;
}


cacheDialog::cacheDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cacheDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


}

cacheDialog::~cacheDialog()
{
    delete ui;
}


void cacheDialog::on_cpreset1_clicked()
{

    setcbuffermode(1);
    setcbuffersize("0");
    setcbufferfactor("10");

}

void cacheDialog::on_cpreset2_clicked()
{
    setcbuffermode(2);
    setcbuffersize("104857600");
    setcbufferfactor("1");
}

void cacheDialog::on_cpreset3_clicked()
{
    setcbuffermode(2);
    setcbuffersize("52428800");
    setcbufferfactor("1");

}


////////////////////////////////////

void cacheDialog::on_writecxml_clicked()
{
   cval1 = 2;
   QDialog::accept();
}

void cacheDialog::on_resetcxml_clicked()
{
     ui->buffersize->setText("20971520");
     ui->buffermode->setCurrentIndex(2);
     ui->bufferfactor->setText("1");

     ui->cpreset1->setAutoExclusive(false);
     ui->cpreset2->setAutoExclusive(false);
     ui->cpreset3->setAutoExclusive(false);

     ui->cpreset1->setChecked(false);
     ui->cpreset2->setChecked(false);
     ui->cpreset3->setChecked(false);

     ui->cpreset1->setAutoExclusive(true);
     ui->cpreset2->setAutoExclusive(true);
     ui->cpreset3->setAutoExclusive(true);

}

//////////////////////////////////////////////

void cacheDialog::on_copyxml_clicked()
{


    QString line1;
    QString line2;
    QString line3;
    QString line4;
    QString line5;
    QString line6;
    QString line7;

   bool oldxml = xmlcheck();

   QString buffersize = ui->buffersize->text();
   int buffermode = ui->buffermode->currentIndex();
   QString bufferfactor = ui->bufferfactor->text();


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

    QString cacheFilename = "temp.txt";
    QFile cacheFile(cacheFilename);
    cacheFile.open(QIODevice::ReadWrite);
    QTextStream out1(&cacheFile);

       out1  << line1 << endl;
       out1  << line2 << endl;
       out1  << line3 << endl;
       out1  << line4 << endl;
       out1  << line5 << endl;
       out1  << line6 << endl;
       out1  << line7 << endl;


    cacheFile.flush();
    out1.seek(0);
    QString temp = out1.readAll();
    cacheclipboard->setText(temp);
    cacheFile.close();



}
