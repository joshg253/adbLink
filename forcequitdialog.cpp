#include "forcequitdialog.h"
#include "ui_forcequitdialog.h"

forcequitDialog::forcequitDialog(bool takeaction, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::forcequitDialog)
{

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


    m_action=takeaction;
    ui->setupUi(this);

      // am start -n org.xbmc.kodi/org.xbmc.kodi.Splash
     // am start -n com.semperpax.spmc16/com.semperpax.spmc16.Splash
     // launchable-activity
    // am force-stop org.xbmc.kodi

    if (m_action)
     {
         ui->title->setText("Start Application");
         ui->explainLabel->setText("String: app starting activity");
         ui->packagename->setText("org.xbmc.kodi/org.xbmc.kodi.Splash");
    }
     else
     {
        ui->title->setText("Stop Application");
        ui->explainLabel->setText("String: app package name");
        ui->packagename->setText("org.xbmc.kodi");
    }

}

forcequitDialog::~forcequitDialog()
{
    delete ui;
}

void forcequitDialog::on_kodiButton_clicked()
{
    if (m_action)
    ui->packagename->setText("org.xbmc.kodi/org.xbmc.kodi.Splash");
    else
    ui->packagename->setText("org.xbmc.kodi");

}

void forcequitDialog::on_spmcButton_clicked()
{
   if (m_action)
       ui->packagename->setText("com.semperpax.spmc16/com.semperpax.spmc16.Splash");
   else
       ui->packagename->setText("com.semperpax.spmc16");
}

void forcequitDialog::on_otherButton_clicked()
{
    ui->packagename->setText("");
}


///////////////////////////////////////////////////
QString forcequitDialog::packagename() {
   return ui->packagename->text();
}
