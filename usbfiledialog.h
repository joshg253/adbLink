#ifndef USBFILEDIALOG_H
#define USBFILEDIALOG_H
#include <QMessageBox>
#include <QDialog>

namespace Ui {
class usbfileDialog;

}

class usbfileDialog : public QDialog

{
    Q_OBJECT

public:
    explicit usbfileDialog(QWidget *parent = 0);
    ~usbfileDialog();

public:


 // void fmactivityIcon(bool value);

void set1FileList(QStringList zstringlist);

void set2FileList(QStringList zstringlist);

void setData(const QString &labelText);

void setADB(const QString &adbdata);



void setPath1(QString currentdir);

void setPath2(QString currentdir);

void setadbdir(QString adbdir);

void setMode(const int &modenum);

void setPackagename(const QString &packagename);

void setkodiPath(const QString &kodipath);

void editfile(QString fileName, QString xpath);

QString RunProcess_ufd(QString cstring);

QString RunLongProcess_ufd(QString cstring);

// void do_copymove(int opcode);

// void do_move();

void do_copy(int opcode);


void setPulldir(const QString &pullstr);

void setAdbdir(const QString &adbstr);

void setdisableroot(const bool &disableroot);

bool checkRoot();

   QString binfileName();


    QString mcpath;

   void setTitle(QString text);

    QString buffer;

    Ui::usbfileDialog *ui;


    QString tmpdir1;
    QString tmpstr1;
    QString commstr1;
    QString adb21;
    QString adbdir_ufd;
    QString cstring;
    QString cstr1;
    QString previous_directory1;
    QString previous_directory2;
    QString pulldir_ufd;
    QString currentitem1;
    QString currentitem2;
    QString current_directory2;
    QString current_directory1;
    QString rootShell;



private slots:



    void on_usblistWidget1_doubleClicked(const QModelIndex &index);
    void on_usblistWidget2_doubleClicked(const QModelIndex &index);

    QString fix_directory(QString dirname);
     bool is_directory(QString fdirectory);

 //   void rootpull();
  //  void shellpull();

    void rootpush(QStringList filenames);
    void userpush(QStringList filenames);

    void on_pushButton_clicked();
    void on_pullButton_clicked();
    void on_copyButton_clicked();
    void on_delButton_clicked();
    void on_editButton_clicked();


    void logfile_ufd(QString line);


    void on_moveButton_clicked();
    void on_resetButton_clicked();
    void on_goButton_clicked();
    void on_clearButton_clicked();

    void on_mkdirButton_clicked();
    void on_renameButton_clicked();
    void on_customdir_returnPressed();

void assignWindow1();
void assignWindow2();
void usbTimerEvent();



};

#endif // USBFILEDIALOG_H
