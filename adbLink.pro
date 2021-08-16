#-------------------------------------------------
#
# Project created by QtCreator 2014-05-21T01:43:07
#
#-------------------------------------------------

QT       += core gui sql network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#LIBS += -lQt5Concurrent

TARGET = adbLink
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog2.cpp \
    helpdialog.cpp \
    uninstalldialog.cpp \
    preferencesdialog.cpp \
    usbfiledialog.cpp \
    adblogdialog.cpp \
    kodidialog.cpp \
    datadialog.cpp \
    restdialog.cpp \
    backupdialog.cpp \
    cachedialog.cpp \
    keyboarddialog.cpp \
    listfiledialog.cpp \
    klogdialog.cpp \
    oslogdialog.cpp \
    editordialog.cpp \
    searchdialog.cpp \
    tcpipdialog.cpp \
    forcequitdialog.cpp \
    foldersdialog.cpp \
    pushfiledialog.cpp \
    adbprefdialog.cpp \
    downloaddialog.cpp \
    sleepdialog.cpp


RESOURCES = adbLink.qrc

HEADERS  += mainwindow.h \
    dialog2.h \
    helpdialog.h \
    uninstalldialog.h \
    preferencesdialog.h \
    usbfiledialog.h \
    adblogdialog.h \  
    kodidialog.h \
    datadialog.h \
    restdialog.h \
    listfiledialog.h \
    backupdialog.h \
    cachedialog.h \
    keyboarddialog.h \
    klogdialog.h \
    oslogdialog.h \
    editordialog.h \
    searchdialog.h \
    tcpipdialog.h \    
    forcequitdialog.h \
    foldersdialog.h \
    pushfiledialog.h \
    adbprefdialog.h \
    downloaddialog.h \
    sleepdialog.h


FORMS    += mainwindow.ui \
    dialog2.ui \
    helpdialog.ui \
    uninstalldialog.ui \
    preferencesdialog.ui \
    usbfiledialog.ui \
    adblogdialog.ui \
    kodidialog.ui \
    datadialog.ui \
    restdialog.ui \
    backupdialog.ui \
    cachedialog.ui \
    listfiledialog.ui \
    keyboarddialog.ui \
    klogdialog.ui \
    oslogdialog.ui \
    editordialog.ui \
    searchdialog.ui \
    tcpipdialog.ui \
    forcequitdialog.ui \
    foldersdialog.ui \
    pushfiledialog.ui \
    adbprefdialog.ui \
    downloaddialog.ui \
    sleepdialog.ui

RC_FILE = adbLink.rc

ICON = adbLink.icns

QMAKE_MAC_SDK = macosx10.15

OTHER_FILES +=

#QMAKE_LFLAGS += -no-pie

#CONFIG+=sdk_no_version_check
