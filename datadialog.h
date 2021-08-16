#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <QDialog>

namespace Ui {
class dataDialog;
}

class dataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit dataDialog(QWidget *parent = 0);
    ~dataDialog();

public:
    int returnval2();
    QString externalLocation();
   void setadb_data(const QString &adb_data);
   void setadb_dir(const QString &adb_dir);



private slots:
    void on_dataButton1_clicked();

    void on_dataButton2_clicked();

    void on_dataButton3_clicked();

    void on_dataButton4_clicked();



    void on_listDirectories1_clicked(const QModelIndex &index);

private:
    Ui::dataDialog *ui;
};

#endif // DATADIALOG_H
