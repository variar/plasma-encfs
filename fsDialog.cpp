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

#include "fsdialog.h"
#include "ui_fsdialog.h"
#include <QFileDialog>
#include <QStandardItemModel>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>
#include <KDE/KWallet/Wallet>
using KWallet::Wallet;

#include <iostream>
using namespace std;

extern Wallet *m_wallet;
extern bool wallet_opened;

extern QStandardItemModel *model;
QString encfs="encfs";
QString fusermount="fusermount";
QString browser="dolphin";
extern bool browseaftermount,usewallet,wallet_opened;

fsDialog::fsDialog(QWidget *parent) :
        QDialog(parent),
        fsui(new Ui::fsDialog)
{
    fsui->setupUi(this);
    QString w;
    w=which(encfs);
    if (!w.isEmpty()) printf("Found encfs: %s\n",w.toLocal8Bit().data());
    else QMessageBox::critical(this,tr("Fatal Error"),tr("Warning! 'encfs' is not installed.\n\nPlease, install it!\n\n"));
    w=which(fusermount);
    if (!w.isEmpty()) printf("Found fusermount: %s\n",w.toLocal8Bit().data());
    else QMessageBox::critical(this,tr("Fatal Error"),tr("Warning! 'fusermount' is not installed.\n\nPlease, install it!\n\n"));
    w=which(browser);
    if (!w.isEmpty()) printf("Found filebrowser: %s\n",w.toLocal8Bit().data());
    else QMessageBox::critical(this,tr("Fatal Error"),tr("Warning! 'dolphin' is not installed.\n\nPlease, install it!\n\n"));
}





fsDialog::~fsDialog()
{
}

void fsDialog::tbcrypt_click()
{
    QString def=QDir::homePath();
    if (fsui->lecrypt->text().length()) def=fsui->lecrypt->text();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select encrypted existing directory or create new"), def, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.length()) dir=def;
    fsui->lecrypt->setText(dir);
}
void fsDialog::tbpmount_click()
{
    QString def=QDir::homePath();
    if (fsui->lepmount->text().length()) def=fsui->lepmount->text();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select mountpoint existing directory or create new"), def, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.length()) dir=def;
    fsui->lepmount->setText(dir);
}

void fsDialog::accept()
{
    if (fsui->leidfs->text().length() && fsui->lepmount->text().length() && fsui->lecrypt->text().length())
    {
        int i=model->rowCount(),j;
        QStandardItem *b=new QStandardItem();
        QStandardItem *c=new QStandardItem();
        QStandardItem *d=new QStandardItem();
        QStandardItem *e=new QStandardItem();
        b->setText(fsui->leidfs->text());
        c->setText(fsui->lepmount->text());
        d->setText(fsui->lecrypt->text());
        e->setText(tr("Mounted"));
        j=createmount(fsui->leidfs->text(),fsui->lepmount->text(),fsui->lecrypt->text());
        if (j == 0)
        {
            model->appendRow(b);
            // lasciare prima il settaggio dell'item numero 3 !
            // Viene usato fin dal primo cambio del model in addmenu()
            model->setItem(i,3,e);
            model->setItem(i,1,c);
            model->setItem(i,2,d);
            if (browseaftermount) browse(fsui->lepmount->text());
        }
        else model->takeRow(i);
        model->sort(0);
        this->hide();
    }
}

void fsDialog::browse(QString pm)
{
    QProcess *p=new QProcess();
    QString prg(browser);
    QStringList arg;
    arg << pm ;
    p->start(prg,arg);
}


int fsDialog::createmount(QString id,QString pm,QString cr)
{

    if (! QFile::exists(pm) || ! QFile::exists(cr))
    {
        if (! QFile::exists(pm)) QMessageBox::critical(this,tr("Fatal Error"),tr("Warning! MountPoint directory doesn't exists\n\n"));
        else QMessageBox::critical(this,tr("Fatal Error"),tr("Warning! Encrypted directory doesn't exists\n\n"));
        return 1;
    }

    QList<QStandardItem *> a,b;
    a=model->findItems(pm,Qt::MatchExactly,1);
    b=model->findItems(cr,Qt::MatchExactly,2);
    if (a.size() && b.size())
    {
        if (model->item(a.at(0)->row(),3)->text().length()) return 0;
    }

    bool ok=true;
    QString text="";
    if (usewallet && wallet_opened) {
        m_wallet->readPassword(id,text);
        cout << "Read Wallet Password for id: " << id.toLatin1().data() <<"\n";
    }
    if (text.isEmpty()) {
        text=QInputDialog::getText(this,tr("Password Request"),tr("Password for Encrypted FileSystem:"),QLineEdit::Password,"", &ok);
    }
    if (ok && !text.isEmpty())
    {
        return(effective_mount(id,pm,cr,text));
    }
    else
    {
        return 1;
    }
}

int fsDialog::effective_mount(QString id, QString pm,QString cr,QString pw) {
    QProcess *p=new QProcess();
    QString prg(encfs);
    QStringList arg;

    arg << cr << pm << "-S";
    p->start(prg,arg);
    if (! QFile::exists(cr+"/.encfs6.xml")) p->write("\n");
    p->write(pw.toLocal8Bit().data());
    p->write("\n");
    p->waitForFinished();
    QByteArray err_array=p->readAllStandardError();
    QString error_encfs = err_array.data();
    if (p->exitCode())
    {
        QString err = tr("Mount Error!\n\n");
        if (error_encfs.length()>0) err=err+tr("Error returned from Encfs:\n")+error_encfs;
        QMessageBox::critical(this,tr("Fatal Error"),err);
    }
    else if (usewallet && wallet_opened) {
        bool f = m_wallet->writePassword(id, pw);
        if (!f) {
            cout << "Wallet Write Password for id: " << id.toLatin1().data() << "\n";
            //QMessageBox::warning(this,"Wallet Status","Wallet Write Password OK",QMessageBox::Ok);
        }
        else QMessageBox::warning(this,"Wallet Status","KDE Wallet Write Password ERROR",QMessageBox::Ok);
    }
    return p->exitCode();
}



void fsDialog::checkmount()
{
    int j;
    QProcess p1,p2,p3;
    p1.setStandardOutputProcess(&p2);
    p2.setStandardOutputProcess(&p3);
    for (j=0; j<model->rowCount(); j++)
    {
        p1.start("cat /etc/mtab");
        p1.waitForFinished();
        p2.start("fgrep -w "+model->item(j,1)->text());
        p2.waitForFinished();
        p3.start("fgrep "+encfs);
        p3.waitForFinished();
        QByteArray r=p3.readAll();
        QStandardItem *e=new QStandardItem();
        if (strlen(r.data())) e->setText(tr("Mounted"));
        else e->setText("");
        model->setItem(j,3,e);
    }
}
int fsDialog::umount(QString pm)
{
    QProcess p;
    p.start(fusermount+" -u "+pm);
    p.waitForFinished();
    return p.exitCode();
}

//void fsDialog::reject()
//{
//}

void fsDialog::hideEvent(QHideEvent *e)
{
    (void)e;
    fsui->lecrypt->clear();
    fsui->leidfs->clear();
    fsui->lepmount->clear();
}


void fsDialog::changeEvent(QEvent *e)
{
        (void)e;
}

QString fsDialog::which(QString w)
{
    QProcess *p=new QProcess();
    QString prg("which");
    QStringList arg;
    arg << w;
    p->start(prg,arg);
    p->waitForFinished();
    return(p->readAll());
}

