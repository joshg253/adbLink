#ifndef ADBPREFDIALOG_H
#define ADBPREFDIALOG_H

#include <QDialog>

namespace Ui {
class adbprefDialog;
}

class adbprefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit adbprefDialog(QWidget *parent = 0);
    ~adbprefDialog();

    QString version2;

    void setversioncheck(const bool &versioncheck);
    void setmsgboxtype(const bool &msgboxtype);
    void setversionLabel(const QString &versiontext);
    void setdownloaddir(const QString &ddir);


bool versioncheck();
QString downloaddir();
bool msgboxtype();


private slots:
    void on_checkButton_clicked();
    void onRequestCompleted();

    void on_downloadButton_clicked();

    QString strip2 (QString str);

private:
    Ui::adbprefDialog *ui;
};

#endif // ADBPREFDIALOG_H
