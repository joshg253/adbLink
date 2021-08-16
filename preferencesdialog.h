#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class preferencesDialog;
}

class preferencesDialog : public QDialog
{
    Q_OBJECT


public:
    explicit preferencesDialog(QWidget *parent = 0 );

      void setPackagename(const QString &packagename);
      void setPulldir(const QString &pulldir);
    //  void setversioncheck(const bool &versioncheck);
      void setisusb(const bool &isusb);
      void setversionLabel(const QString &versiontext);
      void setostype(const QString &ostype);
      void setdaddr(const QString &daddr);
      void setdevicelist(const QStringList &dstringlist);
      void setdescription(const QString &description);
      void setfilepath(const QString &filepath);
      void setdataroot(const QString &data_root);
    //  void setdeviceindex(const int &deviceindex);
      void setport(const QString &port);
      void setadb_pref(const QString &adb_pref);
      void setrecnum(const QString &recnum);
      void setdisableroot(const bool &disableroot);

    ~preferencesDialog();

public:
   QString xbmcpackageName();

   QString pulldir();
   QString daddr();
   QString description();
   QString filepath();
   QString data_root();
   QString port();
   QString ostype();

   // bool versioncheck();

   bool isusb();

       bool disableroot();


int returnval1();

private slots:
   void on_pfolderButton_clicked();
   void onRequestCompleted();
   void on_pushButton_clicked();
   void on_kodiButton_clicked();
   void on_spmcButton_clicked();
   void on_otherButton_clicked();
   void on_isusb_clicked(bool checked);
   void on_listkodirootBox_clicked();
   void disable_ui();
   void enable_ui();

   void on_ostypeBox_currentIndexChanged(int index);

   void on_filepathButton_clicked();


   void on_listDevicesp_doubleClicked(const QModelIndex &index);


private:
    Ui::preferencesDialog *ui;



};

#endif // PREFERENCESDIALOG_H
