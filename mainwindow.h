#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include "usbfiledialog.h"



namespace Ui {
class MainWindow;
}




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private slots:



    void onReqCompleted();

    void on_actionAbout_triggered();

    void on_connButton_clicked();

    void on_sideload_Button_clicked();

    void on_disButton_clicked();

    void on_actionQuit_triggered();

    void on_uninstall_Button_clicked();

    bool installAPK(QString filename);

    void on_actionHelp_triggered();

    void on_backupButton_clicked();

    void open_pref_database();

    void add_disableroot();

    void on_restoreButton_clicked();

    void  cacheButton_other();
    void  cacheButton_android();

    void on_pushRemote_clicked();

    void on_adbshellButton_clicked();

    void splashButton_android();

  void splashButton_other();

    void finishedCopy1();
    void finishedCopy2();

   // void notAndroid();
   // void isAndroid();

    void delay(int secs);

   void insertDevice();

   void backupAndroid();

   void busybox_permissions();

   void createTables();

    void createPrefTables();

    void editAndroid();

    void editOther();

   QString getadb();

   void  finishedLocalBackup();

   void delayTimer(int rdelay);


   void TimerEvent();

   QString RunProcess(QString cstring);

   QString RunLongProcess(QString cstring, QString jobname);

   void updateDeviceRecord();

   void usbbuttons(bool isusb);

   void rotate_logfile();

   void logfile(QString line);

   void rebootDevice(QString reboot);

   bool mount_system(QString mnt);

   bool fileExists(QString path);

   bool is_su();

   bool is_busybox();


    QString strip (QString str);

    void default_device_values();

    void blank_entry_form();

    void kill_server();

   void start_server();

   bool check_Connection();

  //  void adberror();

   bool is_package(QString package);


   int getRows();


    bool find_daddr(QString);

    void updateRoot();

    void androidLog();

    void updateDevice();

    void copyFolder(QString sourceFolder, QString destFolder);

    void kodiFolder(QString sourceFolder, QString destFolder);

    void getRecord(QString descrip);

    void on_actionReboot_triggered();

    void otherLog();

    void deleteRecord(QString descrip);

    bool isConnectedToNetwork();

    bool searchlistDevices(QString mstring);


    void addcolumn1();

    void on_actionRecovery_triggered();

    void device_clicked(QString item);

    void on_donate_clicked();

   void get_data();


   // void on_actionView_Log_triggered();

    void on_actionDownload_Kodi_triggered();

    void on_editRecord_clicked();

    void dataentry(int flag);

    void data_external(QString externallocation);

    void data_internal(QString externallocation);



    void loaddevicebox();

    void examinedatabase();


    void on_doConsole_clicked();

    void on_killServer_clicked();

    void on_refreshConnectedDevices_clicked();


    bool record_exists(QString descrip);

    QString getDescription(QString mdaddr);

   void on_newRecord_clicked();

  void androidRemote();

  void  otherRemote();

   void on_delRecord_clicked();


   void on_cacheButton_clicked();

   void on_splashButton_clicked();

   void on_mvdataButton_clicked();

   void on_actionView_Kodi_Log_triggered();

   void on_actionView_adbLink_Log_triggered();

   void on_screencapButton_clicked();

   // void on_actionMount_R_W_triggered();

   //void on_actionMount_filesystem_R_O_triggered();

  void restoreAndroid();

/*
  void on_fdellButton_clicked();
  void on_fpushButton_clicked();
  void on_fpullButton_clicked();
  void on_fclearButton_clicked();
*/


   void backupOther();
   void restoreOther();

   void on_actionEdit_XML_files_triggered();


   void on_actionPaste_path_triggered();

   void on_actionStop_Application_triggered();

   void on_actionStart_Application_triggered();

   void on_keypadButton_clicked();



   void on_fmButton_clicked();




   void on_editButton_clicked();

   void on_actionMount_system_RO_triggered();

   void on_actionMount_system_RW_triggered();

   void on_actionWireless_ADBD_triggered();

   void on_actionKodi_data_usage_triggered();

  void on_deviceBox_currentIndexChanged(const QString &arg1);

  void on_listDevices_clicked(const QModelIndex &index);

 void on_listDevices_doubleClicked(const QModelIndex &index);

  void on_clearButton_clicked();

  void on_actionReboot_bootloader_triggered();

  void on_actionPreferences_triggered();

  void updateckversion(int value);

   void updateMsgboxtype(int value);
   void updatedownloaddir(QString value);

   QString getDaddr(QString descrip);

  bool getadbPref();

   bool getmsgboxtype();

   bool check_devices();

  QString getdownloadpath();

  void on_actionDown_SPMC_triggered();

  void on_adhocip_returnPressed();

  void on_actionGet_package_name_triggered();

  void on_actionReiinstall_Busybox_triggered();

  void on_actionArchitecture_triggered();

  void on_actionSleep_adjust_triggered();


private:
    Ui::MainWindow *ui;

 usbfileDialog *fmdialog;

 QStringList RunProcessList;


};




#endif // MAINWINDOW_H
