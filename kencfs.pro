# -------------------------------------------------
# Project created by QtCreator 2010-08-22T10:38:43
# -------------------------------------------------
TARGET = kencfs
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    fsDialog.cpp \
    config.cpp \
    qtsingleapplication.cpp \
    qtlocalpeer.cpp \
    qtlockedfile.cpp \
    configdialog.cpp
HEADERS += mainwindow.h \
    fsdialog.h \
    config.h \
    qtsingleapplication.h \
    qtlocalpeer.h \
    qtlockedfile.h \
    configdialog.h
FORMS += mainwindow.ui \
    fsdialog.ui \
    configdialog.ui
OTHER_FILES += 
LIBS += -lkdeui -lkdecore -lQtNetwork
TRANSLATIONS = kencfs_it.ts kencfs_ro.ts kencfs_cs_CZ.ts

#fedora
INCLUDEPATH += "/usr/include/kde4"
LIBS += -L/usr/lib64/kde4/devel

# install
target.path = /usr/bin/
data.path = /usr/share/kde4/apps/kencfs/translations/
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
