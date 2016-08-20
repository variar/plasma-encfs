/******************************************************************
** KENCFS - (c) 2010-2014 by Felice Murolo, all rigths reserved
** Author: Felice Murolo, Salerno, Italia
** eMail: linuxboy@RE-MO-VEfel.hopto.org
**
** GNU Lesser General Public License Usage
** KEncFS and it's sources may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**/

#include <QtWidgets>
#include "mainwindow.h"
#include "fsdialog.h"
#include <QtSolutions/QtSingleApplication>
#include "configdialog.h"

fsDialog *fsw;
configDialog *configw;
extern int starthide;

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    if (a.sendMessage("Wake up!")) return 0;

    /*if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, "KEncFS",
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }*/
    QApplication::setQuitOnLastWindowClosed(false);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    QString f="/usr/share/kencfs/translations/kencfs_" + QLocale::system().name();
    if (QFile::exists("./kencfs_" + QLocale::system().name())) f="./kencfs_" + QLocale::system().name();
    myappTranslator.load(f);
    a.installTranslator(&myappTranslator);

    fsw=new fsDialog();
    fsw->hide();

    configw=new configDialog();
    configw->hide();

    MainWindow w;
    a.setActivationWindow(&w);
    QObject::connect(&a, SIGNAL(messageReceived(const QString)),&w, SLOT(otherinstance(const QString)));
    if (!starthide) w.show();
    return a.exec();
}
