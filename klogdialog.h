#ifndef KLOGDIALOG_H
#define KLOGDIALOG_H

#include <QDialog>

namespace Ui {
class klogDialog;
}

class klogDialog : public QDialog
{
    Q_OBJECT

private:

   QString cstring;
   QString command;
   QString kfilepath;
   QString kpackage;
   QString kdataroot;
   QString kadbpath;
   QString kcontent;
   bool kgetfile = true;
   QString klogfile1 = "kodi.log";
   QString klogfile2 = "kodi.old.log";

public:
    void passdata(const QString &adbpath, const QString &dataroot, const QString &filepath, const QString xbmcpackage);

public:
    explicit klogDialog(QWidget *parent = 0);
    ~klogDialog();

private slots:
    void on_kswapButton_clicked();

    void on_kcopyButton_clicked();


private:
    Ui::klogDialog *ui;

};

#endif // KLOGDIALOG_H
