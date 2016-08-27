/*****************************************************************
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fsdialog.h"
#include <QSystemTrayIcon>
#include <QStandardItemModel>
#include <iostream>
#include <QMessageBox>
#include "configdialog.h"
#include <KStatusNotifierItem>
#include <QtWidgets/QMenuBar>


using namespace std;
const char *prgname="Plasma-EncFs";
QString description;
const char *cfgfile="plasmaencfsrc";
const char *author="Anton.Filimonov@gmail.com";
const char *email="anton.filimonov@gmail.com";
const char *copyright="(c) 2016";
const char *license="LGPL";
const char *version="0.0.1";

bool starthide=false;
bool browseaftermount=true;
bool usewallet=false;
bool da_pb_browse=false;
bool wallet_automount=false;
KStatusNotifierItem *tray=NULL;

extern fsDialog *fsw;
extern configDialog *configw;

QStandardItemModel *model;

#include <KDE/KWallet/Wallet>
using KWallet::Wallet;
Wallet *m_wallet;
QString WalletFolder="Kencfs";
bool wallet_opened = false;

#include "config.h"
cfg mycfg;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString title=prgname;
    title=title + " - " + copyright + " by " + author;
    
    ui->setupUi(this);
    connect(ui->action_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    //QIcon qi(QIcon::fromTheme("document-encrypt"));
    QIcon qi(QIcon(QString(KENCFS_ICONS) + QString("/kencfs-icon.png")));
    this->setWindowIcon(qi);
    
    tray = new  KStatusNotifierItem("plasma-encfs");
    tray->setCategory(KStatusNotifierItem::SystemServices);
    tray->setIconByPixmap(qi);
    tray->setStatus(KStatusNotifierItem::Active);
    QString a=prgname;
    description=tr("Easy manager for encrypted filesystem");
    a=a+" v"+version+"\n"+description;
    tray->setTitle(a);

    ui->pbadd->setIcon(QIcon::fromTheme("list-add"));
    ui->pbrem->setIcon(QIcon::fromTheme("list-remove"));
    ui->pbmount->setIcon(QIcon::fromTheme("emblem-mounted"));
    ui->pbumount->setIcon(QIcon::fromTheme("emblem-unmounted"));
    //ui->pbhide->setIcon(QIcon::fromTheme("window-hide"));
    ui->pbexit->setIcon(QIcon::fromTheme("application-exit"));
    ui->pbbrowse->setIcon(QIcon::fromTheme("folder-open"));


    model = new QStandardItemModel(0, 4, this);
    QStringList labels;
    labels << tr("FileSystem ID") << tr("Mount Point") << tr("Encrypted Directory") << tr("Status");
    model->setHorizontalHeaderLabels(labels);
    ui->tv->setModel(model);

    ui->tv->resizeColumnToContents(0);
    ui->tv->resizeColumnToContents(1);
    ui->tv->resizeColumnToContents(2);
    ui->tv->resizeColumnToContents(3);

    ui->tv->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *ad=new QAction(ui->tv);
    QAction *rm=new QAction(ui->tv);
    QAction *mo=new QAction(ui->tv);
    QAction *um=new QAction(ui->tv);
    QAction *br=new QAction(ui->tv);
    ad->setText(tr("Add"));
    rm->setText(tr("Remove"));
    mo->setText(tr("Mount"));
    um->setText(tr("Umount"));
    br->setText(tr("Browse"));
    ui->tv->addAction(ad);
    ui->tv->addAction(rm);
    ui->tv->addAction(mo);
    ui->tv->addAction(um);
    ui->tv->addAction(br);
    connect(ad,SIGNAL(triggered()),this,SLOT(pbadd_click()));
    connect(rm,SIGNAL(triggered()),this,SLOT(pbrem_click()));
    connect(mo,SIGNAL(triggered()),this,SLOT(pbmount_click()));
    connect(um,SIGNAL(triggered()),this,SLOT(pbumount_click()));
    connect(br,SIGNAL(triggered()),this,SLOT(pbbrowse_click()));

    loadcfg();
    addmenu();
    connect(model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(model_changed(QStandardItem *)));
    connect(tray,SIGNAL(activateRequested(bool, const QPoint)),this,SLOT(systray_activated(bool,const QPoint)));
    ui->retranslateUi(this);

    if (usewallet) openWallet();

    this->setWindowTitle(title);
  
}

void MainWindow::openWallet() {
    cout << "KDE Wallet Init" << "\n";
    m_wallet = Wallet::openWallet(Wallet::NetworkWallet(),
                                  winId(),
                                  Wallet::Asynchronous);
    connect(m_wallet, SIGNAL(walletOpened(bool)), SLOT(walletOpened(bool)));
}

void MainWindow::walletOpened(bool ok)
{

    if (ok &&
            (m_wallet->hasFolder(WalletFolder) ||
             m_wallet->createFolder(WalletFolder)) &&
            m_wallet->setFolder(WalletFolder)) {
        //QMessageBox::warning(this,"Wallet Status","Wallet Opened",QMessageBox::Ok);
        cout << "KDE Wallet Opened" << "\n";
        wallet_opened = true;

        // TRY TO MOUNT FILESYSTEM IF KDEWALLET IS USED
        if (usewallet && wallet_opened && wallet_automount) {
            int cnt,i;
            cnt = model->rowCount();
            for (i=0; i<cnt; i++) {
                QString id,pm,cr,pw,mn;
                id=model->item(i,0)->text();
                pm=model->item(i,1)->text();
                cr=model->item(i,2)->text();
                mn=model->item(i,3)->text();
                m_wallet->readPassword(id,pw);
                if (pw.length() && !mn.length()) {
                    cout << "Trying to mount Crypted Filesystem '" << id.toLatin1().data() << "' ..." << endl;
                    if (!fsw->effective_mount(id,pm,cr,pw)) {
                        cout << "Mount of '" << id.toLatin1().data() << "' done." << endl;
                    }
                    else {
                        cout << "Mount of '" << id.toLatin1().data() << "' FAILED!" << endl;
                    }
                }
            }
            fsw->checkmount();
        }

    }
    else {
        //QMessageBox::warning(this,"Wallet Status","Wallet NOT Opened",QMessageBox::Ok);
        cout << "KDE Wallet NOT Opened" << "\n";
        wallet_opened = false;
    }
}




void MainWindow::model_changed(QStandardItem *item)
{
    //printf("%s\n",item->text().toLocal8Bit().data());
    (void)item;
    addmenu();
}

void MainWindow::loadcfg()
{
    // caricamento configurazione
    QList<QStringList> a=mycfg.load();
    int i;
    for (i=0; i<a.size(); i++)
    {
        QStandardItem *b=new QStandardItem();
        QStandardItem *c=new QStandardItem();
        QStandardItem *d=new QStandardItem();
        b->setText(a.at(i).at(0));
        c->setText(a.at(i).at(1));
        d->setText(a.at(i).at(2));
        model->setItem(i,0,b);
        model->setItem(i,1,c);
        model->setItem(i,2,d);
        model->sort(0);
        fsw->checkmount();
    }
}

void MainWindow::savecfg()
{
    // salvataggio configurazione
    int i;
    QList<QStringList> b;
    QStringList a;
    model->sort(0);
    for (i=0; i<model->rowCount(); i++)
    {
        a.clear();
        a << model->item(i,0)->text() << model->item(i,1)->text() << model->item(i,2)->text();
        b << a;
    }
    mycfg.save(b);
}


MainWindow::~MainWindow()
{
    savecfg();
    delete ui;
}

void MainWindow::toggleHideShow()
{
    this->setVisible(! this->isVisible() );
}

void MainWindow::pbadd_click()
{
    fsw->show();
}
void MainWindow::pbrem_click()
{
    QString id,pm,a;
    int row;
    if (model->rowCount())
    {
        if (ui->tv->currentIndex().row() >=0)
        {
            row=ui->tv->currentIndex().row();
            id=model->item(row,0)->text();
            pm=model->item(row,1)->text();
            a=tr("Do really you want to remove FileSystem '")+pm+"'' ?";
            if (QMessageBox::warning(this,tr("Remove FileSystem"),a,QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            {
                model->takeRow(row);
                addmenu();
                if (usewallet) {
                    m_wallet->removeEntry(id);
                }
            }
        }
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::pbmount_click()
{
    QString id,pm,cr;
    int row,j;
    //cout << "umount 1\n";
    //cout << "rowCount: " << model->rowCount() << "\n";

    if (usewallet && !wallet_opened) {
        cout << "Wallet not opened, opening Wallet\n";
        openWallet();
    }

    if (model->rowCount())
    {
        if (ui->tv->currentIndex().row() >=0)
        {
            row=ui->tv->currentIndex().row();
            id=model->item(row,0)->text();
            pm=model->item(row,1)->text();
            cr=model->item(row,2)->text();
            j=fsw->createmount(id,pm,cr);
            QStandardItem *e=new QStandardItem();
            e->setText(tr("Mounted"));
            if (j == 0)
            {
                model->setItem(row,3,e);
                if (browseaftermount && !da_pb_browse) fsw->browse(pm);
            }
            else
            {
                e->setText("");
                model->setItem(row,3,e);
            }
            model->sort(0);
        }
    }
}
void MainWindow::addmenu()
{
    int j;
    menuBar()->clear();
    QMenu *f = menuBar()->addMenu( "File" );
    f->addAction(ui->action_Hide);
    f->addAction(ui->action_Configura);
    f->addSeparator();
    f->addAction(ui->action_About);
    f->addAction(ui->action_Quit);
    f->addSeparator();

    QMenu *f2 = new QMenu();
    f2->addAction(ui->action_Hide);
    f2->addAction(ui->action_Configura);
    f2->addSeparator();
    f2->addAction(ui->action_About);
    f2->addAction(ui->action_Quit);
    f2->addSeparator();

    for (j=0; j<model->rowCount(); j++)
    {
        QAction *a=f->addAction(model->item(j,0)->text());
        a->setCheckable(true);
        if (model->item(j,3)->text().length()) a->setChecked(true);
        else a->setChecked(false);
        QAction *a2=f2->addAction(model->item(j,0)->text());
        a2->setCheckable(false);
        if (model->item(j,3)->text().length()) a2->setChecked(true);
        else a2->setChecked(false);
    }
    connect(f,SIGNAL(triggered(QAction *)),this,SLOT(fs_toggled(QAction *)));
    connect(f2,SIGNAL(triggered(QAction *)),this,SLOT(fs_toggled(QAction *)));
    tray->setContextMenu(f2);

}

void MainWindow::fs_toggled(QAction *a)
{
    int row;
    bool t;
    if (a->isCheckable())
    {
        QList<QStandardItem *> b=model->findItems(a->text(),Qt::MatchExactly,0);
        row=b.at(0)->row();
        QString id=model->item(row,0)->text();
        QString pm=model->item(row,1)->text();
        QString cr=model->item(row,2)->text();
        if (b.size())
        {
            t=a->isChecked();
            if (t)
            {
                if (! fsw->createmount(id,pm,cr) && browseaftermount) fsw->browse(pm);
                fsw->checkmount();
            }
            else
            {
                fsw->umount(pm);
                fsw->checkmount();
            }

        }
    }

}

void MainWindow::otherinstance(const QString)
{
    this->show();
}

void MainWindow::pbumount_click()
{
    QString pm;
    int row;
    if (model->rowCount())
    {
        if (ui->tv->currentIndex().row() >=0)
        {
            row=ui->tv->currentIndex().row();
            pm=model->item(row,1)->text();
            fsw->umount(pm);
            fsw->checkmount();
        }
    }
}

void MainWindow::pbbrowse_click()
{
    QString pm,m;
    int row;
    if (model->rowCount())
    {
        if (ui->tv->currentIndex().row() >=0)
        {
            row=ui->tv->currentIndex().row();
            pm=model->item(row,1)->text();
            m=model->item(row,3)->text();
            //if (!m.isEmpty()) fsw->browse(pm);
            if (m.isEmpty())
            {
                da_pb_browse=true;
                pbmount_click();
                da_pb_browse=false;
                m=model->item(row,3)->text();
                if (!m.isEmpty()) fsw->browse(pm);
            }
            else
            {
                fsw->browse(pm);
            }
        }
    }
}

void MainWindow::showabout()
{
    QString a=prgname;
    a=a+" v"+version+"\n"+copyright+" by "+author+"\n"+"eMail: "+email+"\n"+"License: "+license+"\n\n";
    QMessageBox::about(this,"About Plasma-EncFS",a);
    //QMessageBox::about(this,tr("Donations"),tr("Please, consider to make a <a href='http://kde-apps.org/content/donate.php?content=134003'>Donation</a> for my work.\nIf you like this application, please donate something.\nAll money raised will be donated to charity for meritorious initiatives.\n\n"));
}

void MainWindow::showconfig()
{
    configw->show();
}
//void MainWindow::systray_activated(QSystemTrayIcon::ActivationReason a)
void MainWindow::systray_activated(bool active, const QPoint &pos)
{
    //if (a==QSystemTrayIcon::Trigger) toggleHideShow();
    //cout << "systray_activated called\n";
    (void)active;
    (void)pos;
    
    toggleHideShow();
}



void MainWindow::on_tv_doubleClicked(QModelIndex index)
{
    (void)index;
    pbbrowse_click();
    
}
