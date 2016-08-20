# -------------------------------------------------
# Project created by QtCreator 2010-08-22T10:38:43
# -------------------------------------------------
TARGET = kencfs
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
data.path = /usr/share/kencfs/translations/
data.files = *.qm
doc.path = /usr/share/doc/kencfs
doc.files = README preview.png preview2.png
desktop.path = /usr/share/applications
desktop.files = kencfs.desktop
icon.path = /usr/share/icons/kencfs
icon.files = kencfs-icon.png
#INSTALLS += target data desktop
INSTALLS += target data doc desktop icon
CONFIG += release
