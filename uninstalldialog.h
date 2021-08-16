#ifndef UNINSTALLDIALOG_H
#define UNINSTALLDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>

namespace Ui {
class uninstallDialog;
}

class uninstallDialog : public QDialog
{
    Q_OBJECT


public:
    explicit uninstallDialog(const QString &port, const QString &daddr,QWidget *parent = 0);
    ~uninstallDialog();


private:
        QString m_daddr;
        QString m_port;

public:
   QString packageName();


public:
   bool keepBox();



private slots:


   void loadList();
   void makeFile();
   void loadBox();
   void on_applyButton_clicked();

  // void on_unlistWidget_itemClicked(QListWidgetItem *item);



   void on_apkclearButton_clicked();

private:
    Ui::uninstallDialog *ui;
};

#endif // UNINSTALLDIALOG_H
