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

#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>


class cfg  {
public:
    cfg();
    ~cfg();

    QString id;
    QString pmount;
    QString crypt;


    QList<QStringList> load();
    void save(QList<QStringList>);
};



#endif // CONFIG_H
