#ifndef TCPIPDIALOG_H
#define TCPIPDIALOG_H

#include <QDialog>

namespace Ui {
class tcpipDialog;
}

class tcpipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit tcpipDialog(QWidget *parent = 0);
    ~tcpipDialog();

   int returntcp();

    void settcplabel(const QString &labeltext);

private slots:
    void on_tcpipOn_clicked();

private:
    Ui::tcpipDialog *ui;
};

#endif // TCPIPDIALOG_H
