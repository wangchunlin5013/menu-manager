/*

 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.

 *

 * Author:     wangcl <wangchunlin@uniontech.com>

 *

 * Maintainer: wangcl <wangchunlin@uniontech.com>

 *

 * This program is free software: you can redistribute it and/or modify

 * it under the terms of the GNU General Public License as published by

 * the Free Software Foundation, either version 3 of the License, or

 * any later version.

 *

 * This program is distributed in the hope that it will be useful,

 * but WITHOUT ANY WARRANTY; without even the implied warranty of

 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

 * GNU General Public License for more details.

 *

 * You should have received a copy of the GNU General Public License

 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#ifndef MENUMANAGER_P_H
#define MENUMANAGER_P_H

#include "menumanager.h"
#include "../common/dataDefine.h"

#include <private/qobject_p.h>

class MenuManagerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(MenuManager)
public:
    MenuManagerPrivate();
    ~MenuManagerPrivate();

    void parseFile();
    void saveFile(const QList<ActionData> &menus);

    QString configPath;
    bool hasCustom = true;

    QList<ActionData> sysMenus;
    QStringList sysMenuTxts;
    QList<ActionData> userMenus;
    QStringList userMenuTxts;
};

#endif // MENUMANAGER_P_H
