#ifndef LISTFILEDIALOG_H
#define LISTFILEDIALOG_H

#include <QDialog>

namespace Ui {
class listfileDialog;
}

class listfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit listfileDialog(QWidget *parent = 0);
    ~listfileDialog();

  void setFilelist(const QStringList &filelist);
  void setDialogTitle(const QString &dtitle);
  QString return_fitem();

QString fitem;

private slots:
    void on_selectButton_clicked();

    void on_flistWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::listfileDialog *ui;
};

#endif // LISTFILEDIALOG_H
