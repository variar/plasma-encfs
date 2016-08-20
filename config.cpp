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

#include "config.h"
#include <KConfig>
#include <KConfigGroup>

extern char *cfgfile;
extern bool starthide, browseaftermount,usewallet,wallet_automount;

cfg::cfg()
{

}

cfg::~cfg()
{

}

QList<QStringList> cfg::load()
{
    KConfig config(cfgfile, KConfig::SimpleConfig);
    KConfigGroup g( &config, "" );

    QList<QStringList> a;
    QStringList b;

    QStringList l=config.groupList();
    int i;
    for (i=0; i<l.size(); i++)
        {
            if (l.at(i).compare("General"))
            {
                g.changeGroup(l.at(i));
                id=g.readEntry("Id","myfs");
                pmount=g.readEntry("PMount",QString());
                crypt=g.readEntry("Crypt",QString());
                b.clear();
                b << id << pmount << crypt;
                a << b;
            }
            else
            {
                g.changeGroup(l.at(i));
                starthide=g.readEntry("StartHide",false);
                browseaftermount=g.readEntry("BrowseAfterMount",true);
                usewallet=g.readEntry("UseKdeWallet",false);
                wallet_automount=g.readEntry("KdeWallet_AutoMount",false);
            }
        }
    return a;
}

void cfg::save(QList<QStringList> a)
{
    int i;
    QString rm(QDir::homePath()+"/.kde4/share/config/"+cfgfile);
    QFile::remove(rm);
    KConfig config(cfgfile);

    KConfigGroup g( &config, "General" );
    g.writeEntry("StartHide",starthide);
    g.writeEntry("BrowseAfterMount",browseaftermount);
    g.writeEntry("UseKdeWallet",usewallet);
    g.writeEntry("KdeWallet_AutoMount",wallet_automount);

    for (i=0; i<a.size(); i++)
        {
        id=a.at(i).at(0);
        pmount=a.at(i).at(1);
        crypt=a.at(i).at(2);
        g.changeGroup(id);
        g.writeEntry("Id",id);
        g.writeEntry("PMount",pmount);
        g.writeEntry("Crypt",crypt);
        }
}


