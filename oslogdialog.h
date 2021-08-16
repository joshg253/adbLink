#ifndef OSLOGDIALOG_H
#define OSLOGDIALOG_H

#include <QDialog>

namespace Ui {
class oslogDialog;
}

class oslogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit oslogDialog(QWidget *parent = 0);
    ~oslogDialog();

private:


   QString logostype;

public:
    void ospassdata(const QString &logostype);




private slots:
    void on_oscopyButton_clicked();

    void on_osswapButton_clicked();

private:
    Ui::oslogDialog *ui;
};

#endif // OSLOGDIALOG_H
