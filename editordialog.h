#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QDialog>

namespace Ui {
class editorDialog;
}

class editorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editorDialog(QWidget *parent = 0);
    ~editorDialog();

  void seteditor(const QString &xmlfile);

  void setfilename(const QString &filename);

   QString xmlfile();


private slots:
  void on_copyButton_clicked();

  void on_searchButton_clicked();

private:
    Ui::editorDialog *ui;
};

#endif // EDITORDIALOG_H
