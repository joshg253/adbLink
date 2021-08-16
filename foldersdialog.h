#ifndef FOLDERSDIALOG_H
#define FOLDERSDIALOG_H

#include <QDialog>


namespace Ui {
class foldersDialog;
}

class foldersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit foldersDialog(QWidget *parent = 0);
    ~foldersDialog();

    int fval=0;

    int returnfval();

private slots:
    void on_fileButton_clicked();

    void on_folderButton_clicked();


private:
    Ui::foldersDialog *ui;


};

#endif // FOLDERSDIALOG_H

