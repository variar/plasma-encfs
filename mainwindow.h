/******************************************************************
** KENCFS - (c) 2010-2016 by Felice Murolo, all rigths reserved
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QSystemTrayIcon>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadcfg();
    void savecfg();
    void addmenu();
    void openWallet();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void toggleHideShow();
    void pbadd_click();
    void pbrem_click();
    void pbmount_click();
    void pbumount_click();
    void model_changed(QStandardItem * item);
    void fs_toggled(QAction *);
    //void systray_activated(QSystemTrayIcon::ActivationReason);
    void systray_activated(bool active,const QPoint &pos);
    void otherinstance(const QString);
    void showabout();
    void showconfig();
    void pbbrowse_click();
    void on_tv_doubleClicked(QModelIndex index);
    void walletOpened(bool ok);

};


#endif // MAINWINDOW_H
