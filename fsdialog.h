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

#ifndef FSDIALOG_H
#define FSDIALOG_H
#include <QDialog>

namespace Ui {
    class fsDialog;
}

class fsDialog : public QDialog {
    Q_OBJECT
public:
    fsDialog(QWidget *parent = 0);
    ~fsDialog();
    int createmount(QString id,QString pm,QString cr);
    void checkmount();
    int umount(QString pm);
    void browse(QString pm);
    QString which(QString w);
    int effective_mount(QString id,QString pm,QString cr,QString pw);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::fsDialog *fsui;

public slots:

private slots:
    void accept();
    //void reject();
    void tbcrypt_click();
    void tbpmount_click();
    void hideEvent(QHideEvent *e);
};

#endif // FSDIALOG_H
