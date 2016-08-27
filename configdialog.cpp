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

#include <stdio.h>
#include "configdialog.h"
#include "ui_configdialog.h"
#include <iostream>
using namespace std;


extern bool starthide,browseaftermount,usewallet,wallet_automount;
#include <KWallet>
using KWallet::Wallet;
extern Wallet *m_wallet;
extern QString WalletFolder;
extern bool wallet_opened;

configDialog::configDialog(QWidget *parent) :
        QDialog(parent),
        configui(new Ui::configDialog)
{
configui->setupUi(this);
}


configDialog::~configDialog()
{
}

void configDialog::accept()
{
    starthide=configui->cbstarthide->isChecked();
    browseaftermount=configui->cbbrowse->isChecked();
    usewallet=configui->cbusewallet->isChecked();
    wallet_automount=configui->cbwallet_automount->isChecked();
    if (!usewallet && wallet_opened) {
        cout << "NOT USEWALLET, remove wallet folder for Plasma-EncFS\n";
        m_wallet->removeFolder(WalletFolder);
    }
    this->hide();
}


void configDialog::hideEvent(QHideEvent *e)
{
    (void)e;
}

void configDialog::showEvent(QShowEvent *e)
{
    (void)e;
    configui->cbstarthide->setChecked(starthide);
    configui->cbbrowse->setChecked(browseaftermount);
    configui->cbusewallet->setChecked(usewallet);
    configui->cbwallet_automount->setChecked(wallet_automount);
}


void configDialog::changeEvent(QEvent *e)
{
    (void)e;
}
