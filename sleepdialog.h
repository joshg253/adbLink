#ifndef SLEEPDIALOG_H
#define SLEEPDIALOG_H

#include <QDialog>

namespace Ui {
class sleepDialog;
}

class sleepDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sleepDialog(QWidget *parent = nullptr);
    ~sleepDialog();


    void setcurrentsleep(const QString &csleep);
    void setcurrentscreen(const QString &cscreen);
    void setdevicelabel(const QString &description);


    QString sleepValue();
    QString screenValue();

private slots:
    void on_sleepOff_clicked();

    void on_sleepOn_clicked();

    void on_screenOff_clicked();

    void on_screenOn_clicked();

private:
    Ui::sleepDialog *ui;
};

#endif // SLEEPDIALOG_H
