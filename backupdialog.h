#ifndef BACKUPDIALOG_H
#define BACKUPDIALOG_H

#include <QDialog>

namespace Ui {
class backupDialog;
}

class backupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit backupDialog(QWidget *parent = 0);
    ~backupDialog();

 public:

    QString return_data_root();
    QString returnthumb();

    void setadb_backup(const QString &adb_backup, const QString &data_root_backup);

private slots:
   void on_listDirectories_backup_clicked();

    void on_listDirectories_backup2_clicked();

    void on_okButton_clicked();

private:
    Ui::backupDialog *ui;
};

#endif // BACKUPDIALOG_H
