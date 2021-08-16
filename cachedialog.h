#ifndef CACHEDIALOG_H
#define CACHEDIALOG_H

#include <QDialog>

namespace Ui {
class cacheDialog;
}

class cacheDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cacheDialog(QWidget *parent = 0);
    ~cacheDialog();

public:
    int cbuffermode();
    QString cbuffersize();
    QString cbufferfactor();
    bool cacheclip();

    void setcbuffermode(const int &cbuffermode);
    void setcbuffersize(const QString &cbuffersize);
    void setcbufferfactor(const QString &cbufferfactor);

    int returncval1();

      void setxmlcheck(const bool &xmlcheck);

      bool xmlcheck();


private slots:
    void on_cpreset1_clicked();

    void on_cpreset2_clicked();

    void on_cpreset3_clicked();

    void on_writecxml_clicked();

    void on_resetcxml_clicked();

    void on_copyxml_clicked();

private:
    Ui::cacheDialog *ui;
};

#endif // CACHEDIALOG_H
