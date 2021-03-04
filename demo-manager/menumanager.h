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
#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "../common/dataDefine.h"

#include <QObject>

class MenuManagerPrivate;

class MenuManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MenuManager)
public:
    static MenuManager *instance();

    const QList<ActionData> getAllMenus() const;
    const QStringList getAllMenuTxts() const;

    const QList<ActionData> getUserMenus() const;
    const QStringList getUserMenuTxts() const;

    void saveUserMenus(const QList<ActionData> &userMenus);
private:
    void init();
    bool loadSysMenu();
    bool loadUserMenu();

protected:
    explicit MenuManager(QObject *parent = nullptr);
    MenuManager(MenuManagerPrivate &dd, QObject *parent = nullptr);
};

#endif // MENUMANAGER_H
