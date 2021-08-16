#include "keyboarddialog.h"
#include "ui_keyboarddialog.h"
#include <QMessageBox>
#include <QProcess>
QString cstring;

/*

Key Code Events

0 KEYCODE_UNKNOWN
1 KEYCODE_MENU
2 KEYCODE_SOFT_RIGHT
85 KEYCODE_MEDIA_PLAY_PAUSE
89 KEYCODE_MEDIA_REWIND
90 KEYCODE_MEDIA_FAST_FORWARD
3 KEYCODE_HOME
4 KEYCODE_BACK
5 KEYCODE_CALL
6 KEYCODE_ENDCALL
7 KEYCODE_0
8 KEYCODE_1
9 KEYCODE_2
10 KEYCODE_3
11 KEYCODE_4
12 KEYCODE_5
13 KEYCODE_6
14 KEYCODE_7
15 KEYCODE_8
16 KEYCODE_9
17 KEYCODE_STAR
18 KEYCODE_POUND
19 KEYCODE_DPAD_UP
20 KEYCODE_DPAD_DOWN
21 KEYCODE_DPAD_LEFT
22 KEYCODE_DPAD_RIGHT
23 KEYCODE_DPAD_CENTER
24 KEYCODE_VOLUME_UP
25 KEYCODE_VOLUME_DOWN
26 EYCODE_POWER
27 KEYCODE_CAMERA
28 KEYCODE_CLEAR
29 KEYCODE_A
30 KEYCODE_B
31 KEYCODE_C
32 KEYCODE_D
33 KEYCODE_E
34 KEYCODE_F
35 KEYCODE_G
36 KEYCODE_H
37 KEYCODE_I
38 KEYCODE_J
39 KEYCODE_K
40 KEYCODE_L
41 KEYCODE_M
42 KEYCODE_N
43 KEYCODE_O
44 KEYCODE_P
45 KEYCODE_Q
46 KEYCODE_R
47 KEYCODE_S
48 KEYCODE_T
49 KEYCODE_U
50 KEYCODE_V
51 KEYCODE_W
52 KEYCODE_X
53 KEYCODE_Y
54 KEYCODE_Z
55 KEYCODE_COMMA
56 KEYCODE_PERIOD
224 KEYCODE_WAKEUP
177 KEYCODE_TV_POWER

*/

///////////////////////////////////////////////
QString RunProcess_Keyboard(QString cstring)
{
 QProcess run_command;
 run_command.setProcessChannelMode(QProcess::MergedChannels);
 run_command.start(cstring);

 run_command.waitForStarted();

 while(run_command.state() != QProcess::NotRunning)
     qApp->processEvents();

 QString command=run_command.readAll();

 return command;
}




keyboardDialog::keyboardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyboardDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

}

keyboardDialog::~keyboardDialog()
{
    delete ui;
}


void keyboardDialog::setdaddressLabel(const QString &daddress)
{
    ui->daddressLabel->setText(daddress);
    ui->daddressLabel->setVisible(false);
    cstring = ui->daddressLabel->text();
}



void keyboardDialog::on_upButton_clicked()
{
 QString command=RunProcess_Keyboard(cstring+"19");
}

void keyboardDialog::on_downButton_clicked()
{
    QString command=RunProcess_Keyboard(cstring+"20");
}

void keyboardDialog::on_leftButton_clicked()
{
   QString command=RunProcess_Keyboard(cstring+"21");
}

void keyboardDialog::on_rightButton_clicked()
{
   QString command=RunProcess_Keyboard(cstring+"22");
}

void keyboardDialog::on_homeButton_clicked()
{
   QString command=RunProcess_Keyboard(cstring+"3");
}

void keyboardDialog::on_selectButton_clicked()
{
   QString command=RunProcess_Keyboard(cstring+"23");
}

void keyboardDialog::on_menuButton_clicked()
{
   QString command=RunProcess_Keyboard(cstring+"1");
}

void keyboardDialog::on_backButton_clicked()
{
   QString command=RunProcess_Keyboard(cstring+"4");
}

void keyboardDialog::on_fastforwardButton_clicked()
{
    QString command=RunProcess_Keyboard(cstring+"90");
}

void keyboardDialog::on_rewindButton_clicked()
{
   QString command=RunProcess_Keyboard(cstring+"89");
}

void keyboardDialog::on_playpauseButton_clicked()
{
   QString command=RunProcess_Keyboard(cstring+"85");

}

void keyboardDialog::on_powerButton_clicked()
{
    QString command=RunProcess_Keyboard(cstring+"26");
}
