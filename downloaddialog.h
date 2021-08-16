#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>

namespace Ui {
class downloadDialog;
}

class downloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit downloadDialog(QWidget *parent = 0);
    ~downloadDialog();

    bool getkodiversion();  
    bool getinstall();
    QStringList getselecteddevices();

    void setdevices(const QStringList &mdevices);

private slots:


    void on_okButton_clicked();

private:
    Ui::downloadDialog *ui;
};

#endif // DOWNLOADDIALOG_H
