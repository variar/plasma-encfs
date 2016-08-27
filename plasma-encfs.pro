TARGET = plasma-encfs
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    fsDialog.cpp \
    config.cpp \
    configdialog.cpp
HEADERS += mainwindow.h \
    fsdialog.h \
    config.h \
    configdialog.h
FORMS += mainwindow.ui \
    fsdialog.ui \
    configdialog.ui

TRANSLATIONS = kencfs_it.ts kencfs_ro.ts kencfs_cs_CZ.ts

QT += widgets KWallet KConfigCore KNotifications
CONFIG += qtsingleapplication

# install
target.path = /usr/bin/
data.path = /usr/share/$${TARGET}/translations/
data.files = *.qm
doc.path = /usr/share/doc/$${TARGET}
doc.files = README preview.png preview2.png
desktop.path = /usr/share/applications
desktop.files = plasma-encfs.desktop
icon.path = /usr/share/icons/$${TARGET}
icon.files = kencfs-icon.png
#INSTALLS += target data desktop
INSTALLS += target data doc desktop icon
CONFIG += release

DEFINES += KENCFS_TRANSLATIONS='\\"$${data.path}\\"'
DEFINES += KENCFS_ICONS='\\"$${icon.path}\\"'
