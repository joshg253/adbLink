#ifndef PUSHFILEDIALOG_H
#define PUSHFILEDIALOG_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class pushfileDialog;
}

class pushfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit pushfileDialog(QWidget *parent = 0);
    ~pushfileDialog();

    QStringList rstringlist();

    QStringList mstringlist;

private slots:
    void on_okButton_clicked();

private:
    Ui::pushfileDialog *ui;

    QFileSystemModel *dirModel;

};

#endif // PUSHFILEDIALOG_H
