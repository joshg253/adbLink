#ifndef KEYBOARDDIALOG_H
#define KEYBOARDDIALOG_H

#include <QDialog>

namespace Ui {
class keyboardDialog;
}

class keyboardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit keyboardDialog(QWidget *parent = 0);
    ~keyboardDialog();

void setdaddressLabel(const QString &daddress);

private slots:

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_homeButton_clicked();

    void on_selectButton_clicked();

    void on_fastforwardButton_clicked();

    void on_menuButton_clicked();

    void on_rewindButton_clicked();

    void on_playpauseButton_clicked();

    void on_backButton_clicked();

    void on_powerButton_clicked();

private:
    Ui::keyboardDialog *ui;
};

#endif // KEYBOARDDIALOG_H
