#ifndef RESTDIALOG_H
#define RESTDIALOG_H

#include <QDialog>

namespace Ui {
class restDialog;
}

class restDialog : public QDialog
{
    Q_OBJECT

public:
    explicit restDialog(QWidget *parent = 0);
    ~restDialog();

 public:
   QString restore_data_root();

   void setadb_restore(const QString &adb_restore);

private slots:

    void on_listDirectories_restore_clicked();

    QString on_listDirectories_restore_doubleClicked();

private:
    Ui::restDialog *ui;
};

#endif // RESTDIALOG_H
