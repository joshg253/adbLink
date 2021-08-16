#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class searchDialog;
}

class searchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchDialog(QWidget *parent = 0);
    ~searchDialog();


    QString esearch1();
    QString esearch2();



private slots:
    void on_pushButton_2_clicked();

private:
    Ui::searchDialog *ui;
};

#endif // SEARCHDIALOG_H
