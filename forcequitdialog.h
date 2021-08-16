#ifndef FORCEQUITDIALOG_H
#define FORCEQUITDIALOG_H

#include <QDialog>

namespace Ui {
class forcequitDialog;
}

class forcequitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit forcequitDialog(bool takeaction, QWidget *parent = 0);
    ~forcequitDialog();

    QString packagename();

private slots:
    void on_kodiButton_clicked();

    void on_spmcButton_clicked();

    void on_otherButton_clicked();

private:
    bool m_action;

private:
    Ui::forcequitDialog *ui;
};

#endif // FORCEQUITDIALOG_H

