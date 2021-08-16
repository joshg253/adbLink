#include "mainwindow.h"
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{

    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    MainWindow w;

     w.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w.size(), qApp->desktop()->availableGeometry()));

     w.show();

     QFont font = qApp->font();
     font.setPixelSize(12);
     qApp->setFont(font);


    return a.exec();
}
